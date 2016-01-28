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
                                                                          //Create new MovementAnalysis
//    FWMoAn::iMin_Area = 30.;                                            // Set Min Movement Area for MoAn
    FWMoAn eins(3,10);
    eins.run_MoAn();
    
    
    
    
    
    return 0;
}
