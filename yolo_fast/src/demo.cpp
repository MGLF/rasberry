#include<iostream>
#include "yolo-fastestv2.h"
#include<ctime>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "yolo_fast/position.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{   
    // ros初始化
    ros::init(argc, argv, "yolo_fast");          //创建一个node
    ros::NodeHandle n;                        //创建一个句柄
    ros::NodeHandle private_nh("~");
    ros::Publisher box_pub = n.advertise<yolo_fast::position>("ob_dectetion", 1000);
    std::string model_param_file,model_bin_file;
    //设置默认参数
    //model_param_file = "/opt.param";
    //model_bin_file = "/opt.bin";
    
    private_nh.param<std::string>("model_param_file", model_param_file,"/opt.param");
    private_nh.param<std::string>("model_bin_file", model_bin_file,"/opt.bin");
 

    static const char* class_names[] = {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
        "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
        "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
        "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
        "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
        "hair drier", "toothbrush"
    };
    
    yoloFastestv2 api;
    clock_t startTime,endTime;
    yolo_fast::position obj_position; 
    api.loadModel(model_param_file.c_str(),model_bin_file.c_str());

       
    VideoCapture capture(0);
    cv::Mat cvImg;
    std::vector<TargetBox> boxes;
    while(ros::ok())
    { 
        capture>>cvImg;
        waitKey(30);//延时30毫秒
        startTime = clock();//计时开始
        api.detection(cvImg, boxes);
        endTime = clock();//计时结束
        //cout <<cvImg.size(); //640×480
        //cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC *1000 << "ms" << endl;
        // bounding box处理	
        for (int i = 0; i < boxes.size(); i++) {
            std::cout<<i<<" "<<boxes[i].x1<<" "<<boxes[i].y1<<" "<<boxes[i].x2<<" "<<boxes[i].y2
                 <<" "<<boxes[i].score<<" "<<boxes[i].cate<<std::endl;
            char text[256];
            sprintf(text, "%s %.1f%%", class_names[boxes[i].cate], boxes[i].score * 100);

            int baseLine = 0;
            cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

            int x = boxes[i].x1;
            int y = boxes[i].y1 - label_size.height - baseLine;
            if (y < 0)
                y = 0;
            if (x + label_size.width > cvImg.cols)
                x = cvImg.cols - label_size.width;

            cv::rectangle(cvImg, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      cv::Scalar(255, 255, 255), -1);

            cv::putText(cvImg, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

            cv::rectangle (cvImg, cv::Point(boxes[i].x1, boxes[i].y1), 
                       cv::Point(boxes[i].x2, boxes[i].y2), cv::Scalar(255, 255, 0), 2, 2, 0);
        }
        if(boxes.size())
        {
           cv::Point P;
           obj_position.centerX = boxes[0].x1+(boxes[0].x2 - boxes[0].x1)*0.5;
           obj_position.centerY = boxes[0].y1+(boxes[0].y2 - boxes[0].y1)*0.5;
           box_pub.publish(obj_position);
           P.x = obj_position.centerX;
           P.y = obj_position.centerY;
           cv::circle(cvImg, P, 3,Scalar(255,0,0),-1);
        }
        imshow("Bound_box", cvImg);
        
        
    }
    

    return 0;
}
