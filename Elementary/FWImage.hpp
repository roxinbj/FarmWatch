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


class Point;
class Mat;

class FWImage{
    
public:
    FWImage(int,std::string);
    FWImage();
    
    void                    calc_gray();                        // convertes rgb to gray
    void                    apply_gauß(int);                    // Apply gaußfilter
    void                    sobel();                            // Apply sobel filter
    void                    thresh(float);                      // Apply threshold to pic
    void                    dilation(float);                    // Apply dilation to pic
    void                    erosion(float);                     // Apply erosion to pic
    int                     ccl();                              // Run Conected components labeling
    void                    crop_image(Point, Point);           // Crop out between TopL and BottR
    
    
private:
    
//                                                                // cv Elemente
//    Mat                     iRgb;                               // Colour picture
//    Mat                     iGray;                              // Gray Pic
//    Mat                     iGrad;                              // Gradient Pic
//    Mat                     iTemp;                              // Temporary Pic
//    
    
     int                    iRefnum;                            // Number of pic
     std::string            iName;                              // Name of pic in Setx
};


#endif /* Image_hpp */
