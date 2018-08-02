#include "formXML.h"

using namespace cv;
using namespace std;
using namespace tinyxml2;

namespace build_dataset
{
formXML::formXML(int counter,int width,int height,int xmin,int ymin,int xmax,int ymax,string object_name,string dataset_ads):
    _index(counter),_width(width),_height(height),_xmin(xmin),_ymin(ymin),_xmax(xmax),_ymax(ymax),_object_name(object_name)
{
    int tmp_index = _index;
    char xml_file_name[7]="000000";
    for(int j=5;j>=0;j--)
    {
      if(tmp_index>0)
      {
      xml_file_name[j] ='0' + tmp_index%10;
      tmp_index/=10;
      }
      else xml_file_name[j] = '0';
    }
//      cout<<new_file_name<<endl;

    stringstream ss2;
    string i2;
    ss2<<xml_file_name;
    ss2>>i2;

    string xml_ads = dataset_ads + "/object_xmlfile/" + i2 + ".xml";
    string filename = i2 +".jpg";

    // 新建一个空文档（表示完整的xml）
    XMLDocument xmlDoc;
    // 新节点
    XMLNode * pRoot = xmlDoc.NewElement("annotation");
    // 插入到xmlDoc的第一个节点（根节点）
    xmlDoc.InsertFirstChild(pRoot);
    // 新建一个元素
    XMLElement *pElement = xmlDoc.NewElement("folder");
    pElement->SetText("VOC2007");
    pRoot->InsertFirstChild(pElement);

    // 新建一个元素
    pElement = xmlDoc.NewElement("filename");
    pElement->SetText(filename.c_str());
    pRoot->InsertEndChild(pElement);

    pElement = xmlDoc.NewElement("source");
    pRoot->InsertEndChild(pElement);

    // 新建一个节点
    XMLElement *pNewElement = xmlDoc.NewElement("database");
    pNewElement->SetText("mzm");
    pElement->InsertFirstChild(pNewElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("annotation");
    pNewElement->SetText("PASCAL VOC2007");
    pElement->InsertFirstChild(pNewElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("image");
    pNewElement->SetText("flickr");
    pElement->InsertFirstChild(pNewElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("flickrid");
    pNewElement->SetText("NULL");
    pElement->InsertFirstChild(pNewElement);

    pElement = xmlDoc.NewElement("owner");
    pRoot->InsertEndChild(pElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("flickrid");
    pNewElement->SetText("NULL");
    pElement->InsertFirstChild(pNewElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("name");
    pNewElement->SetText("lbin");
    pElement->InsertFirstChild(pNewElement);

    // 新建一个元素
    pElement = xmlDoc.NewElement("size");
    // 将该节点添加到pRoot节点下("Root")
    pRoot->InsertEndChild(pElement);

//        pElement = xmlDoc.NewElement("filename");
//        pElement->SetText(i2.c_str());
//        pRoot->InsertEndChild(pElement);

//        pElement = xmlDoc.NewElement("source");
//        pRoot->InsertEndChild(pElement);

    // 新建一个节点
    pNewElement = xmlDoc.NewElement("width");
    pNewElement->SetText(_width);
    pElement->InsertFirstChild(pNewElement);

    pNewElement = xmlDoc.NewElement("height");
    pNewElement->SetText(_height);
    pElement->InsertFirstChild(pNewElement);

    pNewElement = xmlDoc.NewElement("depth");
    pNewElement->SetText(3);
    pElement->InsertFirstChild(pNewElement);

    pElement = xmlDoc.NewElement("segmented");
    pElement->SetText(0);
    pRoot->InsertEndChild(pElement);

    pElement = xmlDoc.NewElement("object");
    pRoot->InsertEndChild(pElement);

    pNewElement = xmlDoc.NewElement("name");
    pNewElement->SetText(_object_name.c_str());
    pElement->InsertFirstChild(pNewElement);

    pNewElement = xmlDoc.NewElement("pose");
    pNewElement->SetText("Unspecified");
    pElement->InsertFirstChild(pNewElement);

    pNewElement = xmlDoc.NewElement("truncated");
    pNewElement->SetText("0");
    pElement->InsertFirstChild(pNewElement);

    pNewElement = xmlDoc.NewElement("difficult");
    pNewElement->SetText("0");
    pElement->InsertFirstChild(pNewElement);

    XMLElement *pNewElement2 = xmlDoc.NewElement("bndbox");
    pElement->InsertEndChild(pNewElement2);

    XMLElement *pNewElement3 = xmlDoc.NewElement("xmin");
    pNewElement3->SetText(_xmin);
    pNewElement2->InsertFirstChild(pNewElement3);

    pNewElement3 = xmlDoc.NewElement("ymin");
    pNewElement3->SetText(_ymin);
    pNewElement2->InsertFirstChild(pNewElement3);

    pNewElement3 = xmlDoc.NewElement("xmax");
    pNewElement3->SetText(_xmax);
    pNewElement2->InsertFirstChild(pNewElement3);

    pNewElement3 = xmlDoc.NewElement("ymax");
    pNewElement3->SetText(_ymax);
    pNewElement2->InsertFirstChild(pNewElement3);
    // 保存文件
    XMLError eResult = xmlDoc.SaveFile(xml_ads.c_str());
}
}
