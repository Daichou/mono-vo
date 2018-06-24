/*

   The MIT License

   Copyright (c) 2015 Avi Singh

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

*/

#include "vo_features.h"
#include "fast_hdmi.h"
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "streaming.h" 

using namespace cv;
using namespace std;

#define MAX_FRAME 1000
int MIN_NUM_FEAT = 2000;

sem_t semaphore;


Mat currImage_c;
Mat traj = Mat::zeros(600, 480, CV_8UC3);

struct Point_t{
    int x;
    int y;
} position;

void* display_thread(void*)
{
    char text[100];
    int fontFace = FONT_HERSHEY_PLAIN;
    double fontScale = 1;
    int thickness = 1;  
    cv::Point textOrg(10, 50);
    while(1){
        sem_wait(&semaphore);
        circle(traj, Point(position.x, position.y) ,1, CV_RGB(255,0,0), 2);

        rectangle( traj, Point(10, 30), Point(550, 50), CV_RGB(0,0,0), CV_FILLED);
        putText(traj, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);

        hdmi_show(  currImage_c,traj );
    }
    pthread_exit(NULL);
}



int main( int argc, char** argv )
{
    Mat img_1, img_2;
    Mat R_f, t_f; //the final rotation and tranlation vectors containing the 

    ofstream myfile;
    myfile.open ("results1_1.txt");

    Streaming video_capture;

    std::cout << "input MIN_NUM_FEAT = " << std::endl;
    std::cin >> MIN_NUM_FEAT;
    double scale = 1.00;

    //VideoCapture capture;
    //capture.set(CV_CAP_PROP_BUFFERSIZE, 1);
    //int camera = 0 ;
    //if (!capture.open(camera)){
    //    cout << "Capture from camera #" <<  camera << " didn't work" << endl;
    //    return -1;
    //}
    //cout << "Video capturing has been started ..." << endl;

    hdmi_init();

    Mat img_1_c;
    Mat img_2_c;
    
    video_capture.askFrame();
    img_1_c = video_capture.getFrame();

    video_capture.askFrame();
    img_2_c = video_capture.getFrame();

    //capture >> img_1_c;
    //capture >> img_2_c;

    if ( !img_1_c.data || !img_2_c.data ) {
        std::cout<< " --(!) Error reading images " << std::endl; return -1;
    }

    // we work with grayscale images
    cvtColor(img_1_c, img_1, COLOR_BGR2GRAY);
    cvtColor(img_2_c, img_2, COLOR_BGR2GRAY);

    // feature detection, tracking
    vector<Point2f> points1, points2;        //vectors to store the coordinates of the feature points
    featureDetection(img_1, points1);        //detect features in img_1
    vector<uchar> status;
    featureTracking(img_1,img_2,points1,points2, status); //track those features to img_2

    //TODO: add a fucntion to load these values directly from KITTI's calib files
    // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
    double focal = 718.8560;
    cv::Point2d pp(607.1928, 185.2157);
    //recovering the pose and the essential matrix
    Mat E, R, t, mask;
    E = findEssentialMat(points2, points1, focal, pp, RANSAC, 0.999, 1.0, mask);
    recoverPose(E, points2, points1, R, t, focal, pp, mask);

    Mat prevImage = img_2;
    Mat currImage;
    vector<Point2f> prevFeatures = points2;
    vector<Point2f> currFeatures;

    char filename[100];

    R_f = R.clone();
    t_f = t.clone();

    clock_t begin = clock();

    sem_init(&semaphore, 0, 0);
    pthread_t display_th;
    pthread_create(&display_th, NULL, display_thread, NULL);

    for(int numFrame=2; numFrame < MAX_FRAME; numFrame++)	{
        //while(!capture.read(currImage_c));
        video_capture.askFrame();
        currImage_c = video_capture.getFrame();
        //capture >> currImage_c;
        cvtColor(currImage_c, currImage, COLOR_BGR2GRAY);
        vector<uchar> status;
        featureTracking(prevImage, currImage, prevFeatures, currFeatures, status);

        E = findEssentialMat(currFeatures, prevFeatures, focal, pp, RANSAC, 0.999, 1.0, mask);
        recoverPose(E, currFeatures, prevFeatures, R, t, focal, pp, mask);

        Mat prevPts(2,prevFeatures.size(), CV_64F), currPts(2,currFeatures.size(), CV_64F);

        std::cout << "index = " << numFrame << "prevPts = " << prevFeatures.size() << std::endl;
        for(int i=0;i<prevFeatures.size();i++)	{   //this (x,y) combination makes sense as observed from the source code of triangulatePoints on GitHub
            prevPts.at<double>(0,i) = prevFeatures.at(i).x;
            prevPts.at<double>(1,i) = prevFeatures.at(i).y;

            currPts.at<double>(0,i) = currFeatures.at(i).x;
            currPts.at<double>(1,i) = currFeatures.at(i).y;
        }

        //scale = getAbsoluteScale(numFrame, 0, t.at<double>(2));
        //scale = 0.5;
        cout << "Scale is " << scale << endl;

        if ((scale>0.1)&&(t.at<double>(2) > t.at<double>(0)) && (t.at<double>(2) > t.at<double>(1))) {

          t_f = t_f + scale*(R_f*t);
          R_f = R*R_f;

        }else {
            cout << "scale below 0.1, or incorrect translation" << endl;
        }

        // lines for printing results
        // myfile << t_f.at<double>(0) << " " << t_f.at<double>(1) << " " << t_f.at<double>(2) << endl;

        // a redetection is triggered in case the number of feautres being trakced go below a particular threshold
        if (prevFeatures.size() < MIN_NUM_FEAT)	{
            cout << "Number of tracked features reduced to " << prevFeatures.size() << endl;
            cout << "trigerring redection" << endl;
            featureDetection(prevImage, prevFeatures);
            featureTracking(prevImage,currImage,prevFeatures,currFeatures, status);

        }

        prevImage = currImage.clone();
        prevFeatures = currFeatures;
        position.x = int(t_f.at<double>(0)) + 300;
        position.y = int(t_f.at<double>(2)) + 100;
        printf("Coordinates: x = %02fm y = %02fm z = %02fm\n", t_f.at<double>(0), t_f.at<double>(1), t_f.at<double>(2));
        sem_post(&semaphore);
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Total time taken: " << elapsed_secs << "s" << endl;
    pthread_join(display_th, NULL);

    return 0;
}
