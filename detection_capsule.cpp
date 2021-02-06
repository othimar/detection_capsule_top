#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void detection_capsule(){
    VideoCapture cap("video.mp4");

    string windowName = "Detection";
    namedWindow(windowName, WINDOW_NORMAL);
    namedWindow("Controls");
    namedWindow("Thresholded");


    int iLowH = 165;
    int iHighH = 179;

    int iLowS = 51;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    createTrackbar("LowH", "Controls", &iLowH, 179);
    createTrackbar("HighH", "Controls", &iHighH, 179);

    createTrackbar("LowS", "Controls", &iLowS, 255);
    createTrackbar("HighS", "Controls", &iHighS, 255);

    createTrackbar("LowV", "Controls", &iLowV, 255);
    createTrackbar("HighV", "Controls", &iHighV, 255);

    while(true)
    {
        Mat frame;
        bool bSuccess = cap.read(frame);
        if(!bSuccess){
            cout<<"Fin de la video"<<endl;
            break;
        }

        Mat imgHSV;
        cvtColor(frame, imgHSV, COLOR_BGR2HSV);

        Mat imgThresholded;
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

        imshow("Thresholded", imgThresholded);

        Moments oMoments = moments(imgThresholded);
        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double aire = oMoments.m00;
        if(aire > 10000){
            double y = dM01/aire;
            double x =  dM10/aire;
            circle(frame, Point(x,y), 60, Scalar(100,255,0));
            imshow(windowName, frame);
        }

        int iKey = waitKey(50);
        if(iKey==27){
            break;
        }
    }
}
