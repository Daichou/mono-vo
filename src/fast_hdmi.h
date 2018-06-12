
#ifndef _FAST_HDMI_H_
#define _FAST_HDMI_H_

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

void hdmi_init();
void hdmi_show(Mat& outputimage1,Mat& outputimage2);

#endif
