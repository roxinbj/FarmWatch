//
//  Image.hpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <stdio.h>
#include <string>

//#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#include "mat.hpp"

using namespace cv;

//class Point{};
//class Mat{};

class FWImage{
    
public:
    FWImage(int,int,std::string);
    FWImage();
    
    Mat                    iGray;                              // Mat containing Gray image
    Mat                    iGrad;                              // Mat containing Gradient of image
    Mat                    iBgr;                               // Mat containing RGB (vec3) image
    Mat                    iTemp;                              // Mat needed for calculations
    Mat                    iDog;                               // Mat containing DOG to ref image number
    int                    iDog_ref_n;                         // Ref image number (needed for calculation of iDog)
    
    void                    run_im_prep();                      // runs nessesary filters on image (gauß, gray,)
    
    void                    calc_gray();                        // convertes rgb to gray
//    void                    apply_gauß(int);                    // Apply gaußfilter
//    void                    sobel();                            // Apply sobel filter
//    void                    thresh(float);                      // Apply threshold to pic
//    void                    dilation(float);                    // Apply dilation to pic
//    void                    erosion(float);                     // Apply erosion to pic
//    int                     ccl();                              // Run Conected components labeling, return number of components
    bool                    is_day_pic();                       // yes, if picture is taken during day (color)
    void                    crop_image(Point, Point);           // Crop out between TopL and BottR (to-do : Mat?)
    
    
private:
    int                    iSet;                               // Number of Set
    int                    iRefnum;                            // Number of pic
    std::string            iName;                              // Name of pic in Setx
};


#endif /* Image_hpp */
