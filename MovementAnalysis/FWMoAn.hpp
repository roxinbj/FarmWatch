
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
    FWMoAn(int);                                            // Int referes to the set number
    ~FWMoAn();
    static float            iMin_Area;                      // Minimum required area for be called a movement
    
    void                    run_MoAn();
  
    void                    print_picname_list();
    void                    show_pic(int);                  // Display Image number x
    void                    show_pic(int,int);              // Display all Images from x to y
    int                     get_num_pics();
    
private:
    std::list<std::string>  iPicname_list;                  // List of all names of pictures of certain set
    int                     iNum_pics;                      // number of pics in set
    vec_image               iImage_vec;                     // List, containing pointers to all images in set
    FWImage*                iRef_Image;                     // Reference (Background image)
    //    l_image                 iImage_list;                    // List, containing pointers to images
    //    l_image                 iDog_list;                      // List, containing dog of image_list (dimension n-1)

    
    void                    load_set(int);                // Fill lists with images from files (only used in constructor)
    void                    calc_dog(int, int);             // Calculates Difference of gradient of two images
    
};