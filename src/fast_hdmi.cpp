
#include "fast_hdmi.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/videodev2.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

string cascadeName;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
signed *fbp = 0;

void hdmi_init(){
	int fbfd = 0;
	long int screensize = 0;
	int count, i;
	
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
            fbp = (signed *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,fbfd, 0);

        if (fbp == MAP_FAILED) {

            printf("Error: failed to map framebuffer device to memory.\n");
            exit(4);

        }
		
}


static void hdmi_show(Mat& outputimage)
{
	int x = 0, y = 0;
	long int location = 0;
	int width = outputimage.cols , height = outputimage.rows;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{

			unsigned r, g, b;

			Point p(x % width, y % height);

			location = (x+vinfo.xoffset) + (y+vinfo.yoffset) * vinfo.xres;

			b = outputimage.at<Vec3b>(p)[0];
			g = outputimage.at<Vec3b>(p)[1];
			r = outputimage.at<Vec3b>(p)[2];

			*(fbp + location) = r << 16 | g << 8 | b << 0;

		}
	}
}

