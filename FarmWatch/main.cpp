//
//  main.cpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#include <iostream>
#include <list>
#include "../MovementAnalysis/FWMoAn.hpp"
#include "../Elementary/FWImage.hpp"
#include "highgui.hpp"
#include "core.hpp"
#include "imgproc.hpp"
#include <string>
#include <opencv2/core.hpp>
#include <opencv/cv.h>
using namespace cv;
using namespace std;


int main(int argc, const char * argv[]) {
    
//    cv::Mat A << 1 1;
////    A = cv::Mat::eye(2,2);
//    cv::imwrite("im.png", A);
   
    string windowname = "Image";
    Mat imgB = Mat::ones(2, 2, CV_32F);
    Mat image = imread("/Users/roxinbj/Documents/CPP/Xcode_workspace/FarmWatch/bin/Data/Set1/MFDC8793.JPG", CV_LOAD_IMAGE_COLOR);
//    cout<< "image color:"<<image.channels()<<endl;
    
    Mat image2 = imread("/Users/roxinbj/Documents/CPP/Xcode_workspace/FarmWatch/bin/Data/Set1/MFDC8916.JPG", CV_LOAD_IMAGE_GRAYSCALE);
//    cout <<"image gray: "<<image2.channels();
    Mat grau;
//    cvtColor(image,grau,CV_BGR2GRAY);
//    GaussianBlur(grau, grau, 30,50);
    for(int i = 0; i < 200; i++){
        for(int j = 0; j < 200; j++){
            image.at<uchar>(i, j) = 0;
        }
    }
    imwrite("/Users/roxinbj/Documents/CPP/Xcode_workspace/FarmWatch/bin/Temp/im.png", image2);
    
    
                                                                        //Create new MovementAnalysis
//    FWMoAn::iMin_Area = 30.;                                            // Set Min Movement Area for MoAn
    FWMoAn eins(1);
    std::cout << "Size ist: "<< eins.get_num_pics()<<std::endl;
//    eins.print_picname_list();
    
    
    return 0;
}
