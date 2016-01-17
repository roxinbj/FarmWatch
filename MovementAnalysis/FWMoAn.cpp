
#include "FWMoAn.hpp"
#include "FWImage.hpp"
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sstream>


typedef std::list<std::string>  ls;

//  ************* public ***********

FWMoAn::FWMoAn(int set_n, int nmax)
{
    this->iNmax = nmax;
    DIR *pdir = NULL;                                                   // Pointer to a directory
    struct dirent *pent = NULL;
    char str[80];
    strcpy(str, "../../bin/Data/Set");
    //char str1[20] = "../../bin/Data/Set";
    std::string n_str = std::to_string(set_n);                          // Convert int to string
    char const* pchar = n_str.c_str();                                  // Convert string to char
    
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
        
        if (pent->d_namlen > 9 && i<this->iNmax)                                         // found true image   /only first 10 pics
        {
            iPicname_list.push_back(pent->d_name);                      // Adds name of pic to iPicname_list
            //            std::stringstream ss;
            //            ss << pent->d_name ;
            FWImage* image = new FWImage(set_n,i,pent->d_name,true);         // create new image and
            //            iImage_list.push_back(image);                 // add it to the list
            iImage_vec.push_back(image);                                // add it to the vector
            i++;
        }
    }
    closedir(pdir);
    this->iNum_pics = i;                                                // iNum_pics is been set
    
    if(iNum_pics!=iPicname_list.size())
    {
        std::cout << "Length of iPicname_list is not the same as num_pics!!! ";
    }
}

FWMoAn::~FWMoAn()
{
    for(int i;i<iNum_pics;i++)
    {
        FWImage* image = iImage_vec.at(i);
        
        // first delete all AOI from current Pic
        for (int j=0; j<image->get_iAoi_vec_size(); j++)
        {
            FWAoi* aoi = image->get_aoi(j);
            image->delete_aoi(aoi);
        }
        // then delete image itself
        delete image;
    }
    iImage_vec.clear();
}


void FWMoAn::run_MoAn()
{
    iRef_Image = iImage_vec.at(0);                                          //Set first Image to background image
    int ref_number = 0;                                                     // Number referes to the x element in iImage_vec
    for(int i=1;i<iNmax-1;i++)
    {
        calc_dog(i-1,i);
        calc_contures(i);
        
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


void FWMoAn::load_directory(int n)
{
    
}

void FWMoAn::calc_dog(int ref,int pic_num)
{
    FWImage* next = iImage_vec.at(pic_num);                             // Image to be subtracted from refImage
    FWImage* ref_im  = iImage_vec.at(ref);
    absdiff(ref_im->iGray,next->iGray,next->iDog);
    threshold(next->iDog, next->iDog, 70, 255, THRESH_BINARY);
    medianBlur(next->iDog, next->iDog, 9);
    int dilation_size = 4;
    Mat element = getStructuringElement( 0,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) );
    Mat element2 = getStructuringElement( 0,Size( 4*dilation_size + 1, 4*dilation_size+1 ),Point( 2*dilation_size, 2*dilation_size ) );
    erode(next->iDog, next->iDog, element);
    dilate(next->iDog, next->iDog, element2);
    
//   Canny(next->iDog, next->iTemp, 50, 200, 3);
//   imwrite("../../bin/Temp/img.png", next->iDog);
    next->iDog_ref_n = ref;
//    next->write(3);
}

void FWMoAn::calc_contures(int pic_num)
{
    FWImage* next = iImage_vec.at(pic_num);                             // Image to be subtracted from refImage
    next->iTemp = next->iGray;
    double min_area = 3000;
    findContours(next->iDog, next->iContours, next->iHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0, 0));
    
    std::stringstream ss;
    ss << next->get_iName();
    std::cout << "contours: "<<next->iContours.size()<<", nr: "<< ss.str()<< std::endl;
    int k=0;                                                                                // k = iterator through images
    for(int i=0; i<next->iContours.size();i++)
    {
        if( contourArea(next->iContours.at(i))>min_area)                                    // j = iterator through contours
        {
            k++;
            // Create Bounding Boxes and AOI
            vector<Point>  contours_poly;
            approxPolyDP( Mat(next->iContours[i]), contours_poly, 3, true );
            Rect  rect = boundingRect(Mat(contours_poly));
            double asp_rat = rect.height/rect.width;
            
            next->iBoundingRect.push_back(rect);
            rectangle( next->iTemp, rect.tl(), rect.br(), Scalar(255,255,255), 2, 8, 0 );
            
            double area =  contourArea(next->iContours.at(i));
            std::cout << "area = " << area << std::endl;
            
            // Create AOI
            FWAoi* aoi = new FWAoi(next,rect.tl(),rect.br());
            next->add_aoi(aoi);
//            aoi->write(1,k);
        }
    }
    if (next->iBoundingRect.size()>0)
    {
        
        next->write(4);
    }
}

void FWMoAn::create_aoi(int pic_num)
{
    
    
}
