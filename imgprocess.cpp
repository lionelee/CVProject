#include "imgprocess.h"
#include <stdio.h>
#include <strstream>
#include <QMessageBox>
#include <vector>
#include <algorithm>
#include <QDebug>

#define PI 3.1415926

std::string num2str(int num){
    std::stringstream ss;
    ss<<num;
    std::string s;
    ss>>s;
    return s;
}

bool cvtBGR2RGB(Mat* src, Mat* dst)
{
    if(src->channels() != 3)return false;
    int rows = src->rows, cols = src->cols * src->channels();
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; j+=3){
            data_out[j]   = data_in[j+2];
            data_out[j+1] = data_in[j+1];
            data_out[j+2] = data_in[j];
        }
    }
    return true;
}


bool cvtRGB2BGR(Mat* src, Mat* dst)
{
    return cvtBGR2RGB(src, dst);
}

bool cvtRGB2SC(Mat* src, Mat* dst, int channel)
{
    if(src->channels() != 3 )return false;
    int offset = 0;
    switch(channel){
    case CHANNEL_R:
        offset = 0;
        break;
    case CHANNEL_G:
        offset = 1;
        break;
    case CHANNEL_B:
        offset = 2;
        break;
    default:return false;
    }
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            data_out[j] = data_in[3*j+offset];
        }
    }
    return true;
}

bool cvtRGB2GRAY(Mat* src, Mat* dst)
{
    if(src->channels() != 3 )return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            data_out[j] = (data_in[3*j]*76 + data_in[3*j+1]*150 + data_in[3*j+2]*30) >> 8;
        }
    }
    return true;
}

bool cvtRGB2HSL(Mat *src, Mat *dst)
{
    if(src->channels() != 3 )return false;
    int rows = src->rows, cols = src->cols * src->channels();
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        double* data_out = dst->ptr<double>(i);
        for(int j = 0; j < cols; j+=3){
            double R = data_in[j] / 255.0;
            double G = data_in[j+1] / 255.0;
            double B = data_in[j+2] / 255.0;
            double min = MIN(MIN(R,G),B);
            double max = MAX(MAX(R,G),B);

            double delta = max - min;
            double H = 0.0;
            if (delta == 0.0){
                H = 0.0;
            }else if (max == R){
                H = (G-B)/(6*delta);
            }else if (max == G){
                H = (B-R)/(6*delta)+(1.0/3.0);
            }else if (max == B){
                H = (R-G)/(6*delta)+(2.0/3.0);
            }
            if(H < 0) H += 1.0;
            if(H > 1) H -= 1.0;

            double L = (max + min)/2;
            double S = 0;
            if (L == 0.0 || delta == 0.0){
                S = 0;
            }else if (L > 0.0 && L < 0.5){
                S = delta/(max+min);
            }else if (L >=
                      0.5){
                S = delta/(2-max-min);
            }
            data_out[j] = H;
            data_out[j+1] = S;
            data_out[j+2] = L;
        }
    }
    return true;
}

inline uchar hsl2rbg(double v1, double v2, double v3)
{
    if (v3 < 0) v3 += 1.0;
    if (v3 > 1) v3 -= 1.0;
    if ((6*v3) < 1.0){
        return (uchar)round(255*(v1+(v2-v1)*6*v3));
    }
    if ((2*v3) < 1.0){
        return (uchar)round(255*v2);
    }
    if ((3*v3) < 2.0){
        return (uchar)round(255*(v1+(v2-v1)*((2.0/3.0)-v3)*6));
    }
    return (uchar)round(255*v1);
}

bool cvtHSL2RGB(Mat *src, Mat *dst)
{
    if(src->channels() != 3 )return false;
    int rows = src->rows, cols = src->cols * src->channels();
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        double* data_in = src->ptr<double>(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; j+=3){
            double H = data_in[j];
            double S = data_in[j+1];
            double L = data_in[j+2];
            uchar R, G, B;
            if(S ==0.0){
                R = G = B = (uchar)round(L*255);
            }else{
                double v1, v2;
                if (L < 0.5){
                    v2 = L * (1.0 + S);
                }else{
                    v2 = L + S - L * S;
                }
                v1 = 2.0 * L - v2;
                R = hsl2rbg(v1, v2, H+1.0/3.0);
                G = hsl2rbg(v1, v2, H);
                B = hsl2rbg(v1, v2, H-1.0/3.0);
                data_out[j] = R;
                data_out[j+1] = G;
                data_out[j+2] = B;
            }

        }
    }
    return true;
}

bool adjusthsl(Mat *src, Mat* dst, int hue, int sat, int lig)
{
    if(src->channels() != 3 )return false;
    int rows = src->rows, cols = src->cols * src->channels();
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    double dH = hue/240.0, dS = sat/240.0, dL = lig/240.0;
    for(int i = 0; i < rows; ++i){
        double* data_in = src->ptr<double>(i);
        double* data_out = dst->ptr<double>(i);
        for(int j = 0; j < cols; j+=3){
            data_out[j] = data_in[j] + dH;
            if(data_out[j] > 1.0) data_out[j] = 1.0;
            data_out[j+1] = data_in[j+1] + dS;
            if(data_out[j+1] > 1.0) data_out[j+1] = 1.0;
            data_out[j+2] = data_in[j+2] + dL;
            if(data_out[j+2] > 1.0) data_out[j+2] = 1.0;
        }
    }
    return true;
}

bool adjustColorlevel(Mat* src, Mat* dst, int channel, int ib, double ig, int iw, int ob, int ow)
{
    if(src->channels() != 3 )return false;
    src->copyTo(*dst);
    double diff = MAX(iw-ib, 1), coeffi = 1 / ig, coeffo = (ow-ob)/(double)255;
    for(int i = 0; i < src->rows; ++i){
        Vec3b* data_in = src->ptr<Vec3b>(i);
        Vec3b* data_out = dst->ptr<Vec3b>(i);
        for(int j = 0; j < src->cols; ++j){
            int idiff = 0;
            double tmp = 0;
            switch (channel) {
            case CHANNEL_RGB:
            case CHANNEL_R:
                idiff = data_in[j][0]-ib;
                if(idiff <= 0)data_out[j][0] = ob;
                else{
                    tmp = 255*pow(idiff/(double)diff, coeffi);
                    tmp = MIN(tmp, 255);
                    tmp = tmp*coeffo+ob;
                    data_out[j][0] = (uchar)MIN(round(tmp), 255);
                }
                if(channel!=CHANNEL_RGB)break;
            case CHANNEL_G:
                idiff = data_in[j][1]-ib;
                if(idiff <= 0)data_out[j][1] = ob;
                else{
                    tmp = 255*pow(idiff/(double)diff, coeffi);
                    tmp = MIN(tmp, 255);
                    tmp = tmp*coeffo+ob;
                    data_out[j][1] = (uchar)MIN(round(tmp), 255);
                }
                if(channel!=CHANNEL_RGB)break;
            case CHANNEL_B:
                idiff = data_in[j][2]-ib;
                if(idiff <= 0)data_out[j][2] = ob;
                else{
                    tmp = 255*pow(idiff/(double)diff, coeffi);
                    tmp = MIN(tmp, 255);
                    tmp = tmp*coeffo+ob;
                    data_out[j][2] = (uchar)MIN(round(tmp), 255);
                }
                if(channel!=CHANNEL_RGB)break;
            default: break;
            }
        }
    }
    return true;
}


//binarization of gray img
bool cpMatGray(Mat *src, Mat *dst)
{
    if(src->channels() != 1)return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            data_out[j] = data_in[j];
        }
    }
    return true;
}

bool getHistogram(Mat *src, int* hist)
{
    if(src->channels() != 1)return false;
    int rows = src->rows, cols = src->cols, size = rows*cols;
    if (src->isContinuous()) {
        cols = size ;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        for(int j = 0; j < cols; ++j){
            int idx = data_in[j];
            ++hist[idx];
        }
    }
    return true;
}

int ostuThreshold(Mat* src)
{
    if(src->channels() != 1)return false;
    int rows = src->rows, cols = src->cols, size = rows*cols;
    if (src->isContinuous()) {
        cols = size ;
        rows = 1;
    }
    int hist[256];
    memset(hist,0,sizeof(hist));
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        for(int j = 0; j < cols; ++j){
            int idx = data_in[j];
            ++hist[idx];
        }
    }

    double histp[256];
    memset(histp,0,sizeof(histp));
    for(int i = 0; i < 256; ++i){
        histp[i] = hist[i] / (double)size;
    }

    double avg_gray = 0;
    for(int i = 0; i < 256; ++i){
        avg_gray += histp[i] * i;
    }

    int threshold = 0;
    double theta = 0, max_theta = 0;
    for(int k = 0; k < 256; ++k){
        double p0 = 0, g0 = 0, p1 = 0, g1 = 0;
        for(int i = 0; i < k; ++i){
            p0 += histp[i];
            g0 += histp[i] * i;
        }
        p1 = 1 - p0;
        g1 = (avg_gray - g0) / p1;
        g0 = g0 / p0;

        theta = p0*pow(g0-avg_gray,2) + p1*pow(g1-avg_gray,2);
        if(theta > max_theta){
            max_theta = theta;
            threshold = k;
        }
    }
    return threshold;
}

bool ostuBinary(Mat *src, Mat *dst)
{
    if(src->channels() != 1)return false;
    uchar threshold = (uchar)ostuThreshold(src);
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            if(data_in[j] < threshold){
                data_out[j] = 0;
            }else{
                data_out[j] = 255;
            }
        }
    }
    return true;
}

bool handsonBinary(Mat* src, Mat* dst, uchar min, uchar max)
{
    if(src->channels() != 1)return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols = rows * cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            if(data_in[j] > min && data_in[j] < max){
                data_out[j] = 255;
            }else{
                data_out[j] = 0;
            }
        }
    }
    return true;
}


//algebra and geometry
bool addMatC1(Mat* src, Mat* dst)
{
    if(src->channels() != 1 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols);
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            int res = data_in[j] + data_out[j];
            data_out[j] = MIN(res, 255);
        }
    }
    return true;
}

bool addMatC3(Mat* src, Mat* dst)
{
    if(src->channels() != 3 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols) * 3;
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr<uchar>(i);
        uchar* data_out = dst->ptr<uchar>(i);
        for(int j = 0; j < cols; j += 3){
            int res = data_in[j] + data_out[j];
            data_out[j] = MIN(res, 255);
            res = data_in[j+1] + data_out[j+1];
            data_out[j+1] = MIN(res, 255);
            res = data_in[j+2] + data_out[j+2];
            data_out[j+2] = MIN(res, 255);
        }
    }
    return true;
}

bool diffMatC1(Mat* src, Mat* dst)
{
    if(src->channels() != 1 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols);
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            int res = data_in[j] - data_out[j];
            data_out[j] = MAX(res, 0);
        }
    }
    return true;
}

bool diffMatC3(Mat* src, Mat* dst)
{
    if(src->channels() != 3 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols) * 3;
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; j += 3){
            int res = data_in[j] - data_out[j];
            data_out[j] = MAX(res, 0);
            res = data_in[j+1] - data_out[j+1];
            data_out[j+1] = MAX(res, 0);
            res = data_in[j+2] - data_out[j+2];
            data_out[j+2] = MAX(res, 0);
        }
    }
    return true;
}

bool mulMatC1(Mat* src, Mat* dst)
{
    if(src->channels() != 1 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols);
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            int res = data_in[j] * data_out[j];
            data_out[j] = MIN(res, 255);
        }
    }
    return true;
}

bool mulMatC3(Mat *src, Mat *dst)
{
    if(src->channels() != 3 || src->channels() != dst->channels())return false;
    int rows = MIN(src->rows, dst->rows);
    int cols = MIN(src->cols, dst->cols) * 3;
    if(src->isContinuous() && dst->isContinuous()){
        cols = rows*cols;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; j += 3){
            int res = data_in[j] * data_out[j];
            data_out[j] = MIN(res, 255);
            res = data_in[j+1] * data_out[j+1];
            data_out[j+1] = MIN(res, 255);
            res = data_in[j+2] * data_out[j+2];
            data_out[j+2] = MIN(res, 255);
        }
    }
    return true;
}

bool addMat(Mat* src, Mat* dst)
{
    if(src->channels()==1){
        return addMatC1(src, dst);
    }else if(src->channels()==3){
        return addMatC3(src, dst);
    }else{
        return false;
    }
}

bool diffMat(Mat* src, Mat* dst)
{
    if(src->channels()==1){
        return diffMatC1(src, dst);
    }else if(src->channels()==3){
        return diffMatC3(src, dst);
    }else{
        return false;
    }
}

bool mulMat(Mat* src, Mat* dst)
{
    if(src->channels()==1){
        return mulMatC1(src, dst);
    }else if(src->channels()==3){
        return mulMatC3(src, dst);
    }else{
        return false;
    }
}

bool scaleC1(Mat* src, Mat* dst, int type)
{
    if(src->channels() != 1)return false;
    int rows = dst->rows, cols = dst->cols;
    double xratio = src->rows / (double)rows, yratio = src->cols / (double)cols;

    for(int i = 0; i < rows; ++i){
        double y_res = i * xratio;
        for(int j = 0; j < cols; ++j){
            double x_res = j * yratio;
            if(y_res < 0 || y_res >= src->rows || x_res < 0 || x_res >= src->cols)continue;
            if(type == BILINEAR){
                int y = (int)y_res;
                double v0 = y_res - y, v1 = 1.0 - v0;
                int x = (int)x_res;
                double u0 = x_res - x, u1 = 1.0 - u0;
                int y1 = y;
                if(y < src->rows-1)++y1;
                int x1 = x;
                if(x1 < src->cols-1)++x1;

                double gray = v1*(u0*src->ptr(y)[x1]+u1*src->ptr(y)[x])+
                             v0*(u0*src->ptr(y1)[x1]+u1*src->ptr(y1)[x]);
                dst->ptr(i)[j] = (uchar)round(gray);

            }else if(type == NEAREST){
                int y = (uchar)MIN(src->rows-1,round(y_res)), x = (uchar)MIN(src->cols-1,round(x_res));
                dst->ptr(i)[j] = src->ptr(y)[x];
            }else{
                return false;
            }

        }
    }
    return true;
}

bool scaleC3(Mat* src, Mat* dst, int type)
{
    if(src->channels() != 3)return false;
    int rows = dst->rows, cols = dst->cols;
    double xratio = src->rows / (double)rows, yratio = src->cols / (double)cols;

    for(int i = 0; i < rows; ++i){
        double y_res = i * xratio;
        for(int j = 0; j < cols; ++j){
            double x_res = j * yratio;
            if(y_res < 0 || y_res >= src->rows || x_res < 0 || x_res >= src->cols)continue;
            if(type == BILINEAR){
                int y = (int)y_res;
                double v0 = y_res - y, v1 = 1.0 - v0;
                int x = (int)x_res;
                double u0 = x_res - x, u1 = 1.0 - u0;
                int y1 = y;
                if(y < src->rows-1)++y1;
                int x1 = x;
                if(x1 < src->cols-1)++x1;

                double red = v1*(u0*src->ptr<Vec3b>(y)[x1][0]+u1*src->ptr<Vec3b>(y)[x][0])+
                             v0*(u0*src->ptr<Vec3b>(y1)[x1][0]+u1*src->ptr<Vec3b>(y1)[x][0]);
                double green = v1*(u0*src->ptr<Vec3b>(y)[x1][1]+u1*src->ptr<Vec3b>(y)[x][1])+
                        v0*(u0*src->ptr<Vec3b>(y1)[x1][1]+u1*src->ptr<Vec3b>(y1)[x][1]);
                double blue = v1*(u0*src->ptr<Vec3b>(y)[x1][2]+u1*src->ptr<Vec3b>(y)[x][2])+
                        v0*(u0*src->ptr<Vec3b>(y1)[x1][2]+u1*src->ptr<Vec3b>(y1)[x][2]);
                dst->ptr<Vec3b>(i)[j][0] = (uchar)round(red);
                dst->ptr<Vec3b>(i)[j][1] = (uchar)round(green);
                dst->ptr<Vec3b>(i)[j][2] = (uchar)round(blue);

            }else if(type == NEAREST){
                int y = (uchar)MIN(src->rows-1,round(y_res)), x = (uchar)MIN(src->cols-1,round(x_res));
                dst->ptr<Vec3b>(i)[j][0] = src->ptr<Vec3b>(y)[x][0];
                dst->ptr<Vec3b>(i)[j][1] = src->ptr<Vec3b>(y)[x][1];
                dst->ptr<Vec3b>(i)[j][2] = src->ptr<Vec3b>(y)[x][2];
            }else{
                return false;
            }

        }
    }
    return true;
}

bool rotateC1(Mat* src, Mat* dst, double angle, int type)
{
    if(src->channels()!=1)return false;
    int rows = dst->rows, cols = dst->cols;
    double scx = src->cols / 2, scy = src->rows / 2;
    double centerx = cols / 2, centery = rows / 2;
    double ca = cos(angle), sa = sin(angle);

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j)
            dst->ptr(i)[j]=0;
    }

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            double y_res = 0, x_res = 0;
            y_res = (i-centery)*ca-(j-centerx)*sa+scy;
            x_res = (i-centery)*sa+(j-centerx)*ca+scx;
            if(y_res < 0 || y_res >= src->rows || x_res < 0 || x_res >= src->cols)continue;
            if(type == BILINEAR){
                int y = (int)y_res;
                double v0 = y_res - y, v1 = 1.0 - v0;
                int x = (int)x_res;
                double u0 = x_res - x, u1 = 1.0 - u0;
                int y1 = y;
                if(y < src->rows-1)++y1;
                int x1 = x;
                if(x1 < src->cols-1)++x1;

                double gray = v1*(u0*src->ptr(y)[x1]+u1*src->ptr(y)[x])+
                             v0*(u0*src->ptr(y1)[x1]+u1*src->ptr(y1)[x]);
                dst->ptr(i)[j] = (uchar)round(gray);

            }else if(type == NEAREST){
                int y = (uchar)MIN(src->rows-1,round(y_res)), x = (uchar)MIN(src->cols-1,round(x_res));
                dst->ptr(i)[j] = src->ptr(y)[x];
            }else{
                return false;
            }
        }
    }
    return true;
}

bool rotateC3(Mat* src, Mat* dst, double angle, int type)
{
    if(src->channels()!=3)return false;
    int rows = dst->rows, cols = dst->cols;
    double centerx = cols / 2, centery = rows / 2;
    double scx = src->cols / 2, scy = src->rows / 2;
    double ca = cos(angle), sa = sin(angle);

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols*3; ++j)
            dst->ptr(i)[j]=0;
    }

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            double y_res = 0, x_res = 0;
            y_res = (i-centery)*ca-(j-centerx)*sa+scy;
            x_res = (i-centery)*sa+(j-centerx)*ca+scx;
            if(y_res < 0 || y_res >= src->rows || x_res < 0 || x_res >= src->cols)continue;
            if(type == BILINEAR){
                int y = (int)y_res;
                double v0 = y_res - y, v1 = 1.0 - v0;
                int x = (int)x_res;
                double u0 = x_res - x, u1 = 1.0 - u0;
                int y1 = y;
                if(y < src->rows-1)++y1;
                int x1 = x;
                if(x1 < src->cols-1)++x1;

                double red = v1*(u0*src->ptr<Vec3b>(y)[x1][0]+u1*src->ptr<Vec3b>(y)[x][0])+
                             v0*(u0*src->ptr<Vec3b>(y1)[x1][0]+u1*src->ptr<Vec3b>(y1)[x][0]);
                double green = v1*(u0*src->ptr<Vec3b>(y)[x1][1]+u1*src->ptr<Vec3b>(y)[x][1])+
                        v0*(u0*src->ptr<Vec3b>(y1)[x1][1]+u1*src->ptr<Vec3b>(y1)[x][1]);
                double blue = v1*(u0*src->ptr<Vec3b>(y)[x1][2]+u1*src->ptr<Vec3b>(y)[x][2])+
                        v0*(u0*src->ptr<Vec3b>(y1)[x1][2]+u1*src->ptr<Vec3b>(y1)[x][2]);
                dst->ptr<Vec3b>(i)[j][0] = (uchar)round(red);
                dst->ptr<Vec3b>(i)[j][1] = (uchar)round(green);
                dst->ptr<Vec3b>(i)[j][2] = (uchar)round(blue);

            }else if(type == NEAREST){
                int y = (uchar)MIN(src->rows-1,round(y_res)), x = (uchar)MIN(src->cols-1,round(x_res));
                dst->ptr<Vec3b>(i)[j][0] = src->ptr<Vec3b>(y)[x][0];
                dst->ptr<Vec3b>(i)[j][1] = src->ptr<Vec3b>(y)[x][1];
                dst->ptr<Vec3b>(i)[j][2] = src->ptr<Vec3b>(y)[x][2];
            }else{
                return false;
            }
        }
    }
    return true;
}

bool cutC1(Mat* src, Mat* dst, int x, int y)
{
    if(src->channels()!=1)return false;
    int h = dst->rows, w = dst->cols;
    int rangex = x + w, rangey = y + h;
    for(int i = y; i < rangey; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i-y);
        for(int j = x; j < rangex; ++j){
            data_out[j-x] = data_in[j];
        }
    }
    return true;
}

bool cutC3(Mat* src, Mat* dst, int x, int y)
{
    if(src->channels()!=3)return false;
    int h = dst->rows, w = dst->cols;
    int rangex = x + w, rangey = y + h;
    for(int i = y; i < rangey; ++i){
        Vec3b* data_in = src->ptr<Vec3b>(i);
        Vec3b* data_out = dst->ptr<Vec3b>(i-y);
        for(int j = x; j < rangex; ++j){
            data_out[j-x][0] = data_in[j][0];
            data_out[j-x][1] = data_in[j][1];
            data_out[j-x][2] = data_in[j][2];
        }
    }
    return true;
}

//contrast gradient adjustment
bool getGrayRange(Mat* src, int& min, int& max)
{
    if(src->channels() != 1)return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols *= rows;
        rows = 1;
    }
    max = min = 0;
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        for(int j = 0; j < cols; ++j){
            if(data_in[j] < min){
                min = data_in[i];
            }else if (data_in[j] > max){
                max = data_in[j];
            }
        }
    }
    return true;
}

bool linearAdjust(Mat* src, Mat* dst, int min, int max)
{
    if(src->channels()!=1)return false;
    int smin = 0, smax = 0;
    getGrayRange(src, smin, smax);
    int rows = src->rows, cols = src->cols;
    double slope = (max - min) / (double)(smax - smin);
    double inter = min - slope * smin;
    if (src->isContinuous()) {
        cols *= rows;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            double tmp = round(data_in[j]*slope+inter);
            data_out[j] = (uchar)MIN(tmp, (double)255);
        }
    }
    return true;
}

bool piecewiseAdjust(Mat* src, Mat* dst, int smin, int smax, int dmin, int dmax)
{
    if(src->channels()!=1)return false;
    int rows = src->rows, cols = src->cols;
    int src_min = 0, src_max = 0;
    getGrayRange(src, src_min, src_max);
    double slope1 = (dmin - src_min) / (double)(smin - src_min);
    double inter1 = src_min - slope1 * src_min;
    double slope2 = (dmax - dmin) / (double)(smax - smin);
    double inter2 = dmin - slope2 * smin;
    double slope3 = (src_max - dmax) / (double)(src_max - smax);
    double inter3 = dmax - slope3 * smax;
    if (src->isContinuous()) {
        cols *= rows;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            double tmp = 0;
            if(data_in[j] < smin){
                tmp = round(data_in[j]*slope1+inter1);
            }else if(data_in[j] <smax){
                tmp = round(data_in[j]*slope2+inter2);
            }else{
                tmp = round(data_in[j]*slope3+inter3);
            }
            data_out[j] = (uchar)MIN(tmp, (double)255);
        }
    }
    return true;
}

bool logAdjust(Mat* src, Mat* dst, double coeff)
{
    if(src->channels()!=1)return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols *= rows;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            double tmp = round(coeff*log(1+data_in[j]));
            data_out[j] = (uchar)MIN(tmp, (double)255);
        }
    }
    return true;
}

bool expAdjust(Mat* src, Mat* dst, double coeff1, double coeff2)
{
    if(src->channels()!=1)return false;
    int rows = src->rows, cols = src->cols;
    if (src->isContinuous()) {
        cols *= rows;
        rows = 1;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            double tmp = round(coeff1*pow(data_in[j],coeff2));
            data_out[j] = (uchar)MIN(tmp, (double)255);
        }
    }
    return true;
}

bool equalization(Mat* src, Mat* dst)
{
    if(src->channels()!=1)return false;
    int* hist = new int[256];
    for(int i = 0; i < 256; ++i){
        hist[i] = 0;
    }
    int rows = src->rows, cols = src->cols, size = rows * cols;
    getHistogram(src, hist);
    std::vector<double> p;
    p.push_back(hist[0]/(double)size);
    for(int i = 1; i < 256; ++i){
        double tmp = hist[i]/((double)size);
        p.push_back(p[i-1]+tmp);
    }
    if(src->isContinuous()){
        rows = 1;
        cols = size;
    }
    for(int i = 0; i < rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < cols; ++j){
            int idx = data_in[j];
            double res = round(255*p[idx]);
            data_out[j] = (uchar)MIN(res,(double)255);
        }
    }
    return true;
}


//smoothing filter
bool saltNoise(Mat* src, int n)
{
    int flag = src->channels();
    if(flag != 1 && flag != 3)return false;
    for(int k = 0; k < n; ++k){
        int i = rand()%src->rows;
        int j = rand()%src->cols;
        if(flag==1){
            src->at<uchar>(i,j) = 255;
        }else{
            src->at<Vec3b>(i,j)[0] = 255;
            src->at<Vec3b>(i,j)[1] = 255;
            src->at<Vec3b>(i,j)[2] = 255;
        }
    }
    return true;
}

bool pepperNoise(Mat* src, int n)
{
    int flag = src->channels();
    if(flag != 1 && flag != 3)return false;
    for(int k = 0; k < n; ++k){
        int i = rand()%src->rows;
        int j = rand()%src->cols;
        if(flag==1){
            src->at<uchar>(i,j) = 0;
        }else{
            src->at<Vec3b>(i,j)[0] = 0;
            src->at<Vec3b>(i,j)[1] = 0;
            src->at<Vec3b>(i,j)[2] = 0;
        }
    }
    return true;
}

bool meanFilter(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int krows = kernel->rows, kcols = kernel->cols;
    int ksum = 0;
    for(int i= 0; i < krows; ++i){
        for(int j = 0; j < kcols; ++j){
            ksum += kernel->ptr<char>(i)[j];
        }
    }
    std::vector<uchar*> pix(krows);
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            int sum = 0;
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    sum += pix[m][j+n]*kernel->ptr<char>(m)[n];
                }
            }
            if(ksum!=0) sum = round(sum / ksum);
            dst->ptr(i+anchory)[j+anchorx] = (uchar)MIN(sum,255);
        }
    }
    return true;
}

bool meanFilter(Mat *src, Mat *dst, int rows, int cols, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int krows = rows, kcols = cols, ksize = krows*kcols;
    std::vector<uchar*> pix(krows);
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            int sum = 0;
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    sum += pix[m][j+n];
                }
            }
            double tmp = sum/(double)ksize;
            dst->ptr(i+anchory)[j+anchorx] = (uchar)MIN(round(tmp),255);
        }
    }
    return true;
}

bool medianFilter(Mat* src, Mat* dst, int rows, int cols, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int krows = rows, kcols = cols, ksize = krows*kcols;
    std::vector<uchar*> pix(krows);
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            std::vector<uchar> vec;
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    vec.push_back(pix[m][j+n]);
                }
            }
            std::sort(vec.begin(), vec.end());
            dst->ptr(i+anchory)[j+anchorx] = vec[ksize/2];
        }
    }
    return true;
}

bool calGaussianFilter(Mat* kernel, double variance){
    if(kernel->channels() != 1)return false;
    double coeff0 = 0.5 / pow(variance, 2), coeff1 = coeff0 / PI;
    int rows = kernel->rows, cols = kernel->cols;
    int centery = rows / 2, centerx = cols / 2;
    for(int i = 0; i < rows; ++i){
        double* data = kernel->ptr<double>(i);
        for(int j = 0; j < cols; ++j){
            int dx = j - centerx, dy = i - centery;
            double k = -(dx*dx+dy*dy)*coeff0;
            data[j] = coeff1*exp(k);
        }
    }
    return true;
}

bool GaussianFilter(Mat* src, Mat* dst, Mat* kernel,int anchorx, int anchory)
{
    if(src->channels()!=1 || kernel->channels() != 1)return false;
    double ksum = 0;
    int krows = kernel->rows, kcols = kernel->cols;
    for(int i = 0; i < krows; ++i){
        double* data = kernel->ptr<double>(i);
        for(int j = 0; j < kcols; ++j){
            ksum += data[j];
        }
    }
    src->copyTo(*dst);
    std::vector<uchar*> pix(krows);
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            double sum = 0;
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    sum += pix[m][j+n] * kernel->ptr<double>(m)[n];
                }
            }
            dst->ptr(i+anchory)[j+anchorx] = (uchar)MIN(round(sum/ksum),255);
        }
    }
    return true;
}

//edge detection
bool convolution(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    int krows = kernel->rows, kcols = kernel->cols;
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    std::vector<uchar*> pix(krows);
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            int sum = 0;
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    sum += pix[m][j+n]*((int)kernel->ptr<char>(m)[n]);
                }
            }
            sum = abs(sum);
            dst->ptr(i+anchory)[j+anchorx] = (uchar)MIN(sum,255);
        }
    }
    return true;
}

bool SobelDetect(Mat* src, Mat* dst, int type)
{
    if(src->channels() != 1)return false;
    Mat kernel(3,3,CV_8SC1);
    switch (type) {
    case 0:
        kernel = (Mat_<char>(3,3)<<-1,-2,-1,0,0,0,1,2,1);
        break;
    case 1:
        kernel = (Mat_<char>(3,3)<<-1,0,1,-2,0,2,-1,0,1);
        break;
    case 2:
        kernel = (Mat_<char>(3,3)<<0,1,2,-1,0,1,-2,-1,0);
        break;
    case 3:
        kernel = (Mat_<char>(3,3)<<-2,-1,0,-1,0,1,0,1,2);
        break;
    default: return false;
    }
    src->copyTo(*dst);
    return convolution(src, dst, &kernel, 1, 1);
}

bool LaplacianDetect(Mat* src, Mat* dst)
{
    if(src->channels() != 1)return false;
    Mat kernel(3,3,CV_8SC1);
    kernel = (Mat_<char>(3,3)<<0,-1,0,-1,4,-1,0,-1,0);
    src->copyTo(*dst);

    return convolution(src, dst, &kernel, 1, 1);
}

//void trace(Mat* src, int low){
//    for(int i = 1; i < src->rows-1; ++i){
//        for(int j = 1; j < src->cols-1; ++j){
//            if(src->ptr(i)[j] > low){
//                if(src->ptr(i-1)[j-1]==255 || src->ptr(i-1)[j]==255 || src->ptr(i-1)[j+1]==255 ||
//                   src->ptr(i)[j-1]==255 || src->ptr(i)[j+1]==255 || src->ptr(i+1)[j-1]==255 ||
//                   src->ptr(i+1)[j]==255 || src->ptr(i+1)[j+1]==255)
//                {
//                    src->ptr(i)[j] = 255;
//                   trace(src, low);
//                }else{
//                    src->ptr(i)[j] = 0;
//                }
//            }
//        }
//    }
//}

const int xdir[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
const int ydir[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };

void trace(Mat* tmp, Mat* dst, int x, int y, int low, bool** flag) {
    int xx, yy;
    for (int i = 0; i < 8; i++) {
        xx = x + xdir[i];
        yy = y + ydir[i];
        if (flag[yy][xx] && tmp->ptr<double>(yy)[xx] >= low
            && dst->ptr(yy)[xx] == 0) {
            dst->ptr(yy)[xx] = 255;
            trace(tmp, dst, xx, yy, low, flag);
        }
    }
}

bool CannyDetect(Mat* src, Mat* dst, int low, int high, int size)
{
    if(src->channels() != 1)return false;
    Mat kernel(size, size, CV_64FC1);
    calGaussianFilter(&kernel, 0.5);
    int rows = src->rows, cols = src->cols;
    GaussianFilter(src, dst, &kernel, size/2, size/2);

    //calculate gratitude
    double** direction = new double*[(rows-2)];
    for(int i = 0; i < rows-2; ++i){
        direction[i] = new double[cols-2];
        for(int j = 0; j < cols - 2; ++j)
            direction[i][j] = 0;
    }

    std::vector<uchar*> pix(3);
    Mat tmp(rows, cols, CV_64FC1);
    for(int i = 0; i < rows-2; ++i){
        for(int k = 0; k < 3; ++k) pix[k] = dst->ptr(i+k);
        for(int j = 0; j < cols-2; ++j){
            double gradY = (pix[0][j+2]+2*pix[1][j+2]+pix[2][j+2])-(pix[0][j]+2*pix[1][j]+pix[2][j]);
            double gradX = (pix[2][j]+2*pix[2][j+1]+pix[2][j+2])-(pix[0][j]+2*pix[0][j+1]+pix[0][j+2]);
            tmp.ptr<double>(i+1)[j+1] = sqrt(gradX*gradX+gradY*gradY);
            if(gradX==0) gradX=0.00000000000000001;
            direction[i][j]=atan(gradY/gradX)*57.2957;
        }
    }

    //non-maxium supression
    bool** flag = new bool*[rows];
    for(int i = 0; i < rows; ++i){
        flag[i] = new bool[cols];
        for(int j = 0; j < cols; ++j)
            flag[i][j] = false;
    }
    double h1 = 0.0, h2 = 0.0;
    for(int i = 0; i < rows-2; ++i){
        for(int j = 0; j < rows-2; ++j){
            double theta = direction[i][j];
            if((theta > -91 && theta <= -67.5) || (theta > 67.5 && theta < 91)){
                h1 = tmp.ptr<double>(i)[j+1];
                h2 = tmp.ptr<double>(i+2)[j+1];
            }else if(theta > -67.5 && theta <= -22.5){
                h1 = tmp.ptr<double>(i)[j+2];
                h2 = tmp.ptr<double>(i+2)[j];
            }else if(theta > -22.5  && theta <= 22.5){
                h1 = tmp.ptr<double>(i+1)[j];
                h2 = tmp.ptr<double>(i+1)[j+2];
            }else if(theta > 22.5 && theta <= 67.5){
                h1 = tmp.ptr<double>(i)[j];
                h2 = tmp.ptr<double>(i+2)[j+2];
            }else continue;
            if(tmp.ptr<double>(i+1)[j+1] < h1 || tmp.ptr<double>(i+1)[j+1] < h2){
                tmp.ptr<double>(i+1)[j+1] = 0;
            }else{
                flag[i+1][j+1] = true;
            }
        }
    }

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j)
            dst->ptr(i)[j] = 0;
    }

    //double threshold detecting
    for (int i = 0; i < rows; i++) {
        double* data_in = tmp.ptr<double>(i);
        uchar* data_out = dst->ptr(i);
        for (int j = 0; j < cols; j++) {
            if(flag[i][j] && data_in[j] >= high && data_out[j]==0){
                data_out[j] = 255;
                trace(&tmp, dst, j, i, low, flag);
            }
        }
    }

    for(int i = 0; i < cols-2; ++i)
        delete []direction[i];
    delete []direction;
    for(int i = 0; i < cols; ++i)
        delete []flag[i];
    delete []flag;
    return true;
}


//Hough transformation
struct line{
    int theta;
    int r;
};

void HoughLine(Mat* src, Mat* dst, int threshold)
{
    double sin_value[360];
    double cos_value[360];
    for(int i=0; i < 360; i++){
        sin_value[i] = sin(i*PI/180);
        cos_value[i] = cos(i*PI/180);
    }
    src->copyTo(*dst);
    blur(*dst, *dst, Size(3,3) );
    Canny(*dst, *dst, 50, 200);
    std::vector<struct line> lines;
    int diagonal = floor(sqrt(src->rows*src->rows + src->cols*src->cols));
    std::vector<std::vector<int>>p(360,std::vector<int>(diagonal));
    for(int i = 0; i < 360; ++i){
        for(int j = 0; j < diagonal; ++j)
            p[i][j] = 0;
    }
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            if(dst->ptr(i)[j] > 0){
                for(int a = 0; a < 360; ++a){
                    int r = round(j*sin_value[a] + i*cos_value[a]);
                    if(r < 0) continue;
                    ++p[a][r];
                }
            }
        }
    }
    for( int theta = 0; theta < 360; ++theta){
        for( int r = 0; r < diagonal; ++r){
            int thetaLeft = MAX(0,theta-1);
            int thetaRight = MIN(359,theta+1);
            int rLeft = MAX(0,r-1);
            int rRight = MIN(diagonal-1,r+1);
            int tmp = p[theta][r];
            if( tmp > threshold
                && tmp > p[thetaLeft][rLeft] && tmp > p[thetaLeft][r] && tmp > p[thetaLeft][rRight]
                && tmp > p[theta][rLeft] && tmp > p[theta][rRight]
                && tmp > p[thetaRight][rLeft] && tmp > p[thetaRight][r] && tmp > p[thetaRight][rRight])
            {
                struct line newline;
                newline.theta = theta;
                newline.r = r;
                lines.push_back(newline);
            }
        }
    }
    src->copyTo(*dst);
    for(int i = 0;i < lines.size();i++) {
        std::vector<Point> points;
        int theta = lines[i].theta;
        int r = lines[i].r;

        double ct = cos_value[theta];
        double st = sin_value[theta];

        int y = int(r/st);
        if(y >= 0 && y < src->rows){
            Point point(0, y);
            points.push_back(point);
        }
        y = int((r-ct*(src->cols-1))/st);
        if(y >= 0 && y < src->rows){
            Point point(src->cols-1, y);
            points.push_back(point);
        }
        int x = int(r/ct);
        if(x >= 0 && x < src->cols){
            Point point(x, 0);
            points.push_back(point);
        }
        x = int((r-st*(src->rows-1))/ct);
        if(x >= 0 && x < src->cols){
            Point point(x, src->rows-1);
            points.push_back(point);
        }
        if(points.size()<2)continue;
        cv::line(*dst, points[0], points[1], Scalar(255,255,255), 1, CV_AA);
    }
}


//binary morphologic transformation
void reflectKernel(Mat* kernel)
{
    if(kernel->channels()!=1)return;
    int rows = kernel->rows, cols = kernel->cols;
    Mat tmp(rows, cols, CV_8SC1);
    for(int i = 0; i < rows; ++i){
        char* data_in = kernel->ptr<char>(rows-1 - i);
        char* data_out = tmp.ptr<char>(i);
        for(int j = 0; j < cols; ++j){
            data_out[j] = data_in[cols-1 - j];
        }
    }
    tmp.copyTo(*kernel);
}

bool dilation(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int krows = kernel->rows, kcols = kernel->cols;
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    std::vector<uchar*> pix(krows);
    reflectKernel(kernel);
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    if(kernel->ptr<char>(m)[n]>=1 && pix[m][j+n]==255){
                        dst->ptr(i+anchory)[j+anchorx] = 255;
                        goto nextloop;
                    }
                }
            }
            dst->ptr(i+anchory)[j+anchorx] = 0;
            nextloop:;
        }
    }
    return true;
}

bool erosion(Mat *src, Mat *dst, Mat *kernel, int anchorx, int anchory)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int krows = kernel->rows, kcols = kernel->cols;
    int rangex = src->cols+1-kcols, rangey = src->rows+1-krows;
    std::vector<uchar*> pix(krows);
    for(int i = 0; i < rangey; ++i){
        for(int k = 0; k < krows; ++k) pix[k] = src->ptr(i+k);
        for(int j = 0; j < rangex; ++j){
            for(int m = 0; m < krows; ++m){
                for(int n = 0; n < kcols; ++n){
                    if(kernel->ptr<char>(m)[n]>=1 && pix[m][j+n]!=255){
                        dst->ptr(i+anchory)[j+anchorx] = 0;
                        goto nextloop;
                    }
                }
            }
            dst->ptr(i+anchory)[j+anchorx] = 255;
            nextloop:;
        }
    }
    return true;
}

void matAnd(Mat* src1, Mat* src2,Mat* dst)
{
    for(int i = 0; i < src1->rows; ++i){
        uchar* data_in1 = src1->ptr(i);
        uchar* data_in2 = src2->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < src1->cols; ++j){
            data_out[j] = data_in1[j] & data_in2[j];
        }
    }
}

void matOr(Mat* src1, Mat* src2,Mat* dst)
{
    for(int i = 0; i < src1->rows; ++i){
        uchar* data_in1 = src1->ptr(i);
        uchar* data_in2 = src2->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < src1->cols; ++j){
            data_out[j] = data_in1[j] | data_in2[j];
        }
    }
}

void matOr(Mat* src, Mat* dst)
{
    for(int i = 0; i < src->rows; ++i){
        uchar* data_in = src->ptr(i);
        uchar* data_out = dst->ptr(i);
        for(int j = 0; j < src->cols; ++j){
            data_out[j] = data_in[j] | data_out[j];
        }
    }
}


void matNot(Mat* src)
{
    for(int i = 0; i < src->rows; ++i){
        uchar* data = src->ptr(i);
        for(int j = 0; j < src->cols; ++j){
            data[j] = 255 - data[j];
        }
    }
}

void matNot(Mat *src, Mat *dst)
{
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            dst->ptr(i)[j] = 255 - src->ptr(i)[j];
        }
    }
}

void hitMiss(Mat* src, Mat* dst, Mat* k1, Mat* k2, int anchorx, int anchory)
{
    Mat con(src->rows, src->cols, CV_8UC1);
//    src->copyTo(*dst);
    matNot(src, &con);
    Mat tmp1(src->rows, src->cols, CV_8UC1);
    Mat tmp2(src->rows, src->cols, CV_8UC1);
    erosion(src, &tmp1, k1, anchorx, anchory);
    erosion(&con, &tmp2, k2, anchorx, anchory);
    matAnd(&tmp1, &tmp2, dst);
}

void getThinningSE(int num, Mat* se){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            se->ptr(i)[j] = 0;
        }
    }
    switch(num){
    case 0:
        se->ptr(2)[0] = 1;
        se->ptr(2)[1] = 1;
        se->ptr(2)[2] = 1;
        se->ptr(1)[1] = 1;
        break;
    case 1:
        se->ptr(1)[1] = 1;
        se->ptr(1)[0] = 1;
        se->ptr(2)[0] = 1;
        se->ptr(2)[1] = 1;
        break;
    case 2:
        se->ptr(1)[1] = 1;
        se->ptr(1)[0] = 1;
        se->ptr(2)[0] = 1;
        se->ptr(0)[0] = 1;
        break;
    case 3:
        se->ptr(1)[1] = 1;
        se->ptr(0)[0] = 1;
        se->ptr(0)[1] = 1;
        se->ptr(1)[0] = 1;
        break;
    case 4:
        se->ptr(1)[1] = 1;
        se->ptr(0)[0] = 1;
        se->ptr(0)[1] = 1;
        se->ptr(0)[2] = 1;
        break;
    case 5:
        se->ptr(0)[1] = 1;
        se->ptr(0)[2] = 1;
        se->ptr(1)[1] = 1;
        se->ptr(1)[2] = 1;
        break;
    case 6:
        se->ptr(1)[1] = 1;
        se->ptr(0)[2] = 1;
        se->ptr(1)[2] = 1;
        se->ptr(2)[2] = 1;
        break;
    case 7:
        se->ptr(1)[1] = 1;
        se->ptr(1)[2] = 1;
        se->ptr(2)[1] = 1;
        se->ptr(2)[2] = 1;
        break;
    }
}

void getThinningConSE(int num, Mat* se){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            se->ptr(i)[j] = 0;
        }
    }
    switch(num){
    case 0:
        se->ptr(0)[1] = 1;
        se->ptr(0)[2] = 1;
        se->ptr(0)[0] = 1;
        break;
    case 1:
        se->ptr(0)[1] = 1;
        se->ptr(0)[2] = 1;
        se->ptr(1)[2] = 1;
        break;
    case 2:
        se->ptr(0)[2] = 1;
        se->ptr(1)[2] = 1;
        se->ptr(2)[2] = 1;
        break;
    case 3:
        se->ptr(1)[2] = 1;
        se->ptr(2)[1] = 1;
        se->ptr(2)[2] = 1;
        break;
    case 4:
        se->ptr(2)[0] = 1;
        se->ptr(2)[1] = 1;
        se->ptr(2)[2] = 1;
        break;
    case 5:
        se->ptr(1)[0] = 1;
        se->ptr(2)[0] = 1;
        se->ptr(2)[1] = 1;
        break;
    case 6:
        se->ptr(0)[0] = 1;
        se->ptr(1)[0] = 1;
        se->ptr(2)[0] = 1;
        break;
    case 7:
        se->ptr(0)[0] = 1;
        se->ptr(0)[1] = 1;
        se->ptr(1)[0] = 1;
        break;
    }
}

bool isEqual(Mat* src, Mat* dst)
{
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            if(src->ptr(i)[j]!=dst->ptr(i)[j]){
                return false;
            }
        }
    }
    return true;
}

int thintimes = 0;

bool thinning(Mat* src, Mat* dst)
{
    if(src->channels() != 1)return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    Mat tmp1(src->rows, src->cols, CV_8UC1);
    Mat tmp2(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    thintimes = 0;
    int i = 0;
//    while(!isEqual(&tmp, &tmp2)){
    while(i<50){
        tmp.copyTo(tmp2);
        for(int i = 0; i < 8; ++i){
            tmp.copyTo(tmp1);
            Mat* k1 = new Mat(3,3, CV_8UC1);
            Mat* k2 = new Mat(3,3, CV_8UC1);
            getThinningSE(i, k1);
            getThinningConSE(i, k2);
            hitMiss(&tmp1, &tmp, k1, k2, 1, 1);
            matNot(&tmp);
            matAnd(&tmp1, &tmp, &tmp);
            delete k1, k2;
//            diffMatC1(&tmp, &tmp1);
        }
        ++i;
    }
    thintimes = i;
    tmp.copyTo(*dst);
    return true;
}

bool thickening(Mat* src, Mat* dst)
{
    if(src->channels() != 1)return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    Mat tmp1(src->rows, src->cols, CV_8UC1);
//    Mat tmp2(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    while(thintimes > 0){
        for(int i = 0; i < 8; ++i){
            tmp.copyTo(tmp1);
            Mat* k1 = new Mat(3,3, CV_8UC1);
            Mat* k2 = new Mat(3,3, CV_8UC1);
            getThinningSE(i, k1);
            getThinningConSE(i, k2);
            hitMiss(&tmp1, &tmp, k1, k2, 1, 1);
            matOr(&tmp1, &tmp);
            delete k1, k2;
        }
        --thintimes;
    }
    tmp.copyTo(*dst);
    return true;
}

std::vector<Mat> skeset;

bool skeleton(Mat *src, Mat *dst, Mat *kernel, int anchorx, int anchory)
{
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            dst->ptr(i)[j] = 0;
        }
    }
    skeset.clear();
    Mat tmp(src->rows, src->cols, CV_8UC1);
    Mat tmp1(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp1);
    int i  = 0;
    do{
        Mat tmp2(src->rows, src->cols, CV_8UC1);
        erosion(&tmp1, &tmp2, kernel, anchorx, anchory);
        tmp2.copyTo(tmp1);
        erosion(&tmp1, &tmp2, kernel, anchorx, anchory);
        dilation(&tmp2, &tmp, kernel, anchorx, anchory);
        matNot(&tmp);
        matAnd(&tmp1, &tmp, &tmp2);
        skeset.push_back(tmp2);
        matOr(&tmp2, dst);
        ++i;
    }while(countNonZero(skeset.back())>0);
    return true;
}

bool skeletonReconstruct(Mat* src, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(skeset.size()==0)return false;
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            dst->ptr(i)[j] = 0;
        }
    }
    Mat tmp(src->rows, src->cols, CV_8UC1);
    Mat tmp1(src->rows, src->cols, CV_8UC1);
    int i = skeset.size()-1;
    skeset[i].copyTo(tmp1);
    while(i > 0){
        dilation(&tmp1, &tmp, kernel, anchorx, anchory);
        --i;
        matOr(&tmp, &skeset[i], &tmp1);
    }
    dilation(&tmp1,dst,kernel, anchorx, anchory);
    skeset.clear();
    return true;
}


bool dilationRebuild(Mat* mark, Mat* ground, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(mark->channels()!= 1 || ground->channels()!=1) return false;
    Mat tmp1(mark->rows, mark->cols, CV_8UC1);
    mark->copyTo(tmp1);
    Mat tmp2(mark->rows, mark->cols, CV_8UC1);
    while (!isEqual(&tmp1, &tmp2)) {
        tmp1.copyTo(tmp2);
        dilation(&tmp1, &tmp1, kernel, anchorx, anchory);
        matAnd(&tmp1, ground, dst);
        dst->copyTo(tmp1);
    }
    tmp1.copyTo(*dst);
    return true;
}

bool erosionRebuild(Mat* mark, Mat* ground, Mat* dst, Mat* kernel, int anchorx, int anchory)
{
    if(mark->channels()!= 1 || ground->channels()!=1) return false;
    Mat tmp1(mark->rows, mark->cols, CV_8UC1);
    mark->copyTo(tmp1);
    Mat tmp2(mark->rows, mark->cols, CV_8UC1);
    while (!isEqual(&tmp1, &tmp2)) {
        tmp1.copyTo(tmp2);
        erosion(&tmp1, &tmp1, kernel, anchorx, anchory);
        matOr(&tmp1, ground, dst);
        dst->copyTo(tmp1);
    }
    tmp1.copyTo(*dst);
    return true;
}

bool openRebuild(Mat *src, Mat *dst, Mat *kernel, int n, int anchorx, int anchory)
{
    if(src->channels()!= 1) return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    for(int i = 0; i < n; ++i){
        erosion(&tmp, &tmp, kernel, anchorx, anchory);
    }
    return dilationRebuild(&tmp, src, dst, kernel, anchorx, anchory);
}

bool closeRebuild(Mat *src, Mat *dst, Mat *kernel, int n, int anchorx, int anchory)
{
    if(src->channels()!= 1) return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    for(int i = 0; i < n; ++i){
        dilation(&tmp, &tmp, kernel, anchorx, anchory);
    }
    return erosionRebuild(&tmp, src, dst, kernel, anchorx, anchory);
}

//gray-level morphologic transformation
bool grayDilation(Mat* src, Mat* dst)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int rangex = src->cols-1, rangey = src->rows-1;
    for(int i = 1; i < rangey; ++i){
        for(int j = 1; j < rangex; ++j){
            uchar max = 0;
            for(int m = -1; m < 2; ++m){
                for(int n = -1; n < 2; ++n){
                    max = MAX(max, src->ptr(i+m)[j+n]);
                }
            }
            dst->ptr(i)[j] = max;
        }
    }
    return true;
}

bool grayErosion(Mat* src, Mat* dst)
{
    if(src->channels() != 1)return false;
    src->copyTo(*dst);
    int rangex = src->cols-1, rangey = src->rows-1;
    for(int i = 1; i < rangey; ++i){
        for(int j = 1; j < rangex; ++j){
            uchar min = 255;
            for(int m = -1; m < 2; ++m){
                for(int n = -1; n < 2; ++n){
                    min = MIN(min, src->ptr(i+m)[j+n]);
                }
            }
            dst->ptr(i)[j] = min;
        }
    }
    return true;
}

void matMin(Mat* src, Mat* dst){
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            dst->ptr(i)[j] = MIN(src->ptr(i)[j], dst->ptr(i)[j]);
        }
    }
}

void matMax(Mat* src, Mat* dst){
    for(int i = 0; i < src->rows; ++i){
        for(int j = 0; j < src->cols; ++j){
            dst->ptr(i)[j] = MAX(src->ptr(i)[j], dst->ptr(i)[j]);
        }
    }
}

bool grayDilationRebuild(Mat* mark, Mat* ground, Mat* dst)
{
    if(mark->channels()!= 1 || ground->channels()!=1) return false;
    Mat tmp1(mark->rows, mark->cols, CV_8UC1);
    mark->copyTo(tmp1);
    Mat tmp2(mark->rows, mark->cols, CV_8UC1);
    while (!isEqual(&tmp1, &tmp2)) {
        tmp1.copyTo(tmp2);
        grayDilation(&tmp1, &tmp1);
        matMin(ground, &tmp1);
    }
    tmp1.copyTo(*dst);
    return true;
}

bool grayErosionRebuild(Mat* mark, Mat* ground, Mat* dst)
{
    if(mark->channels()!= 1 || ground->channels()!=1) return false;
    Mat tmp1(mark->rows, mark->cols, CV_8UC1);
    mark->copyTo(tmp1);
    Mat tmp2(mark->rows, mark->cols, CV_8UC1);
    while (!isEqual(&tmp1, &tmp2)) {
        tmp1.copyTo(tmp2);
        grayErosion(&tmp1, &tmp1);
        matMax(ground, &tmp1);
    }
    tmp1.copyTo(*dst);
    return true;
}

bool grayOpenRebuild(Mat *src, Mat *dst, int n)
{
    if(src->channels()!= 1) return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    for(int i = 0; i < n; ++i){
        grayErosion(&tmp, &tmp);
    }
    return grayDilationRebuild(&tmp, src, dst);
}

bool grayCloseionRebuild(Mat *src, Mat *dst, int n)
{
    if(src->channels()!= 1) return false;
    Mat tmp(src->rows, src->cols, CV_8UC1);
    src->copyTo(tmp);
    for(int i = 0; i < n; ++i){
        grayDilation(&tmp, &tmp);
    }
    return grayErosionRebuild(&tmp, src, dst);
}

bool waterShed(Mat* src, Mat* dst)
{
    return true;
}



