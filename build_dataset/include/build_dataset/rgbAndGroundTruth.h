#ifndef RGBANDGROUNDTRUTH_H
#define RGBANDGROUNDTRUTH_H

#include "common_include.h"

using namespace cv;
using namespace std;

namespace build_dataset
{
class rgbAndGroundTruth
{
public:
    rgbAndGroundTruth();
    void updatePic(Mat rgb,int counter);
    void adjustHsvValue();
    void setHSVValue(int lowH,int highH,int lowS,int highS,int lowV,int highV);
    Mat getResultRgb();
    Mat getResultGT();
    Mat getROIRgb();
    Mat getROIGT();
    Rect getBoundingBox();

protected:
    void FindGroundTruth();
    void saveRGBAndGT();
    void showRGBAndGT();

private:
    Mat sourceRgb;
    Mat hsv;
    Mat resultRgb;
    Mat resultGT;
    Mat roiRgb;
    Mat roiGT;
    Rect boundingBox;
    int index;

    //hsv value
    int iLowH;
    int iHighH;

    int iLowS;
    int iHighS;

    int iLowV;
    int iHighV;
};
}

#endif //RGBANDGROUNDTRUTH_H
