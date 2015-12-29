
#include "FWMoAn.hpp"
#include "FWImage.hpp"
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sstream>

typedef std::list<std::string>  ls;

FWMoAn::FWMoAn(char* set_n){
    this->load_set(set_n);
    for(int i=0; i<iNum_pics; i++)
    {
        std::stringstream ss;
        ss << "Bild"<<i;
        FWImage* image = new FWImage(i,ss.str());       // create new image and
        iImage_list.push_back(image);                   // add it to the list
        iImage_vec.push_back(image);                    // add it to the list
    }
    iRef_Image = iImage_vec.at(0);                      // Set first image to reference/backgrpund image
    
}

FWMoAn::~FWMoAn(){
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

    iImage_list.clear();
    
}


void FWMoAn::load_set(char* n){
    DIR *pdir = NULL;                                   // Pointer to a directory
    struct dirent *pent = NULL;
    char str[80];
    strcpy(str, "../../bin/Data/Set");
    //char str1[20] = "../../bin/Data/Set";
    strcat(str,n);
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
        ss << "im" << i;                                    // name of current image object
        if (pent->d_namlen > 2)
        {
            iPicname_list.push_back(pent->d_name);
            i++;
        }
       // iImage_list.push_back(image delta);
        //std::cout << "%s\n"<< pent->d_name<<std::endl;
    }
    closedir(pdir);
    
    //Set constants of FWMoAn
    this->iNum_pics = i;
    
    if(iNum_pics!=iPicname_list.size())
    {
        std::cout << "Length of iPicname_list is not the same as num_pics!!! ";
    }
}

void FWMoAn::run_MoAn(){
    
                                                            // Filter all Images
                                                            // Create DOG
    
}



void FWMoAn::filter(){
    
    for(int i=0;i<get_num_pics();i++)                       // Iterate through whole image vector
    {
        FWImage* image = iImage_vec.at(i);
//        
//        image->calc_gray();
//        image->apply_gauß(5);
//        image->sobel();
//        image->thresh(25.);
//        int labels = image->ccl();
//        
    }
    
}


void FWMoAn::print_picname_list(){
    
    ls::iterator i = iPicname_list.begin();
    while (i!=iPicname_list.end())
    {
        std::cout << *i <<std::endl;
        ++i;
    }
}

int FWMoAn::get_num_pics(){
    return iNum_pics;
}