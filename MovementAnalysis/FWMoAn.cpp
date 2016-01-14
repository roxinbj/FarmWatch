
#include "FWMoAn.hpp"
#include "FWImage.hpp"
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sstream>

typedef std::list<std::string>  ls;

//  ************* public ***********

FWMoAn::FWMoAn(int set_n)
{
    this->load_set(set_n);
}

FWMoAn::~FWMoAn()
{
    for(int i;i<iNum_pics;i++)
    {
        FWImage *image = iImage_vec.at(i);
        delete image;
    }
    iImage_vec.clear();

    
//    l_image_it it = iImage_list.begin();
//    while (it != iImage_list.end())
//    {
//        FWImage *image = *it;
//        delete image;
//        it++;
//    }

//    iImage_list.clear();
    
}


void FWMoAn::run_MoAn()
{
    iRef_Image = iImage_vec.at(0);                          //Set first Image to background image
    int ref_number = 0;
    for(int i=1;i<5;i++)
    {
        calc_dog(ref_number,i);
        
    }
}


void FWMoAn::print_picname_list()
{
    ls::iterator i = iPicname_list.begin();
    while (i!=iPicname_list.end())
    {
        std::cout << *i <<std::endl;
        ++i;
    }
}

int FWMoAn::get_num_pics()
{
    return iNum_pics;
}



// ************* private ************


void FWMoAn::load_set(int n)
{
    DIR *pdir = NULL;                                   // Pointer to a directory
    struct dirent *pent = NULL;
    char str[80];
    strcpy(str, "../../bin/Data/Set");
    //char str1[20] = "../../bin/Data/Set";
    std::string n_str = std::to_string(n);              // Convert int to string
    char const* pchar = n_str.c_str();                  // Convert string to char
    
    strcat(str,pchar);
    strcat(str, "/.");
    puts(str);
  
    pdir = opendir (str);
    if(pdir == NULL)
    {
        std::cout << "No directory found";
        exit(1);
    }
    int i=0;
    while ((pent = readdir(pdir)))
    {
        if(pent==NULL)
        {
            std::cout << "pent Problem";
            exit(2);
        }
        std::stringstream ss;
        ss << "im" << i;                                        // name of current image object
        if (pent->d_namlen > 2)                                 // found true image
        {
            iPicname_list.push_back(pent->d_name);              // Adds name of pic to iPicname_list
            std::stringstream ss;
            ss << pent->d_name ;
            FWImage* image = new FWImage(n,i,ss.str());         // create new image and
//            iImage_list.push_back(image);                       // add it to the list
            iImage_vec.push_back(image);                        // add it to the vector
            i++;
        }
    }
    closedir(pdir);
    this->iNum_pics = i;                                        // iNum_pics is been set
    
    if(iNum_pics!=iPicname_list.size())
    {
        std::cout << "Length of iPicname_list is not the same as num_pics!!! ";
    }
}

void FWMoAn::calc_dog(int ref,int num)
{
    FWImage* next = iImage_vec.at(num);
    //to-do calculate gradient of image (or fill)
    //        next->iDog = next->iGrad-iRef_Image->iGrad;
    next->iDog_ref_n = ref;
    
}
