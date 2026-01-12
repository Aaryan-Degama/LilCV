#include "image_import.hpp"


// In image_import.cpp
Image::Image(int width, int height) 
    : m_width(width), m_height(height), pixels(width * height) {
}

int Image::width() const{
    return m_width;
}

int Image::height() const{
    return m_height;
}


Pixel& Image::at(int x, int y){
    if(x<0 || x>=m_width || y<0 ||y>=m_height)
        throw std::out_of_range("Image::at() index out of range");
    return pixels[y*m_width+x];
}

const Pixel& Image::at(int x,int y) const{
    if(x<0 || x>=m_width || y<0 ||y>=m_height)
        throw std::out_of_range("Image::at() index out of range");
    return pixels[y*m_width+x];
}


//This functions loads the data of the image
Image loadPPM(const std::string& ifile){
    std::ifstream limg(ifile); // load image == limg
    if(!limg.is_open()){ //Checks if the file was opened 
        throw std::runtime_error("Failed to open loading image");
    }
    std::string type;
    limg>>type;

    if (type != "P3") { //checks if the type is correct
        throw std::runtime_error("Only P3 PPM supported");
    }
    
    int width,height;
    int maxRGB;

    if(limg>>width>>height>>maxRGB){
        if(maxRGB != 255 ){ //Checks if the max RGB is correct or not
            throw std::runtime_error("Only max RGB = 255 are supported");
        }
        Image img(width,height);
        for(int y = 0; y<height; y++){
            for (int x = 0; x<width; x++){
                Pixel& p = img.at(x,y) ;
                limg>> p.r>> p.g>>p.b;
            }
        }
        return img;
    }else{
        throw std::runtime_error("File was'nt able to be extracted");
    }
}


void savePPM(const Image& img, const std::string& ofile){
    std::ofstream simg(ofile); //save image == simg
    if(!simg.is_open())
        throw std::runtime_error("Failed to open saving image");

    simg<<"P3\n"<<img.width()<<" "<<img.height()<<"\n255\n";

    for(int y = 0; y<img.height(); y++){
        for(int x = 0; x<img.width(); x++){
            const Pixel& p = img.at(x,y);
            simg<<static_cast<int>(p.r)
            <<" "<<static_cast<int>(p.g)
            <<" "<<static_cast<int>(p.b)<<" ";
        }
        simg<<'\n';
    }
}


const Image bw(const Image& img){

    Image output(img.width(),img.height());

    for(int y = 0; y<img.height(); y++){
        for(int x = 0; x<img.width(); x++){
            const Pixel& real_pixel = img.at(x,y);
            Pixel& new_pixel = output.at(x,y);
            float a{(real_pixel.r+real_pixel.g+real_pixel.b)/3};
            new_pixel.r = a;
            new_pixel.g = a;
            new_pixel.b = a;
        }
    }
    return output;
}

const Image blur(const Image& img,double intensity){

    intensity = std::max(intensity, 0.5);

    Image output(img.width(),img.height());
    int size = static_cast<int>(std::ceil(intensity * 6)) | 1;
    size = std::max(size, 3);

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

    Image temp(img.width(), img.height());

    // Horizontal Pass
    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float r = 0, g = 0, b = 0; // Accumulators
            for (int i = -half; i <= half; i++) {
                int ix = std::clamp(x + i, 0, img.width() - 1);
                const Pixel& p = img.at(ix, y);
                r += p.r * kernel1D[i + half];
                g += p.g * kernel1D[i + half];
                b += p.b * kernel1D[i + half];
            }
            Pixel& outP = temp.at(x, y);
            outP.r = r; outP.g = g; outP.b = b;
        }
    }

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            float r = 0, g = 0, b = 0; // Accumulators
            for (int i = -half; i <= half; i++) {
                int iy = std::clamp(y + i, 0, img.height() - 1); // Fixed: height
                const Pixel& p = temp.at(x, iy); // Fixed: read from temp
                r += p.r * kernel1D[i + half];
                g += p.g * kernel1D[i + half];
                b += p.b * kernel1D[i + half];
            }
            Pixel& outP = output.at(x, y);
            outP.r = r; outP.g = g; outP.b = b;
        }
    }

    return output;
}

