#ifndef IMAGE_IMPORT_H
#define IMAGE_IMPORT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

struct Pixel{
    float r,g,b;
};

class Image{
private:

public:
    Image(std::string& file);
    int width,height;
    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width));

};

//Color filters
void bf(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void redfilter(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void greenfilter(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void bf(std::ifstream& ifile,std::ofstream& ofile);

//Image transformation
void invert(std::ifstream& ifile,std::ofstream& ofile);
void threshold(std::ifstream& ifile,std::ofstream& ofile,double& intensity);
void blur(std::ifstream& ifile,std::ofstream& ofile,double& intensity);

#endif