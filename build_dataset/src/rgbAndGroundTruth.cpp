#include "rgbAndGroundTruth.h"

using namespace cv;
using namespace std;

namespace build_dataset
{
rgbAndGroundTruth::rgbAndGroundTruth()
{}

void rgbAndGroundTruth::setHSVValue(int lowH, int highH, int lowS, int highS, int lowV, int highV)
{
    //hsv value
    iLowH = lowH;
    iHighH = highH;

    iLowS = lowS;
    iHighS = highS;

    iLowV = lowV;
    iHighV = highV;
}

void rgbAndGroundTruth::updatePic(Mat rgb, int counter)
{
    sourceRgb = rgb;
    index = counter;
    hsv = Mat::zeros(sourceRgb.size(),CV_8UC3);
    resultRgb = Mat::zeros(sourceRgb.size(),CV_8UC3);
    resultGT = Mat::zeros(sourceRgb.size(),CV_8UC1);

    FindGroundTruth();
    //saveRGBAndGT();
    //showRGBAndGT();
}

void rgbAndGroundTruth::FindGroundTruth()
{
    Mat bin = Mat::zeros(resultGT.size(),CV_8UC1);
    Mat binROI = Mat::zeros(bin.size(),CV_8UC1);

    vector<vector<cv::Point> > contours ;

    cvtColor(sourceRgb, hsv, CV_BGR2HSV );

    inRange(hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV),bin); //Threshold the image
    cv::threshold (bin, bin, 70, 255, CV_THRESH_BINARY_INV);

    // 查找轮廓，对应连通域
    cv::findContours(bin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    // 寻找最大连通域
    double maxArea = 0;
    vector<cv::Point> maxContour;
    for(size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
        }
    }

    // 将轮廓转为矩形框
    boundingBox = cv::boundingRect(maxContour);

    binROI(boundingBox).setTo(255);
    bin.copyTo(resultGT,binROI);
    sourceRgb.copyTo(resultRgb,resultGT);
    roiRgb = resultRgb(boundingBox);
    roiGT = resultGT(boundingBox);
}

void rgbAndGroundTruth::saveRGBAndGT()
{
    int tmp_index = index;
    char new_file_name[7]="000000";
    for(int j=5;j>=0;j--)
    {
      if(tmp_index>0)
      {
      new_file_name[j] ='0' + tmp_index%10;
      tmp_index/=10;
      }
      else new_file_name[j] = '0';
    }
//      cout<<new_file_name<<endl;

    stringstream ss2;
    string i2;
    ss2<<new_file_name;
    ss2>>i2;

    string title1 = "/home/mzm/build_dataset_pic/src/build_dataset/object_rgb_dataset/" + i2 +".jpg";
    string title2 = "/home/mzm/build_dataset_pic/src/build_dataset/object_groundtruth_dataset/" + i2 +".jpg";

    imwrite( title1, resultRgb);
    imwrite( title2, resultGT);
    cout<<"save No: "<<new_file_name<<" data."<<endl;
}

void rgbAndGroundTruth::showRGBAndGT()
{
    imshow("resultRgb",resultRgb);
    imshow("resultGT",resultGT);
    waitKey(1);
}

void rgbAndGroundTruth::adjustHsvValue()
{
    Mat tmp_bin;
    //创建调参模块
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    inRange(hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV),tmp_bin); //Threshold the image

    cv::threshold (tmp_bin, tmp_bin, 70, 255, CV_THRESH_BINARY_INV);
    cv::imshow("bin",tmp_bin);
    waitKey(1);

}

Mat rgbAndGroundTruth::getResultRgb()
{
    return resultRgb;
}

Mat rgbAndGroundTruth::getResultGT()
{
    return resultGT;
}

Mat rgbAndGroundTruth::getROIRgb()
{
    return roiRgb;
}

Mat rgbAndGroundTruth::getROIGT()
{
    return roiGT;
}

Rect rgbAndGroundTruth::getBoundingBox()
{
    return boundingBox;
}
}
