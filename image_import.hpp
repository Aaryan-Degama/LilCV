#ifndef IMAGE_IMPORT_H
#define IMAGE_IMPORT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

class Image{
private:
    std::ifstream
public:
    Image(std::string& file);


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