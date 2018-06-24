#include <iostream>
#include <fcntl.h>

#include "streaming.h"


using namespace std;

fb_var_screeninfo Streaming::screen_vinfo;
fb_var_screeninfo Streaming::vinfo;
fb_fix_screeninfo Streaming::screen_finfo;
fb_fix_screeninfo Streaming::finfo;

Streaming::Streaming()
{
    //need to find a way to know error
    if (init_v4l2() == FALSE )
        throw("init v4l2 failed ");

    //init_hdmi();
    v4l2_grab();
    this->asked_for_frame = FALSE;
    this->get_frame=FALSE;

}

Streaming::~Streaming()
{
  stop_streaming();
  close_hdmi();

}

void Streaming::stop_streaming(){
    unsigned int n_buffers;

    // 7. Stop streaming/////////////////////////////////
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(this->fd, VIDIOC_STREAMOFF, &(this->type)))
        perror("Stop streaming error:");
    /////////////////////////////////////////////////////

    // 8. Memory unmap///////////////////////////////////
    for (n_buffers = 0; n_buffers < this->req.count; n_buffers++)
        munmap(this->buffers[n_buffers].start, this->buffers[n_buffers].length);
    /////////////////////////////////////////////////////

    // 9. Close device///////////////////////////////////
    close_v4l2();
    /////////////////////////////////////////////////////
};


void Streaming::init_hdmi()
{
    int x = 0, y = 0;
    long int location = 0;
    this->screensize =0;
    this->hdmi_frame_bfr_pt =0;
    

	// Open the file for reading and writing
	this->hdmi_fd = open("/dev/fb0", O_RDWR);
	if (this->hdmi_fd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(this->hdmi_fd, FBIOGET_FSCREENINFO, &screen_finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(this->hdmi_fd, FBIOGET_VSCREENINFO, &screen_vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", this->screen_vinfo.xres, this->screen_vinfo.yres, this->screen_vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	this->screensize = this->screen_vinfo.xres * this->screen_vinfo.yres * this->screen_vinfo.bits_per_pixel / 8;

	// Map the device to memory
	this->hdmi_frame_bfr_pt = (char *)mmap(0, this->screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
					hdmi_fd, 0);
	if ((int)this->hdmi_frame_bfr_pt == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

}

int Streaming::init_v4l2(){
	// 1. Open device////////////////////////////////////
	if ((this->fd = open(FILE_VIDEO, O_RDWR)) == -1)
	{
		printf("Error opening V4L interface\n");
		return  FALSE;
	}
    /////////////////////////////////////////////////////

    // 2. Device setup///////////////////////////////////
	// Query device capabilities
	if (ioctl(this->fd, VIDIOC_QUERYCAP, &(this->cap) ) == -1)
	{
		printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
		return  FALSE;
	}
	else
	{
     	printf("driver:\t\t%s\n"    , this->cap.driver);
     	printf("card:\t\t%s\n"      , this->cap.card);
     	printf("bus_info:\t%s\n"    , this->cap.bus_info);
     	printf("version:\t%d\n"     , this->cap.version);
     	printf("capabilities:\t%x\n", this->cap.capabilities);

     	if ((this->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
			printf("Device %s: supports capture.\n",FILE_VIDEO);

		if ((this->cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
			printf("Device %s: supports streaming.\n",FILE_VIDEO);
	}

	// Print support format
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(this->fd, VIDIOC_ENUM_FMT, &(this->fmtdesc) ) != -1)
	{
		printf("\t%d.%s\n",this->fmtdesc.index+1,this->fmtdesc.description);
		this->fmtdesc.index++;
	}

    // Set format
    this->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	this->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	this->fmt.fmt.pix.height = IMAGEHEIGHT;
	this->fmt.fmt.pix.width = IMAGEWIDTH;
	this->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    // Set the data format
	if(ioctl(this->fd, VIDIOC_S_FMT, &(this->fmt)) == -1)
	{
		printf("Unable to set format\n");
		return FALSE;
	}
    // Get the data format
	if(ioctl(this->fd, VIDIOC_G_FMT, &(this->fmt) ) == -1)
	{
		printf("Unable to get format\n");
		return FALSE;
	}
	{
     	printf("fmt.type:\t\t%d\n", this->fmt.type);
     	printf("pix.pixelformat:\t%c%c%c%c\n", this->fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF, (fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
     	printf("pix.height:\t\t%d\n", this->fmt.fmt.pix.height);
     	printf("pix.width:\t\t%d\n", this->fmt.fmt.pix.width);
     	printf("pix.field:\t\t%d\n", this->fmt.fmt.pix.field);
	}
	// Set fps
	this->setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	this->setfps.parm.capture.timeperframe.numerator = 10;
	this->setfps.parm.capture.timeperframe.denominator = 10;
    /////////////////////////////////////////////////////

	printf("init %s \t[OK]\n", FILE_VIDEO);

	return TRUE;
}

int Streaming::close_v4l2(){
    if(this->fd != -1) {
        close(this->fd);
        return TRUE;
    }
    return FALSE;
}

void Streaming::close_hdmi(){
  munmap(this->hdmi_frame_bfr_pt, this->screensize);
  close(this->hdmi_fd);
}

int Streaming::v4l2_grab(){
	unsigned int n_buffers;

    // 3. Memory map/////////////////////////////////////
	// Request for 1 buffer
	this->req.count  = 1;
	this->req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	this->req.memory = V4L2_MEMORY_MMAP;
    // Initiate memory mapping or user pointer I/O
	if (ioctl(this->fd, VIDIOC_REQBUFS, &(this->req) ) == -1)
		perror("Request for buffers error:");

	// Memory allocate
	this->buffers = (Streaming::buffer*) malloc(this->req.count * sizeof(*this->buffers));
	if (!this->buffers) {
		perror("Memory allocate error:");
		return FALSE;
	}

    printf("Start query buffer");

	for (n_buffers = 0; n_buffers < this->req.count; n_buffers++)
	{
		this->buf.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		this->buf.memory    = V4L2_MEMORY_MMAP;
		this->buf.index     = n_buffers;
		// Query buffers
		if (ioctl (this->fd, VIDIOC_QUERYBUF, &(this->buf)) == -1) {
			perror("query buffer error\n");
			return FALSE;
		}

		this->buffers[n_buffers].length = this->buf.length;
		// Memory map
		this->buffers[n_buffers].start = mmap(NULL, this->buf.length, PROT_READ |PROT_WRITE, MAP_SHARED, this->fd, this->buf.m.offset);
		if (this->buffers[n_buffers].start == MAP_FAILED) {
			printf("buffer map error\n");
			return FALSE;
		}
	}

    printf("Start queue buffer");
	// Queue
	for (n_buffers = 0; n_buffers < this->req.count; n_buffers++) {
		this->buf.index = n_buffers;
        // Enqueue an empty (capturing) or filled (output) buffer in the driver's incoming queue
		if (ioctl(this->fd, VIDIOC_QBUF, &(this->buf)) )
            perror("Queue buffer error:");
	}
    /////////////////////////////////////////////////////

    printf("Start streaming");
    // 4. Start capturing////////////////////////////////
    // Start streaming I/O
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(this->fd, VIDIOC_STREAMON, &(this->type) )) {
        perror("Start streaming error:");
        return FALSE;
    }
    /////////////////////////////////////////////////////

    printf("Start dequeue buffer");
    // 5. Read buffer////////////////////////////////////
    // Dequeue
	if (ioctl(this->fd, VIDIOC_DQBUF, &(this->buf))) {
        perror("Dequeue buffer error:");
        return FALSE;
    }
    /////////////////////////////////////////////////////

    printf("grab yuyv OK\n");

	return TRUE;
}

void Streaming::askFrame(){
    this->asked_for_frame = TRUE;
    this->buf.index = 0;
    if (ioctl(this->fd, VIDIOC_QBUF, &(this->buf) ))
        perror("Queue buffer error:");
}

cv::Mat Streaming::getFrame(){
    if( this->asked_for_frame == FALSE){
        throw("need to use Streaming::askFrame() before Streaming::getFrame()");
    }
    this->asked_for_frame = FALSE;

    //dequeue frame
    if (ioctl(fd, VIDIOC_DQBUF, &buf)) {
        perror("Dequeue buffer error:");
        throw("Dequeue buffer error");
    }
    //沒意外的話是0
    yuyv_2_rgb888(this->buf.index);

    return cv::Mat(480,640,CV_8UC3,frame_buffer);
}

void Streaming::yuyv_2_rgb888( int buffer_index ){
	int i, j;
    unsigned char y1, y2, u, v;
    int r1, g1, b1, r2, g2, b2;
    char *pointer;

	if (buffer_index >=0){
		pointer = (char*) buffers[buffer_index].start;
	}else{
		pointer = (char*) buffers[0].start;
	}
    for(i = 0; i < IMAGEHEIGHT; i++)
    {
    	for(j = 0;j < (IMAGEWIDTH/2); j++)
    	{
    		y1 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4);
    		u  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 1);
    		y2 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 2);
    		v  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 3);

			/*lab2.4_YUYV2RGB*/

			// RGB1
			r1 = y1 + 1.042*(v-128);
			g1 = y1 - 0.344 * (u - 128) - 0.714 * (v - 128);
			b1 = y1 + 1.772 * (u - 128);

			// RGB2
			r2 = y2+ 1.042 * (v - 128);
			g2 = y2 - 0.344 * (u - 128) - 0.714 * (v - 128);
			b2 = y2 + 1.772 * (u - 128);


            r1 = (r1>255)? 255: ((r1<0)? 0: r1);
            b1 = (b1>255)? 255: ((b1<0)? 0: b1);
            g1 = (g1>255)? 255: ((g1<0)? 0: g1);
            r2 = (r2>255)? 255: ((r2<0)? 0: r2);
            b2 = (b2>255)? 255: ((b2<0)? 0: b2);
            g2 = (g2>255)? 255: ((g2<0)? 0: g2);

    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6    ) = (unsigned char)b1;
    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6 + 1) = (unsigned char)g1;
    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6 + 2) = (unsigned char)r1;
    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6 + 3) = (unsigned char)b2;
    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6 + 4) = (unsigned char)g2;
    		*(frame_buffer + (i*(IMAGEWIDTH/2)+j)*6 + 5) = (unsigned char)r2;
    	}
    }
}


