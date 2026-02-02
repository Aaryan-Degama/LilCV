#ifndef IMAGE_IMPORT_H
#define IMAGE_IMPORT_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <array>

// ONNX Runtime
#include <onnxruntime/onnxruntime_cxx_api.h>

struct Pixel{
    float r,g,b;
};

class Image{
private:
    int m_width;
    int m_height;
    std::vector<Pixel> pixels; 
public:
    Image(int width, int height);
    
    int width() const;
    int height() const;
    
    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;
};

Image loadPPM(const std::string& ifile);
void savePPM(const Image& img,const std::string& ofile);

// Color filters
const Image bw(const Image& img);

// Blur
const Image blur(const Image& img,double intensity);

// Depth-based blur
Image computeDepthMap(const Image& img);
const Image depthBlur(const Image& img,double intensity);

const Image depthImage(const Image& img);

// Basic transforms
const Image negative(const Image& img);
const Image grayscale(const Image& img);

// Resize
const Image resize(const Image& img, int newW, int newH);

// Depth output only
const Image depthImage(const Image& img);

#endif