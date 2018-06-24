#ifndef __FINAL_STREAMING__
#define __FINAL_STREAMING__

#include "v4l2grab.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "opencv2/mat.hpp"



#define TRUE 1
#define FALSE 0

#define  IMAGEWIDTH    640
#define  IMAGEHEIGHT   480

class Streaming{
public:

    /*
        Retreving frame with asynchronous style
        use askFrame() to let machine to get buffer
        and getFrame
    */
    void askFrame();

    //要改成回傳 opencv2的 mat架構
    int getFrame();
    //init hdmi
    Streaming();
    
    //close hdmi
    ~Streaming();

    //等待實踐的功能
    //void hdmi_show(Mat& outputimage);

private:
    bool asked_for_frame;
    bool get_frame;

    void init_hdmi();
    void close_hdmi();
    int init_v4l2();

    int close_v4l2();
    int v4l2_grab();
    void yuyv_2_rgb888(int buffer_index);

    struct buffer
    {
        void * start;
        unsigned int length;
    }*buffers;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    struct  v4l2_capability     cap;
    struct  v4l2_fmtdesc        fmtdesc;
    struct  v4l2_format         fmt,fmtack;
    struct  v4l2_streamparm     setfps;
    struct  v4l2_requestbuffers req;
    struct  v4l2_buffer         buf;
    enum    v4l2_buf_type       type;

    int fd;
    unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];
    int hdmi_fd;
    long int screensize;
    char *hdmi_frame_bfr_pt;
}

#endif