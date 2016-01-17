//
//  FWAoi.cpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#include "FWAoi.hpp"

FWAoi::FWAoi(FWImage* image, Point tl,Point br) :FWImage(image->get_iSet(),image->get_iNum(),image->get_iName(),false),iTL(tl),iBR(br)
{
    
    // Securitytest, so that the cropped is not outside of the image range
    if(br.x>image->get_cols()-2)
    {
        br.x = image->get_cols()-2;
    }
    if(br.y>image->get_rows()-2){
        br.y = image->get_rows()-2;
    }
    if(tl.x<2){
        tl.x =2;
    }
    if(tl.y <2){
        tl.y =2;
    }
    if(tl.x > br.x){
        std::cout << "tl.x > br.x"<<std::endl;
    }
    if(tl.y > br.y){
        std::cout<< "tl.y > br.y"<<endl;
    }
    // Fills AOI with information
    double width = br.x-tl.x;
    double height = br.y -tl.y;
    Size s;
    s.width = width;
    s.height = height;
    float mitte_x = tl.x +width/2;
    float mitte_y = tl.y + height/2;
    this->iCenter = Point(mitte_x,mitte_y);
    
    getRectSubPix(image->iGray,s,this->iCenter,this->iGray,-1);
    if(!image->iDog.empty())                                             // Check if Dog is empty
    {
        getRectSubPix(image->iDog,s,this->iCenter,this->iDog,-1);
    }
    if( !image->iTemp.empty())
    {
        getRectSubPix(image->iTemp,s,this->iCenter,this->iTemp,-1);
    }
    if (!image->iGrad.empty())
    {
        getRectSubPix(image->iGrad,s,this->iCenter,this->iGrad,-1);
    }
    if (!image->iBgr.empty())
    {
        getRectSubPix(image->iBgr,s,this->iCenter,this->iBgr,-1);
    }
}


void FWAoi::write(int choice, int num_aoi)
{
    std::stringstream ss;
    ss << "../../bin/Temp/" << get_iName()<<"-AOI-" <<num_aoi<< ".png";
    switch (choice)
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



