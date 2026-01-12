#include "image_import.hpp"

#include <vector>
#include <cmath>
#include <algorithm>


void blur(std::ifstream& img, std::ofstream& nimg,const double& intensity = -1.0) {
    if(intensity < 0){
        std::cerr<<"The intensity of the blur is not being entered or is not a valid value,\nUsage:\n\t--intensity <intensity value>";
    }

    std::string type;
    int width, height, maxRGB;
    if (!(img >> type >> width >> height >> maxRGB)) {
        std::cout << "Unable to Extract Information\n";
        return;
    }
    int size = std::ceil(intensity * 6);
    if (size % 2 == 0) size++;
    if (size < 3) size = 3;
    int half = size / 2;

    std::vector<float> kernel1D(size);
    float sum = 0.0f;
    for (int i = -half; i <= half; i++) {
        float val = exp(-(i * i) / (2.0f * intensity * intensity));
        kernel1D[i + half] = val;
        sum += val;
    }
    for (float& w : kernel1D) w /= sum;

    struct Pixel { float r, g, b; };
    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r, g, b;
            img >> r >> g >> b;
            pixels[y][x] = {(float)r, (float)g, (float)b};
        }
    }

    std::vector<std::vector<Pixel>> temp = pixels;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float r = 0, g = 0, b = 0;
            for (int k = -half; k <= half; k++) {
                int ix = std::clamp(x + k, 0, width - 1);
                r += pixels[y][ix].r * kernel1D[k + half];
                g += pixels[y][ix].g * kernel1D[k + half];
                b += pixels[y][ix].b * kernel1D[k + half];
            }
            temp[y][x] = {r, g, b};
        }
    }

    nimg << type << "\n" << width << " " << height << "\n" << maxRGB << "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float r = 0, g = 0, b = 0;
            for (int k = -half; k <= half; k++) {
                int iy = std::clamp(y + k, 0, height - 1);
                r += temp[iy][x].r * kernel1D[k + half];
                g += temp[iy][x].g * kernel1D[k + half];
                b += temp[iy][x].b * kernel1D[k + half];
            }
            nimg << (int)r << " " << (int)g << " " << (int)b << "\n";
        }
    }

    img.close();
    nimg.close();
}



void bw(std::ifstream& img,std::ofstream& nimg){
       
    std::string type,width,height,maxRGB;
    img>>type>>width>>height>>maxRGB;

    nimg<<type<<'\n'<<width<<" "<<height<<'\n'<<maxRGB<<'\n';

    int r,g,b;
    while(img>>r>>g>>b){
    int a{(r+b+g)/3};
    nimg<<a<<" "<<a<<" "<<a<<'\n';}
    img.close();
    nimg.close();
}


void bf(std::ifstream& img,std::ofstream& nimg,const double& intensity = -1){
    if(intensity < 0){
        std::cerr<<"The intensity of the blur is not being entered or is not a valid value,\nUsage:\n\t--intensity <intensity value>";
    }

    std::string type,width,height,maxRGB;
    img>>type>>width>>height>>maxRGB;

    nimg<<type<<'\n'<<width<<" "<<height<<'\n'<<maxRGB<<'\n';

    int r,g,b;
    while(img>>r>>g>>b){
        b+=intensity;
        if(b>255)
            b=255;
        nimg<<r<<" "<<g<<" "<<b<<'\n';
    }

    img.close();
    nimg.close();
}



