#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/videodev2.h>

#include "opencv2/opencv.hpp"
#include "streaming.h"

using namespace cv;
using namespace std;


void Streaming::Streaming(){
	int fbfd = 0;
	long int screensize = 0;
	int count, i;
	this->fbp = 0;
	/* Open the file for reading and writing */
        fbfd = open("/dev/fb0", O_RDWR);

        if (!fbfd) {

            printf("Error: cannot open framebuffer device.\n");
            exit(1);

        }

        printf("The framebuffer device was opened successfully.\n");


        /* Get fixed screen information */
        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {

            printf("Error reading fixed information.\n");
            exit(2);

        }


        /* Get variable screen information */
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {

            printf("Error reading variable information.\n");
            exit(3);

        }


        /* Figure out the size of the screen in bytes */
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;


        /* Map the device to memory */
            this->fbp = (signed *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,fbfd, 0);

        if (this->fbp == MAP_FAILED) {

            printf("Error: failed to map framebuffer device to memory.\n");
            exit(4);

        }
		
}


static void Streaming::hdmi_show(Mat& outputimage)
{
	int x = 0, y = 0;
	long int location = 0;
	int width = outputimage.cols , height = outputimage.rows;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{

			unsigned r, g, b;
            float gray;

			Point p(x % width, y % height);

			//location = (x+vinfo.xoffset) + (y+vinfo.yoffset) * vinfo.xres;
            location = (x+this->vinfo.xoffset) + (y+this->vinfo.yoffset) * this->vinfo.xres;

			b = outputimage.at<Vec3b>(p)[0];
			g = outputimage.at<Vec3b>(p)[1];
			r = outputimage.at<Vec3b>(p)[2];
            gray = 0.2126*r + 0.7152*g + 0.0722*b;
            r = g = b = unsigned(gray); 

			*(this->fbp + location) = r << 16 | g << 8 | b << 0;

		}
	}
}

