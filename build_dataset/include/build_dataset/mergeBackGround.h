#ifndef MERGEBACKGROUND_H
#define MERGEBACKGROUND_H

#include "common_include.h"

using namespace cv;
using namespace std;

namespace build_dataset
{
class mergeBackGround
{
public:
    mergeBackGround();
    void getPic(Mat rgb,Mat gt,Mat roi_rgb,Mat roi_GT,int counter);
    void savePic(string dataset_ads);
    int getBoundingBox();
    int xmin;
    int xmax;
    int ymin;
    int ymax;

protected:
    void scalePic();

private:
    Mat frontImg;
    Mat frontGT;
    Mat roiRgb;
    Mat roiGT;
    Mat scaleFrontImg;
    Mat scaleGT;
    Mat backGroundImg;
    double scale;
    int index;

};
}

#endif //MERGEBACKGROUND_H
