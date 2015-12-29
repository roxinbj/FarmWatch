
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "../Elementary/FWAoi.hpp"

class FWImage;

typedef std::list<FWImage*> l_image;
typedef std::list<FWImage*>::iterator l_image_it;

typedef std::vector<FWImage*> vec_image;
typedef std::vector<FWImage*>::iterator vec_image_it;


class FWMoAn{
    
public:
    FWMoAn(char*);                                          // Char referes to the set number
    ~FWMoAn();
    
    void                    run_MoAn();
    void                    calc_dog();
    void                    print_picname_list();
    void                    show_pic(int);                  // Display Image number x
    void                    show_pic(int,int);              // Display all Images from x to y
    int                     get_num_pics();
    void                    filter();                       // Applies required filters to all images
    void                    filter(int);                    // Apply required filter to image number i
    static float            iMin_Area;                      // Minimum required area for be called a movement
    
private:
    void                    load_set(char*);                // Fill lists with images from files
    
    std::list<std::string>  iPicname_list;                  // List of all names of pictures of certain set
    int                     iNum_pics;                      // number of pics in set
    l_image                 iImage_list;                    // List, containing pointers to images
    l_image                 iDog_list;                      // List, containing dog of image_list (dimension n-1)
    vec_image               iImage_vec;                     // List, containing pointers to images
    FWImage*                iRef_Image;                     // Reference (Background image)
    
};