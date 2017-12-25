#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include <opencv2/opencv.hpp>

using namespace cv;

#define CHANNEL_RGB 0
#define CHANNEL_R   1
#define CHANNEL_G   2
#define CHANNEL_B   3

#define BILINEAR    0
#define NEAREST     1

std::string num2str(int num);

//color img process
bool cvtBGR2RGB(Mat* src, Mat* dst);
bool cvtRGB2BGR(Mat* src, Mat* dst);
bool cvtRGB2SC(Mat* src, Mat* dst, int channel);
bool cvtRGB2GRAY(Mat* src, Mat* dst);
bool cvtRGB2HSL(Mat* src, Mat* dst);
bool cvtHSL2RGB(Mat* src, Mat* dst);
bool adjusthsl(Mat* src, Mat* dst, int hue, int sat, int lig);
bool adjustLevels(Mat* src, Mat* dst);

//binarization of gray img
bool cpMatGray(Mat* src, Mat* dst);
bool getHistogram(Mat* src, int* hist);
int ostuThreshold(Mat* src);
bool ostuBinary(Mat* src, Mat* dst);
bool handsonBinary(Mat* src, Mat* dst, uchar min, uchar max);

//algebra and geometry
bool addMatC1(Mat* src, Mat* dst);
bool addMatC3(Mat* src, Mat* dst);
bool diffMatC1(Mat* src, Mat* dst);
bool diffMatC3(Mat* src, Mat* dst);
bool mulMatC1(Mat* src, Mat* dst);
bool mulMatC3(Mat* src, Mat* dst);
bool addMat(Mat* src, Mat* dst);
bool diffMat(Mat* src, Mat* dst);
bool mulMat(Mat* src, Mat* dst);


#endif // IMGPROCESS_H
