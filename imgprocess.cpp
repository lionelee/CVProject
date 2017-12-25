#include "imgprocess.h"
#include <stdio.h>
#include <strstream>
#include <QMessageBox>

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
            double R = data_in[j] * 1.0 / 255;
            double G = data_in[j+1] * 1.0 / 255;
            double B = data_in[j+2] * 1.0 / 255;
            double min = MIN(MIN(R,G),B);
            double max = MAX(MAX(R,G),B);

//            double delta = vmax - vmin;

//            double dr, dg, db;

//            *l = (vmax+vmin) / 2;

//            if (0 == delta)
//            {
//            *h = 0;
//            *s = 0;
//            }
//            else
//            {
//            if (*l < 0.5)
//            *s = delta / (vmax+vmin);
//            else
//            *s = delta / (2-vmax-vmin);

//            dr = (((vmax-var_R)/6) + (delta/2))/delta;
//            dg = (((vmax-var_G)/6) + (delta/2))/delta;
//            db = (((vmax-var_B)/6) + (delta/2))/delta;

//            if (var_R == vmax)
//            *h = db - dg;
//            else if (var_G == vmax)
//            *h = (1.0/3.0) + dr - db;
//            else if (var_B == vmax)
//            *h = (2.0/3.0) + dg - dr;

//            if (*h < 0)
//            *h += 1;
//            if (*h > 1)
//            *h -= 1;
//            }

            double H = 0.0;
            if (max == min){
                H = 0.0;
            }else if (max == R && G > B){
                H = 60.0*(G-B)/(max-min);
            }else if (max == R && G < B){
                H = 60.0*(G-B)/(max-min)+360.0;
            }else if (max == G){
                H = 60.0*(B-R)/(max-min)+120.0;
            }else if (max == B){
                H = 60.0*(R-G)/(max-min)+240.0;
            }
            if(H > 360.0) H = 360.0;
            else if(H < 0) H = 0.0;
            H = H / 360.0;

            double L = (max + min)/2;
            double S = 0;
            if (L == 0.0 || max == min){
                S = 0;
            }else if (L > 0.0 && L < 0.5){
                S = (max-min)/(max+min);
            }else if (L > 0.5){
                S = (max-min)/(2-max-min);
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
