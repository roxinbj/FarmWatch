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

//-------------------Constructors------------------------------

FWImage::FWImage(int set,int num,std::string name, bool load_pic)
{
    this->iSet      = set;
    this->iNum      = num;
    this->iName     = name;
    if(load_pic) run_im_prep();
}

FWImage::~FWImage()
{
    iAoi_vec.clear();
}


//------------------get Functions-------------------------------

int FWImage::get_iAoi_vec_size()
{
    if (!this->iAoi_vec.empty())
    {
        return this->iAoi_vec.size();
    }
    else return 0;
}

FWAoi* FWImage::get_aoi(int i)
{
    return this->iAoi_vec.at(i);
}

int FWImage::get_rows()
{
    return this->iGray.rows;
}

int FWImage::get_cols()
{
    return this->iGray.cols;
}

std::string FWImage::get_iName()
{
    return this->iName;
}

int FWImage::get_iNum()
{
    return this->iNum;
}

int FWImage::get_iSet()
{
    return this->iSet;
}


// -------------------other functions -------------------------

void FWImage::run_im_prep()
{
    std::stringstream load;
    load   << "../../bin/Data/Set" << iSet << "/" << get_iName();
    this->iGray = imread(load.str(),CV_LOAD_IMAGE_GRAYSCALE);                           // load Image from file
    
    //Prepare Gradient of image
    GaussianBlur( this->iGray, this->iGray, Size(3,3), 0, 0, BORDER_DEFAULT );          //Apply Gauß filter
    this->createGrad(1,0);
       
//    write(1);
    
}

void FWImage::calc_gray()
{
    cvtColor(this->iBgr, this->iGray,CV_BGR2GRAY);          // Convert BGR Pic to gray pic
}

void FWImage::createGrad(int scale,int delta)
{
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    int ddepth = CV_16S;
    
    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( this->iGray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( this->iGray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, this->iGrad );
    
}

void FWImage::add_aoi(FWAoi* aoi)
{
    this->iAoi_vec.push_back(aoi);
}

void FWImage::delete_aoi(FWAoi* aoi)
{
    delete aoi;
}

void FWImage::write(int i)
{
    std::stringstream ss;
    ss << "../../bin/Temp/" << get_iName()<<".png";
    switch (i)
    {
        case 1:
            imwrite(ss.str(), this->iGray);
            break;
        case 2:
            imwrite(ss.str(), this->iGrad);
            break;
        case 3:
            imwrite(ss.str(), this->iDog);
            break;
        case 4:
            imwrite(ss.str(), this->iTemp);
            break;
            
        default:
            break;
    }
}


bool FWImage::is_day_pic()
{
    bool answer = true;
    
    return answer;
}
