#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;


image mat_to_image(Mat src)
{
    int h = src.rows;
    int w = src.cols;
    int c = src.channels();
    image im = make_image(w, h, c);
    int step = src.step[0];
    (void)step;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k = 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                float val = src.at<float>(i, j*c + k);
                im.data[k*w*h + i*w + j] = val/255.;
            }
        }
    }
    rgbgr_image(im);
    return im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    if(im.c == 3) rgbgr_image(copy);

    int w = copy.w;
    int h = copy.h;
    int c = copy.c;

    Mat m(h, w, CV_32FC(c));
    for(int y = 0; y < h; ++y){
        for(int x = 0; x < w; ++x){
            for(int k = 0; k < c; ++k){
                float val = copy.data[k*h*w + y*w + x];
                m.at<float>(y, x*c + k) = val;
            }
        }
    }
    free_image(copy);
    return m;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    if(m.empty()) return make_empty_image(0,0,0);
    return mat_to_image(m);
}

image load_image_cv(char *filename, int channels)
{
    int flag = -1;
    if (channels == 0) flag = -1;
    else if (channels == 1) flag = cv::IMREAD_GRAYSCALE;
    else if (channels == 3) flag = cv::IMREAD_COLOR;
    else {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m = imread(filename, flag);
    if(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        return make_image(10,10,3);
    }
    image im = mat_to_image(m);
    return im;
}

int show_image_cv(image im, const char* name, int ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    int c = waitKey(ms);
    if (c != -1) c = c%256;
    return c;
}

void make_window(char *name, int w, int h, int fullscreen)
{
    namedWindow(name, WINDOW_NORMAL);
    if (fullscreen) {
        setWindowProperty(name, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    } else {
        resizeWindow(name, w, h);
        if(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

#endif