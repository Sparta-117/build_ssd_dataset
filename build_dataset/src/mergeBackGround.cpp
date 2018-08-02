#include "mergeBackGround.h"

using namespace cv;
using namespace std;

namespace build_dataset
{
mergeBackGround::mergeBackGround()
{}

void mergeBackGround::getPic(Mat rgb, Mat gt, Mat roi_rgb, Mat roi_GT, int counter)
{
    frontImg = rgb;
    frontGT = gt;
    roiRgb = roi_rgb;
    roiGT = roi_GT;
    scaleFrontImg = Mat::zeros(frontImg.size(),CV_8UC3);
    backGroundImg = Mat::zeros(frontImg.size(),CV_8UC3);
    scaleGT = Mat::zeros(frontImg.size(),CV_8UC1);
    //frontGT = Mat::zeros(frontImg.size(),CV_8UC1);
    scale = (rand()%50)/100.0+0.5;
    index = counter;

    scalePic();
}

void mergeBackGround::scalePic()
{
    char bg_file_name[7]="000000";
    int bg_num = 811;
    int bg_index = rand()%bg_num+1;

    for(int j=5;j>=0;j--)
    {
      if(bg_index>0)
      {
      bg_file_name[j] ='0' + bg_index%10;
      bg_index/=10;
      }
      else bg_file_name[j] = '0';
    }

    stringstream ss3;
    string i3;
    ss3<<bg_file_name;
    ss3>>i3;
    string background_title = "/home/mzm/build_dataset_pic/src/build_dataset/background_dataset/"+i3+".jpg";
    Mat scale_roiRgb;
    Mat scale_roiGT;
    Size scale_size = Size(roiRgb.cols*scale,roiRgb.rows*scale);
    resize(roiRgb,scale_roiRgb,scale_size);
    resize(roiGT,scale_roiGT,scale_size);

    double angle = rand()%360;
    cv::Point2f center(scale_roiRgb.cols / 2, scale_roiRgb.rows / 2);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
    cv::Rect bbox = cv::RotatedRect(center, scale_roiRgb.size(), angle).boundingRect();

    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

    cv::Mat rotated_scale_roiRgb;
    cv::Mat rotated_scale_roiGT;
    cv::warpAffine(scale_roiRgb, rotated_scale_roiRgb, rot, bbox.size());
    cv::warpAffine(scale_roiGT, rotated_scale_roiGT, rot, bbox.size());

    cv::Rect roi_rect = cv::Rect(rand()%(scaleFrontImg.cols-rotated_scale_roiRgb.cols), rand()%(scaleFrontImg.rows-rotated_scale_roiRgb.rows), rotated_scale_roiRgb.cols, rotated_scale_roiRgb.rows);
    rotated_scale_roiRgb.copyTo(scaleFrontImg(roi_rect));
    rotated_scale_roiGT.copyTo(scaleGT(roi_rect));
    backGroundImg = imread(background_title);

    for(int x=0;x<scaleGT.cols;x++)
    {
        for(int y=0;y<scaleGT.rows;y++)
        {
            if(scaleGT.at<uchar>(y,x) < 20)
            {
                if((x<backGroundImg.cols)&&(y<backGroundImg.rows))
                    scaleFrontImg.at<Vec3b>(y,x) = backGroundImg.at<Vec3b>(y,x);
                else
                    scaleFrontImg.at<Vec3b>(y,x) = backGroundImg.at<Vec3b>(y%(backGroundImg.rows),x%(backGroundImg.cols));
            }
        }
    }

    xmin = roi_rect.x;
    ymin = roi_rect.y;
    xmax = roi_rect.x + roi_rect.width;
    ymax = roi_rect.y + roi_rect.height;

    imshow("merge result",scaleFrontImg);
    waitKey(1);
}

void mergeBackGround::savePic(string dataset_ads)
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

//    string title1 = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/object_rgb_dataset/" + i2 +".jpg";
//    string title2 = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/object_groundtruth_dataset/" + i2 +".jpg";

    string rgb_ads = dataset_ads + "/object_rgb_dataset/" + i2 +".jpg";
    string gt_ads = dataset_ads + "/object_groundtruth_dataset/" + i2 +".jpg";

    imwrite( rgb_ads, scaleFrontImg);
    imwrite( gt_ads , scaleGT);
    cout<<"save No: "<<new_file_name<<" data."<<endl;
}

}
