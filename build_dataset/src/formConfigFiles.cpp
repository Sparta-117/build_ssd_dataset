//ros
#include <ros/ros.h>
#include <ros/package.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

//tinyxml
#include "tinyxml2.h"

//opencv
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//c++
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>

#define rw 640
#define rh 480

using namespace cv;
using namespace std;
using namespace tinyxml2;

int main(int argc,char *argv[])
{
    int object_num = 3;
    string object_adr[3];
    int file_num = 800; //for every object
    int total_index = 1;
    vector<string> file_table;
    vector<string> test_table;
    vector<string> val_table;
    vector<string> train_table;
    vector<string> trainval_table;
    float test_ratio = 0.15;
    float val_ratio = 0.08;

    object_adr[0] = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/movefree";
    object_adr[1] = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/zhencui";
    object_adr[2] = "/home/mzm/build_dataset_pic/src/build_dataset/object_dataset/taiping";

    string xml_file = "/object_xmlfile/";
    string rgb_file = "/object_rgb_dataset/";

    string new_xml_file = "/home/mzm/build_dataset_pic/src/build_dataset/voc2007_object_dataset/Annotations/";
    string new_rgb_file = "/home/mzm/build_dataset_pic/src/build_dataset/voc2007_object_dataset/JPEGImages/";
    string new_config_file = "/home/mzm/build_dataset_pic/src/build_dataset/voc2007_object_dataset/ImageSets/Main/";

    string test_file = "/test.txt";
    string train_file = "/train.txt";
    string val_file = "/val.txt";
    string trainval_file = "/trainval.txt";


    for(int n=0;n<object_num;n++)
    {
        for(int i = 1;i<=file_num;i++)
        {
            char file_name[7]="000000";
            char new_file_name[7]="000000";
            int index = i;
            int new_index = total_index;
            for(int j=5;j>=0;j--)
            {
                if(index>0)
                {
                file_name[j] ='0' + index%10;
                index/=10;
                }
                else file_name[j] = '0';
            }

            for(int j=5;j>=0;j--)
            {
                if(new_index>0)
                {
                new_file_name[j] ='0' + new_index%10;
                new_index/=10;
                }
                else new_file_name[j] = '0';
            }

            stringstream ss1;
            string i1;
            ss1<<file_name;
            ss1>>i1;

            stringstream ss2;
            string i2;
            ss2<<new_file_name;
            ss2>>i2;

            string title1 = object_adr[n]+xml_file+i1+".xml";
            string title2 = new_xml_file+i2+".xml";

            //save xml files
            XMLDocument docXml;
            XMLError errXml = docXml.LoadFile(title1.c_str());
            if (XML_SUCCESS == errXml)
            {
                XMLError eResult = docXml.SaveFile(title2.c_str());
                 if (XML_SUCCESS == eResult)
                     cout<<"save no: "<< i2 <<" xml success."<<endl;
            }
            else
            {
                cout<<"save xml file failed."<<endl;
            }

            //save rgb images
            string title3 = object_adr[n]+rgb_file+i1+".jpg";
            string title4 = new_rgb_file+i2+".jpg";
            Mat rgb;
            rgb = imread(title3);
            imwrite(title4,rgb);
            cout<<"save no: "<< i2 <<" jpg success."<<endl;

            total_index++;
        }
    }

    //save config files
    for(int i=1;i<=file_num*object_num;i++)
    {
        char result_file_name[7]="000000";
        int result_index = i;

        for(int j=5;j>=0;j--)
        {
            if(result_index>0)
            {
            result_file_name[j] ='0' + result_index%10;
            result_index/=10;
            }
            else result_file_name[j] = '0';
        }

        stringstream ss3;
        string i3;
        ss3<<result_file_name;
        ss3>>i3;

        file_table.push_back(i3.c_str());
    }
    random_shuffle(file_table.begin(),file_table.end()); /* 打乱顺序 */

    int test_num = file_num*object_num*test_ratio;
    int val_num = (file_num*object_num-test_num)*val_ratio;
    for(int i=0;i<test_num;i++)
    {
        test_table.push_back(file_table[i]);
    }
    for(int i=test_num;i<test_num+val_num;i++)
    {
        val_table.push_back(file_table[i]);
    }
    for(int i=test_num+val_num;i<file_table.size();i++)
    {
        train_table.push_back(file_table[i]);
    }
    for(int i=test_num;i<file_table.size();i++)
    {
        trainval_table.push_back(file_table[i]);
    }

    ofstream test_stream;
    ofstream train_stream;
    ofstream val_stream;
    ofstream trainval_stream;

    string title5 = new_config_file+test_file;
    string title6 = new_config_file+train_file;
    string title7 = new_config_file+val_file;
    string title8 = new_config_file+trainval_file;

    test_stream.open(title5.c_str());
    train_stream.open(title6.c_str());
    val_stream.open(title7.c_str());
    trainval_stream.open(title8.c_str());

    cout<<"file:"<<endl;
    for (int i = 0; i<file_table.size(); i++)
    {
         cout<<file_table[i]<<" "; /* 显示打乱顺序后的元素 */
    }
    cout<<endl;
    cout<<"test:"<<endl;
    for (int i = 0; i<test_table.size(); i++)
    {
         cout<<test_table[i]<<" "; /* 显示打乱顺序后的元素 */
         test_stream<<test_table[i]<<endl;
    }
    cout<<endl;
    cout<<"val:"<<endl;
    for (int i = 0; i<val_table.size(); i++)
    {
         cout<<val_table[i]<<" "; /* 显示打乱顺序后的元素 */
         val_stream<<val_table[i]<<endl;
    }
    cout<<endl;
    cout<<"train:"<<endl;
    for (int i = 0; i<train_table.size(); i++)
    {
         cout<<train_table[i]<<" "; /* 显示打乱顺序后的元素 */
         train_stream<<train_table[i]<<endl;
    }
    cout<<endl;
    cout<<"trainval:"<<endl;
    for (int i = 0; i<trainval_table.size(); i++)
    {
         cout<<trainval_table[i]<<" "; /* 显示打乱顺序后的元素 */
         trainval_stream<<trainval_table[i]<<endl;
    }

    test_stream.close();
    train_stream.close();
    val_stream.close();
    trainval_stream.close();

//    cout<<file_table<<endl;

}
