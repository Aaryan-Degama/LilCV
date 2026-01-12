#ifndef IMAGE_IMPORT_H
#define IMAGE_IMPORT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>

struct Pixel{
    float r,g,b;
};

class Image{
private:
    int m_width;
    int m_height;
    std::vector<Pixel> pixels; 
public:
    // Image(int width,int height)// In image_import.cpp
    Image(int width, int height);
    
    int width() const;
    int height() const;
    
    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;
};

Image loadPPM(const std::string& ifile);
void savePPM(const Image& img,const std::string& ofile);



//Color filters
const Image bw(const Image& img);
void redfilter(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void greenfilter(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void bf(std::ifstream& ifile,std::ofstream& ofile);

//Image transformation
void invert(std::ifstream& ifile,std::ofstream& ofile);
void threshold(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
const Image blur(const Image& img,double intensity);

#endif