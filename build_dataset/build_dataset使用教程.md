# build_dataset使用方法

build_dataset包主要有两个功能：

- **在使用绿幕环境下，对绿布上的物体建立数据集。数据集包括rgb图像，像素级真值图和voc2007版本的xml文件**
- **生成voc2007标准的数据包**
-------------------

## 采集数据

 - 在object_dataset文件夹下按照example的格式建立物体的空数据文件夹。其下包括object_groundtruth_dataset，object_rgb_dataset和object_xmlfile
 - 打开相机。有realsense_driver包时，使用：```roslaunch realsense_driver realsense.launch```打开realsense
 - 校准HSV的绿色背景分割参数，可在main.cpp中调整hsv的参数为最佳效果
 - 运行```rosrun build_dataset build_dataset 物体名字```，按s暂停录入数据，按c继续录入，按f停止录入。

## 生成voc2007标准的数据包
 - 完成物体数据录入后，需要调整formConfigFiles.cpp文件内的物体个数，物体图片数量和文件地址。
 - 运行```rosrun build_dataset form_configfils```即可在voc2007_object_dataset文件夹内生成voc2007的标准数据集

## 导入SSD
 - 首先在/home/mzm/data文件夹下将voc2007标准数据包复制进去，然后命名为自己的名字
 - 在/ssd_caffe/caffe/data文件夹下建立自己的数据文件夹。这就是我们自己的数据分组文件和LMDB生成文件的所在地。然后将同样是数据文件夹VOC0712中的create_list.sh,create_data.sh和labelmap_voc.prototxt拷贝过来。并修改其中的一些地方：


create_list.sh中，改for name in 后面的为mydataset

create_data.sh中，改为：
```
dataset_name="mydataset"
mapfile="$root_dir/data/$dataset_name/labelmap_voc.prototxt"
```
注意labelmap_voc.prototxt为标注映射文件，我们要把里面的内容改为自己设定的分类名称和id号。这里的id号要和标注时设定的物体id号相同。

- 再使用```./data/mydataset/create_list.sh```对数据集进行分组
- 再使用```./data/mydataset/create_data.sh```将数据转换为LMDB格式

## 训练SSD
- 修改一下examples/ssd/ssd_pascal.py中的一些配置和地址。具体可以参考：
https://blog.csdn.net/sparta_117/article/details/78226045
- 使用```python examples/ssd/ssd_pascal.py```训练ssd网络

## 使用摄像头识别物体
- 修改examples/ssd/ssd_pascal_webcam.py里面的类别，映射和保存地址
- 运行```python examples/ssd/ssd_pascal_webcam.py```就可以看到摄像头识别的结果了