#ifndef FORMXML_H
#define FORMXML_H

#include "common_include.h"
//tinyxml
#include "tinyxml2.h"

using namespace cv;
using namespace std;
using namespace tinyxml2;

namespace build_dataset
{
class formXML
{
public:
    formXML(int counter,int width,int height,int xmin,int ymin,int xmax,int ymax,string object_name,string dataset_ads);
private:
    string _object_name;
    int _index;
    int _width;
    int _height;
    int _xmin;
    int _xmax;
    int _ymin;
    int _ymax;
};
}

#endif //FORMXML_H
