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

#define EULERDIS    0
#define CBLOCKDIS   1
#define CBOARDDIS   2

std::string num2str(int num);
std::string double2str(double num);

//color img process
bool cvtBGR2RGB(Mat* src, Mat* dst);
bool cvtRGB2BGR(Mat* src, Mat* dst);
bool cvtRGB2SC(Mat* src, Mat* dst, int channel);
bool cvtRGB2GRAY(Mat* src, Mat* dst);
bool cvtRGB2HSL(Mat* src, Mat* dst);
bool cvtHSL2RGB(Mat* src, Mat* dst);
bool adjusthsl(Mat* src, Mat* dst, int hue, int sat, int lig);
bool adjustColorlevel(Mat* src, Mat* dst, int channel, int ib, double ig, int iw, int ob, int ow);

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
bool scaleC1(Mat* src, Mat* dst, int type);
bool scaleC3(Mat* src, Mat* dst, int type);
bool rotateC1(Mat* src, Mat* dst, double angle, int type);
bool rotateC3(Mat* src, Mat* dst, double angle, int type);
bool cutC1(Mat* src, Mat* dst, int x, int y);
bool cutC3(Mat* src, Mat* dst, int x, int y);

//contrast gradient adjustment
bool getGrayRange(Mat* src, int& min, int& max);
bool linearAdjust(Mat* src, Mat* dst, int min, int max);
bool piecewiseAdjust(Mat* src, Mat* dst, int smin, int smax, int dmin, int dmax);
bool logAdjust(Mat* src, Mat* dst, double coeff1);
bool expAdjust(Mat* src, Mat* dst, double coeff1, double coeff2);
bool equalization(Mat* src, Mat* dst);
bool grayReverse(Mat* src, Mat* dst);
bool Reverse(Mat* src, Mat* dst);

//smoothing filter
bool saltNoise(Mat* src, int n);
bool pepperNoise(Mat* src, int n);
bool filpKernel(Mat* kernel);
bool meanFilter(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool meanFilter(Mat* src, Mat* dst, int rows, int cols, int anchorx, int anchory);
bool medianFilter(Mat* src, Mat* dst, int rows, int cols, int anchorx, int anchory);
bool calGaussianFilter(Mat* kernel, double variance);
bool GaussianFilter(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);

//edge detection
bool convolution(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool SobelDetect(Mat* src, Mat* dst, int type);
bool LaplacianDetect(Mat* src, Mat* dst);
bool CannyDetect(Mat* src, Mat* dst, int low, int high, int size);

//Hough transformation
void HoughLine(Mat* src, Mat* dst, int threshold);

//binary morphologic transformation
void reflectKernel(Mat* kernel);
bool dilation(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool erosion(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
void matAnd(Mat* src1, Mat* src2,Mat* dst);
void matOr(Mat* src1, Mat* src2,Mat* dst);
void matOr(Mat* src, Mat* dst);
void matNot(Mat* src);
void matNot(Mat* src, Mat* dst);
void thinning(Mat* src, Mat* dst);
void thickening(Mat* src, Mat* dst);
bool hitMiss(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool thinning(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool thickening(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool skeleton(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool skeletonReconstruct(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool distanceTrans(Mat* src, Mat* dst, int type);
bool dilationRebuild(Mat* mark, Mat* ground, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool erosionRebuild(Mat* mark, Mat* ground, Mat* dst, Mat* kernel, int anchorx, int anchory);
bool openRebuild(Mat* src, Mat* dst, Mat* kernel, int n, int anchorx, int anchory);
bool closeRebuild(Mat* src, Mat* dst, Mat* kernel, int n, int anchorx, int anchory);

//gray-level morphologic transformation
bool grayDilation(Mat* src, Mat* dst, int krows, int kcols);
bool grayErosion(Mat* src, Mat* dst, int krows, int kcols);
bool grayDilationRebuild(Mat* mark, Mat* ground, Mat* dst, int krows, int kcols);
bool grayErosionRebuild(Mat* mark, Mat* ground, Mat* dst, int krows, int kcols);
bool grayOpenRebuild(Mat* src, Mat* dst, int krows, int kcols, int n);
bool grayCloseRebuild(Mat* src, Mat* dst, int krows, int kcols, int n);
bool waterShed(Mat* src, Mat* dst);


#endif // IMGPROCESS_H
