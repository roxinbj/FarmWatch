//
//  FWAoi.cpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#include "FWAoi.hpp"

//-------------------Constructors------------------------------
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
    this->iRect = Rect(tl,br);
    
    getRectSubPix(image->iGray,s,this->iCenter,this->iGray,-1);
    if(!image->iDog.empty())                                             // Check if Dog is empty
    {
        getRectSubPix(image->iDog,s,this->iCenter,this->iDog,-1);
    }
//    if( !image->iTemp.empty())
//    {
//        getRectSubPix(image->iTemp,s,this->iCenter,this->iTemp,-1);
//    }
    if (!image->iGrad.empty())
    {
        getRectSubPix(image->iGrad,s,this->iCenter,this->iGrad,-1);
    }
    if (!image->iBgr.empty())
    {
        getRectSubPix(image->iBgr,s,this->iCenter,this->iBgr,-1);
    }
    int size_of_aoivec = image->get_iAoi_vec_size();
    this->iNum_in_Aoivec = size_of_aoivec;

    if(size_of_aoivec!=0)
    {
    }
}

FWAoi::FWAoi(FWImage* image,Rect rect):FWImage(image->get_iSet(),image->get_iNum(),image->get_iName(),false),iTL(rect.tl()),iBR(rect.br()),iRect(rect)
{
    // Securitytest, so that the cropped is not outside of the image range
//    if(br.x>image->get_cols()-2)
//    {
//        br.x = image->get_cols()-2;
//    }
//    if(br.y>image->get_rows()-2){
//        br.y = image->get_rows()-2;
//    }
//    if(tl.x<2){
//        tl.x =2;
//    }
//    if(tl.y <2){
//        tl.y =2;
//    }
//    if(tl.x > br.x){
//        std::cout << "tl.x > br.x"<<std::endl;
//    }
//    if(tl.y > br.y){
//        std::cout<< "tl.y > br.y"<<endl;
//    }
    // Fills AOI with information
    double width = rect.width;
    double height = rect.height;
    Size s;
    s.width = width;
    s.height = height;
    float mitte_x = this->iTL.x +width/2;
    float mitte_y = this->iTL.y + height/2;
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
    
    int size_of_aoivec = image->get_iAoi_vec_size();
    this->iNum_in_Aoivec = size_of_aoivec;
}

//------------------get Functions-------------------------------

Point FWAoi::get_iCenter()
{
    return this->iCenter;
}

Rect FWAoi::get_iRect()
{
    return this->iRect;
}

int FWAoi::get_iNum_in_Aoivec()
{
    return this->iNum_in_Aoivec;
}

// ----------------other Functions--------------------------------
void FWAoi::create_historgram()
{
    // Initialize parameters
    int histSize = 256;    // bin size
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    
    // Calculate histogram
    MatND hist;
    calcHist( &this->iGray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );
    
    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
    }
    std::stringstream ss;
    ss << "../../bin/Temp/Histograms/" << this->get_iNum_in_Aoivec()<<"-Histogram.png" ;
    imwrite(ss.str(), histImage);
    

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



