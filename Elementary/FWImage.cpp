//
//  Image.cpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#include "FWImage.hpp"
#include <string>
#include <iostream>
//#include <Point>



FWImage::FWImage(int set,int num,std::string str)
{
    this->iSet      = set;
    this->iRefnum   = num;
    this->iName     = str;
    run_im_prep();
}

void FWImage::run_im_prep()
{
    std::stringstream load,temp;
    load   << "../../bin/Data/Set" << iSet << "/" << iName;
    temp << "../../bin/Temp/" << iName;
    this->iGray = imread(load.str(),CV_LOAD_IMAGE_GRAYSCALE);
//    Sobel(this->iGray, this->iGrad, 100, 0, 1);
    imwrite(temp.str(), this->iGray);
//    if (is_day_pic())
//    {
//        this->calc_gray();                          // Converts to gray image
//    }   ////To-do
    ///Fill with list of all nessesary filters
}

bool FWImage::is_day_pic()
{
    bool answer = true;
    ////////To-do
    // Run test, whether pic is made with or without flash
    
    return answer;
}

//
//void FWImage::apply_gauß(int sigma){
//    
//    
//}
//
//
//void FWImage::sobel(){
//}
//
//void FWImage::thresh(float thresh){
//}
//
void FWImage::calc_gray(){
    cvtColor(this->iBgr, this->iGray,CV_BGR2GRAY);          // Convert BGR Pic to gray pic
}
////
////void FWImage::crop_image(Point TL, Point BR){
////    
////    
////}
////
////int FWImage::ccl(){
////}

