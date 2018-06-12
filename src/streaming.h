#ifndef __FINAL_STREAMING__
#define __FINAL_STREAMING__

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Streaming{
public:
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    
    signed *fbp;
    string cascadeName;

    Streaming();
    void hdmi_show(Mat& outputimage);

}

#endif