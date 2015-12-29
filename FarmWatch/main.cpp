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
//#include <opencv/highgui.h>
//#include <opencv2/core.hpp>
//#include <opencv/cv.h>

int main(int argc, const char * argv[]) {
   
                                                                        //Create new MovementAnalysis
//    FWMoAn::iMin_Area = 30.;                                            // Set Min Movement Area for MoAn
    FWMoAn eins("1");
    std::cout << "Size ist: "<< eins.get_num_pics()<<std::endl;
    eins.print_picname_list();
    
    
    return 0;
}
