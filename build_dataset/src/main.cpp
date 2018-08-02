#include "common_include.h"
#include "realsense_msgs/realsense_msgs.h"
#include "rgbAndGroundTruth.h"
#include "mergeBackGround.h"
#include "formXML.h"

using namespace cv;
using namespace std;
using namespace build_dataset;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "build_dataset_pic");
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<realsense_msgs::realsense_msgs>("get_image");
    realsense_msgs::realsense_msgs srv;
    srv.request.start = true;
    sensor_msgs::Image msg_rgb;
    sensor_msgs::Image msg_depth;
    ros::Rate loop_rate(200);

    int counter = 1;

    if (argc < 2)
    {
        cout<<"Not enough input arguments.Please enter object name and make sure the folder is builded!"<<endl;
        exit(1);
        return 0;
    }
    string object_name = (string)argv[1];
    stringstream ss;
    string i1;
    ss<<object_name;
    ss>>i1;
    string dataset_ads = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/" + i1;

    Mat rgbImage;
    rgbAndGroundTruth r1;
    r1.setHSVValue(53,143,90,255,48,219);//lH,hH,lS,hS,lV,hV //zhencui: 49,179,100,255,45,255
                                                             //taiping: 36,90,132,255,62,255
    mergeBackGround m1;

    cout<<"START BUILD DATASET!"<<endl;
    cout<<"'s' is stop. 'c' is continue. 'f' is stop all program."<<endl;
    while(ros::ok())
    {
        if (client.call(srv))
        {
            try
            {
                msg_rgb = srv.response.rgb_image;
                msg_depth = srv.response.depth_image;
                rgbImage = cv_bridge::toCvCopy(msg_rgb, sensor_msgs::image_encodings::TYPE_8UC3)->image;
                // depth_image = cv_bridge::toCvCopy(msg_depth, sensor_msgs::image_encodings::TYPE_32FC1)->image;
            }
            catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("cv_bridge exception: %s", e.what());
                return 1;
            }
        }
        if( !rgbImage.data )
        {
            printf( " No image data \n " );
            return -1;
        }
        r1.updatePic(rgbImage,counter);
        r1.adjustHsvValue();
        m1.getPic(r1.getResultRgb(),r1.getResultGT(),r1.getROIRgb(),r1.getROIGT(),counter);
        m1.savePic(dataset_ads);
        formXML x1(counter,rgbImage.cols,rgbImage.rows,m1.xmin,m1.ymin,m1.xmax,m1.ymax,object_name,dataset_ads);
        counter++;

        char c = 0;
        c = waitKey(200);
        if(c=='s')
        {
          while(waitKey(1) != 'c')
            continue;
        }
        if(c=='f')
        {
            break;
        }

        //ros::Duration(0.3).sleep(); // sleep for half a second
    }
    cout<<"build dataset stop!"<<endl;
    return 0;
}
