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

class FWAoi :public FWImage{
public:
    
private:
    std::string               picname;                // Number of picture, containing this FWAoi
    int                       set_number;             // number of set, to which the picture belongs
    
};

#endif /* FWAoi_hpp */
