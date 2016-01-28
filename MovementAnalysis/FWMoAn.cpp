
#include "FWMoAn.hpp"
#include "FWImage.hpp"
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sstream>

typedef double tNumber;
typedef std::list<std::string>  ls;

// Global Variables in FWMoAn
FWImage* previous;
FWImage* current;
vector<Rect> potential_aois;
Size dim(1920,1080);
//  ************* public ***********



//-------------------Constructors------------------------------

FWMoAn::FWMoAn(int set_n, int nmax)
{
    if (nmax !=0)                   //if nmax ==0 --> load all pictures in set
    {
        this->iNmax = nmax;
    }
    else this->iNmax = 100;
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
            //            iImage_list.push_back(image);                 // add it to the list
           
            i++;
        }
    }
    closedir(pdir);
    this->iNum_pics = i;                                                // iNum_pics is been set
    iImage_vec.reserve(iNum_pics);

    
    for(int it=0;it<iNum_pics;it++)                                     // TODO Parallel
    {
        FWImage* image = new FWImage(set_n,it,this->iPicname_list.at(it),true);         // create new image and
//        iImage_vec.insert(iterator,image);
        iImage_vec.push_back(image);                                // add it to the vector
    }
    
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

//------------------get Functions-------------------------------


int FWMoAn::get_num_pics()
{
    return iNum_pics;
}

void FWMoAn::run_MoAn()
{
    //1. Fill iImage Vector with all images --> done in constructor
    //2. Select image Pair and find movements in second image
    //2.1 Fil rects[array] : Calc DOG of two images, analyse DOG for contures and create rect array of possible movements,
    //2.1.1 merge intersecting rects --> update rect array
    //2.2 test rect[i] : correlation rects in two images, possibly another test
    
  
    
    iRef_Image = iImage_vec.at(0);                                          //Set first Image to background image
    for(int i=1;i<iNmax-1;i++)
    {
        previous    = iImage_vec.at(i-1);
        current       = iImage_vec.at(i);
        
        calc_dog(i-1,i);                                                // create Dog, fills rect array with possible movements
        calc_contures(i);
        
    }
}


void FWMoAn::print_picname_list()
{
//
    for(int i=0;i<iPicname_list.size();i++)
    {
        std::cout<< iPicname_list.at(i);
    }
    
//    ls::iterator i = iPicname_list.begin();
//    while (i!=iPicname_list.end())
//    {
//        std::cout << *i <<std::endl;
//        ++i;
//    }
}

//int FWMoAn::iFWAoi_match(FWAoi* aoi)
//{
////    int image_num = aoi->get_iNum();
////    FWImage* prev = iImage_vec.at(image_num-1);
////    int aoi_vec_size = prev->get_iAoi_vec_size();
//    return 0;
//}

bool FWMoAn::iAoi_exist_prev(FWAoi* aoi)
{
    int image_num = aoi->get_iNum();
    
    FWImage* prev = iImage_vec.at(image_num-1);
    int aoi_vec_size = prev->get_iAoi_vec_size();
    tNumber center_dist[aoi_vec_size];                                              // create array, containing all error sums of aoi's
    if (aoi_vec_size!=0)
    {
        // Get closest Aoi
        FWAoi* aoi_close = prev->get_aoi(0);
        tNumber min_dist = 100000000;
        int aoi_num=0;
        for(int k=0;k<aoi_vec_size;k++)                                                 // iterate through aoi's in prev
        {
            FWAoi* aoi_b = prev->get_aoi(k);                                            // aoi_b is new tested aoi
            Point d_center = Point(aoi->get_iCenter())-Point(aoi_b->get_iCenter());
            center_dist[k] = std::sqrt(d_center.x*d_center.x+d_center.y*d_center.y);
            if(center_dist[k] < min_dist)
            {
                min_dist=center_dist[k];                                                // overwrite min_dist
                aoi_close = aoi_b;                                                      // set aoi_close to current aoi_b
                aoi_num = k;
            }
        }
//        std::cout<<"min_dst: "<< min_dist<<std::endl;
        if(min_dist < 20)
        {
//            std::cout << "AP:"<< "n:"<< aoi->get_iNum_in_Aoivec() << "and p:"<< aoi_close->get_iNum_in_Aoivec()<< std::endl;
            return true;
        }
        else return false;
    }
    else return false;
}

FWAoi* FWMoAn::iAoi_intersects(FWImage* image_in, FWAoi* aoi)
{
    FWImage* image = image_in;
    int max_aoi = image->get_iAoi_vec_size();
    Rect rect1 = aoi->get_iRect();
    Rect newrect;
    bool intersection = false;                              // will be turned true if intersection found
    for(int i=0; i<max_aoi;i++)           // Iterate through Aoi Vector
    {
        FWAoi* aoi_test = image->get_aoi(i);
        Rect rect_i =  aoi_test->get_iRect();
        
        if((rect1 & rect_i)==rect1 || (rect1 & rect_i)==rect_i)                         // rect1 is completely in rect_i
        {
            if(rect1.area()>rect_i.area())
                newrect = rect1;
            
            else if (rect_i.area()>rect1.area())
                newrect = rect_i;
            else
                std::cout <<"error in rect comparison"<<endl;
            
            intersection = true;

        }
        else if((rect1 & rect_i).area() > 0)                // rects intersect
        {
            newrect = rect1 | rect_i;
            intersection = true;
        }
    }
    if(intersection)
    {
        // create new AOI for newrect
        FWAoi* new_aoi = new FWAoi(image,newrect);
        return new_aoi;
    }
    else return aoi;
}

// ************* private ************



void FWMoAn::calc_dog(int ref,int pic_num)
{
    // 1. create DOG
    
    absdiff(previous->iGray,current->iGray,current->iDog);
//    current->write(3);
    medianBlur(current->iDog, current->iDog, 9);
//    current->write(3);
    threshold(current->iDog, current->iDog, 70, 255, THRESH_BINARY);
//    current->write(3);
    int dilation_size = 4;
    Mat element = getStructuringElement( 0,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) );
    Mat element2 = getStructuringElement( 0,Size( 4*dilation_size + 1, 4*dilation_size+1 ),Point( 2*dilation_size, 2*dilation_size ) );
    erode(current->iDog, current->iDog, element);
//    current->write(3);
    dilate(current->iDog, current->iDog, element2);
    current->write(3);
//   Canny(current->iDog, current->iTemp, 50, 200, 3);
    current->iDog_ref_n = ref;
    int thresh = 50;
//    Canny(current->iDog, current->iDog, thresh, thresh*2, 3);
    current->write(3);
    
    // 2. fill contour vector and rect array
    
//    current->iTemp = current->iGray;
    double min_area = 2000;
    Size scalefactor(10,10);                                                                // enlarge found rect
    findContours(current->iDog, current->iContours, current->iHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0, 0));
    std::stringstream ss;
    ss << current->get_iName();
    std::cout << "contours: "<<current->iContours.size()<<", nr: "<< ss.str()<< std::endl;
    int k=0;                                                                                // k = iterator through images
    for(int i=0; i<current->iContours.size();i++)
    {
        if( contourArea(current->iContours.at(i))>min_area)                                    // i = iterator through contours
        {
            k++;
            // Create Bounding Boxes
            vector<Point>  contours_poly;
            approxPolyDP( Mat(current->iContours[i]), contours_poly, 3, true );
            Rect  rect = boundingRect(Mat(contours_poly));
            drawContours( current->iTemp, vector<vector<Point> >(1,current->iContours.at(i)), 0, Scalar(255,0,255), 1, 8);
            rectangle( current->iTemp, rect.tl(), rect.br(), Scalar(255,0,255), 1, 8, 0 );
            current->write(4);
            
            bool rect_ok = true;
            if(rect.br().x>dim.width || rect.br().y > dim.height)
            {
                std::cout<< "Rect too big"<<std::endl;
                rect_ok = false;
            }
            auto size = potential_aois.size();
            if (size==0 && rect_ok)
            {
                potential_aois.push_back(rect);       // push only first rect automatically
            }
//            size = potential_aois.size();
            
            if(size!=0 && rect_ok)
            {
                vector<Rect>::iterator iterator=potential_aois.begin();
                bool replace_rect = false;
                while(iterator!=potential_aois.end() && !replace_rect)
                {
                    Rect new_rect;
                    merge_rects(rect, *iterator, new_rect,replace_rect);
                    if(replace_rect)                                // found similar rect: replace it and restart while loop
                    {
                        potential_aois.erase(iterator);
                        potential_aois.insert(iterator,new_rect);
                        // possible recursive loop?????
                    }
                    iterator++;
                }
                if(!replace_rect)               // if after iterating through potential_aois no rect is similar
                {
                    potential_aois.push_back(rect);
                }
            }
            
            
//            for(int j=0; j<size;j++)
//            {
//                if(rect_ok)
//                {   Rect new_rect;
//                    bool replace_rect;
//                    merge_rects(rect, potential_aois.at(j),new_rect,replace_rect);            // merges, if rects overlap and returns merged rect
//                    if(!replace_rect)                         // same already exists, so exchange
//                    {
//                        potential_aois.at(j) = new_rect;      // TODo : delete does not work
//                        rect = new_rect;
////                        j++;
//                    }
//                    else
//                    {
//                    potential_aois.push_back(rect);         // add element
//                    }
//                }
//                // TODO
//            }
            
            
            // Create AOI
//            FWAoi* aoi = new FWAoi(current,rect.tl(),rect.br());
//            if(iAoi_exist_prev(aoi))                                            // except: Aoi is Out Movement
//            {
//                current->delete_aoi(aoi);
//            }
//            else                                                            // merge if rect intersects with existing
//            {
//                FWAoi* new_aoi = iAoi_intersects(current, aoi);
//                if(new_aoi!=aoi)                                            // if new AOI is not aoi
//                {
//                    double area = aoi->get_iRect().area();
//                    current->delete_aoi(aoi);                                  // delete old aoi
//                    Rect new_rect = new_aoi->get_iRect();
//                    current->iBoundingRect.push_back(new_rect);
//                    rectangle( current->iTemp, new_rect.tl(), new_rect.br(), Scalar(255,255,255), 2, 8, 0 );
//                    current->add_aoi(new_aoi);                                 // Add new aoi
//                    //                    std::cout << "---new AOI area = " << new_rect.area() << std::endl;
//                    //                    std::cout << "old area: "<< area <<std::endl;
//                    //                  new_aoi->write(1,k);
//                }
//                else
//                {
//                    current->iBoundingRect.push_back(rect);
//                    rectangle( current->iTemp, rect.tl(), rect.br(), Scalar(255,255,255), 2, 8, 0 );
//                    current->add_aoi(aoi);                                 // Add new aoi
//                }
//            }
        }
        
    }
    for(int i=0;i<potential_aois.size();i++)
    {
        rectangle( current->iGray, potential_aois.at(i).tl(), potential_aois.at(i).br(), Scalar(255,255,10*i), 2, 8, 0 );
        
    }
    potential_aois.clear();
    current->write(1);
//    if (current->iBoundingRect.size()>0) current->write(4);                        // only rewrite Image, if at least one bounding box is added
//    std::cout <<"Number of AOI:"<< current->get_iAoi_vec_size()<<std::endl;
}

void FWMoAn::merge_rects(Rect& one, Rect& two, Rect& new_rect,bool& replace_rect)
{
    Point new_tl;
    Point new_br;
    replace_rect = false;
    if(one.contains(two.tl()) || one.contains(two.br()) || one.contains(Point(two.tl().x+two.width,two.tl().y)) || one.contains(Point(two.br().x-two.width,two.br().y)))
    {
        replace_rect = true;
        // calc new tl
        if(one.tl().x<two.tl().x)           // one is more left
        {
            if(one.tl().y<two.tl().y)       new_tl = one.tl();
            else                            new_tl = Point(one.tl().x,two.tl().y);
        }
        else if (one.tl().y<two.tl().y)     new_tl = Point(two.tl().x,one.tl().y);
        else                                new_tl = two.tl();
        
        //calc new br
        if(one.br().x>two.br().x)           // one is more left
        {
            if(one.br().y>two.br().y)       new_br = one.br();
            else                            new_br = Point(one.br().x,two.br().y);
        }
        else if (one.br().y>two.br().y)     new_br = Point(two.br().x,one.br().y);
        else                                new_br = two.br();
    }
    else
    {
        replace_rect = false;
        new_tl = one.tl();
        new_br = one.br();
    }
    Rect rect(new_tl,new_br);
    new_rect = rect;
    
}

void FWMoAn::calc_contures(int pic_num)
{
    
    // Not needed anymore
//    FWImage* next = iImage_vec.at(pic_num);                             // Image to be subtracted from refImage
//    current->iTemp = current->iGray;
//    double min_area = 3000;
//    findContours(current->iDog, current->iContours, current->iHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0, 0));
//    
//    std::stringstream ss;
//    ss << current->get_iName();
//    std::cout << "contours: "<<current->iContours.size()<<", nr: "<< ss.str()<< std::endl;
//    int k=0;                                                                                // k = iterator through images
//    for(int i=0; i<current->iContours.size();i++)
//    {
//        if( contourArea(current->iContours.at(i))>min_area)                                    // i = iterator through contours
//        {
//            k++;
//            // Create Bounding Boxes
//            vector<Point>  contours_poly;
//            approxPolyDP( Mat(current->iContours[i]), contours_poly, 3, true );
//            Rect  rect = boundingRect(Mat(contours_poly));
//            
//            // Create AOI
//            FWAoi* aoi = new FWAoi(current,rect.tl(),rect.br());
//            if(iAoi_exist_prev(aoi))                                            // except: Aoi is Out Movement
//            {
//                current->delete_aoi(aoi);
//            }
//            else                                                            // merge if rect intersects with existing
//            {
//                FWAoi* new_aoi = iAoi_intersects(current, aoi);
//                if(new_aoi!=aoi)                                            // if new AOI is not aoi
//                {
//                    double area = aoi->get_iRect().area();
//                    current->delete_aoi(aoi);                                  // delete old aoi
//                    Rect new_rect = new_aoi->get_iRect();
//                    current->iBoundingRect.push_back(new_rect);
//                    rectangle( current->iTemp, new_rect.tl(), new_rect.br(), Scalar(255,255,255), 2, 8, 0 );
//                    current->add_aoi(new_aoi);                                 // Add new aoi
////                    std::cout << "---new AOI area = " << new_rect.area() << std::endl;
////                    std::cout << "old area: "<< area <<std::endl;
////                  new_aoi->write(1,k);
//                }
//                else
//                {
//                    current->iBoundingRect.push_back(rect);
//                    rectangle( current->iTemp, rect.tl(), rect.br(), Scalar(255,255,255), 2, 8, 0 );
//                    current->add_aoi(aoi);                                 // Add new aoi
//                }
//            }            
//        }// end of contour iterator
//    }
//    if (current->iBoundingRect.size()>0) current->write(4);                        // only rewrite Image, if at least one bounding box is added
//    std::cout <<"Number of AOI:"<< current->get_iAoi_vec_size()<<std::endl;
}


