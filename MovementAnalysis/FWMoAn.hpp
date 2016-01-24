
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "../Elementary/FWAoi.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class FWImage;

typedef std::list<FWImage*> l_image;
typedef std::list<FWImage*>::iterator l_image_it;

typedef std::vector<FWImage*> vec_image;
typedef std::vector<FWImage*>::iterator vec_image_it;


class FWMoAn{
    
public:
    FWMoAn(int, int);                                       // Int referes to the set number, second int is the max number of pics to walk through
    ~FWMoAn();
    static float            iMin_Area;                      // Minimum required area for be called a movement
    
    void                    run_MoAn();
  
    void                    print_picname_list();
    int                     get_num_pics();
    int                     iNmax;                           // iterate untill picture nmax
//    int                     iFWAoi_match(FWAoi*);           // input: Aoi output: number, where correspondent Aoi is in previous pic
    bool                    iAoi_exist_prev(FWAoi*);              // Tests whether the same Aoi already exists in previous image
    FWAoi*                  iAoi_intersects(FWImage*, FWAoi*);
    
private:
    std::list<std::string>  iPicname_list;                  // List of all names of pictures of certain set   eg.(MFDC....JPG)
    vec_image               iImage_vec;                     // List, containing pointers to all images in set
    FWImage*                iRef_Image;                     // Reference (Background image)
    int                     iNum_pics;                      // total number of pics in set
    
    void                    load_directory(int);                // Fill lists with images from files (only used in constructor)
    void                    calc_dog(int, int);             // Calculates Difference of gradient of two images
    void                    calc_contures(int);             // Filles the contur vector of int pic
    void                    create_aoi(int);                // Create AOI of all bounding boxes in pic i
    
    
    
};