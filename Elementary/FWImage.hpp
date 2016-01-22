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
using namespace std;

class FWAoi;

class FWImage{
    
public:
    FWImage(int,int,std::string,bool);                          // bool true: run_im_prep() included, false: not 
    FWImage();
    ~FWImage();
    Mat                     iGray;                              // Mat containing Gray image
    Mat                     iGrad;                              // Mat containing Gradient of image
    Mat                     iBgr;                               // Mat containing RGB (vec3) image
    Mat                     iTemp;                              // Mat needed for calculations
    Mat                     iDog;                               // Mat containing DOG to ref image number
    int                     iDog_ref_n;                         // Ref image number (needed for calculation of iDog)
    vector<vector<Point> >  iContours;
    vector<Vec4i>           iHierarchy;
    vector<Rect>            iBoundingRect;
    
    
    void                    write(int);                         // Write image to Temp Folder for analysis (1:IGray, 2:iGrad, 3:iDog, 4:iBgr)
    void                    calc_gray();                        // convertes rgb to gray
    bool                    is_day_pic();                       // yes, if picture is taken during day (color)
    
    void                    add_aoi(FWAoi*);                    // Adds element to Aoi Vector
    void                    delete_aoi(FWAoi*);                 // deletes element of Aoi Vector
    FWAoi*                  get_aoi(int);                         // get Aoi from vector
    int                     get_iAoi_vec_size();                // return size iAoi_vec
    int                     get_rows();                         // return number of rows
    int                     get_cols();                         // return number of cols
    std::string             get_iName();                        // return iName
    int                     get_iNum();                         // return iNum
    int                     get_iSet();                         // return iSet
    
       
    
protected:                                                      // Also available in vererbten Klassen
    void                    run_im_prep();                      // runs nessesary filters on image (gauß, gray,)
    void                    createGrad(int,int);                // Create Gradient (scale =1, delta=0)
    int                     iSet;                               // Number of Set
    int                     iNum;                               // i-Number of pic
    std::string             iName;                              // Name of pic in Setx
    
private:
    vector<FWAoi*>          iAoi_vec;                           // Vector containing all Aoi of current image
    
   };


#endif /* Image_hpp */
