//
//  FWAoi.hpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#ifndef FWAoi_hpp
#define FWAoi_hpp

#include <iostream>
#include <stdio.h>
#include <list>
#include <string>
#include "FWImage.hpp"


// to-do include point.h

class FWAoi :public FWImage
{
public:
    FWAoi(FWImage*, Point,Point);
    FWAoi(FWImage*,Rect);
    ~FWAoi();
    
    Point                       get_iCenter();              // Get Center of AOI
    int                         get_iNum_in_Aoivec();
    Rect                        get_iRect();
    void                        write(int,int);             // Write image to Temp Folder for analysis (1:IGray, 2:iGrad, 3:iDog, 4:iBgr)
private:
    std::string                 iPicname;                   // Number of picture, containing this FWAoi
    int                         iSet_number;                // number of set, to which the picture belongs
    Point                       iTL;                        // Top Left coordinate to original image
    Point                       iBR;
    Point                       iCenter;
    int                         iNum_in_Aoivec;              // Number, where in Aoi Vec of image this aoi is
    Rect                        iRect;
};

#endif /* FWAoi_hpp */
