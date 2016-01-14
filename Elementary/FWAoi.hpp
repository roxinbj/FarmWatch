//
//  FWAoi.hpp
//  FarmWatch
//
//  Created by Björn Roxin on 12/16/15.
//  Copyright © 2015 Björn Roxin. All rights reserved.
//

#ifndef FWAoi_hpp
#define FWAoi_hpp

#include <stdio.h>
#include <list>
#include <string>
#include "FWImage.hpp"

// to-do include point.h

class FWAoi :public FWImage{
public:
    
private:
    std::string               iPicname;                  // Number of picture, containing this FWAoi
    int                       iSet_number;               // number of set, to which the picture belongs
    Point                     iTL;                       // Top Left coordinate to original image
    Point                     iBR;
    
};

#endif /* FWAoi_hpp */
