
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


void hdmi_show(Mat& outputimage1 ,Mat& outputimage2)
{
	int x = 0, y = 0;
	long int location = 0;
	int width1 = outputimage1.cols , height1 = outputimage1.rows;

	for(y = 0; y < height1; y++)
	{
		for(x = 0; x < width1; x++)
		{

			unsigned r, g, b;

			Point p(x % width1, y % height1);

			location = (x+vinfo.xoffset) + (y+vinfo.yoffset) * vinfo.xres;

			b = outputimage1.at<Vec3b>(p)[0];
			g = outputimage1.at<Vec3b>(p)[1];
			r = outputimage1.at<Vec3b>(p)[2];

			*(fbp + location) = r << 16 | g << 8 | b << 0;

		}
	}

	int width2 = outputimage2.cols , height2 = outputimage2.rows;
	for(y = 0; y < height2; y++)
	{
		for(x = 0; x < width2; x++)
		{

			unsigned r, g, b;

			Point p(x % width1, y % height1);

			location = (x+vinfo.xoffset) + (y+vinfo.yoffset) * vinfo.xres + 800;

			b = outputimage2.at<Vec3b>(p)[0];
			g = outputimage2.at<Vec3b>(p)[1];
			r = outputimage2.at<Vec3b>(p)[2];

			*(fbp + location) = r << 16 | g << 8 | b << 0;

		}
	}
}

