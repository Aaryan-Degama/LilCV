#include <iostream>
#include <string>
#include <unordered_map>
#include<functional>

#include "image_import.hpp"


#include <CLI/CLI.hpp>

int main(int argc, char* argv[]){
    CLI::App app{"LilCV"};
    std::string ifile,ofile;
    std::string cmd;
    double intensity{1};
    int size{1};

    int resize_w = 256;
    int resize_h = 256;

    app.add_option(
        "Input_file",
        ifile,
        "Input file"
    )->required();

    app.add_option(
        "Output_file",
        ofile,
        "Output file"
    )->required();

    app.add_option(
        "command",
        cmd,
        "Command to apply"
    )->required();

    app.add_option(
        "--intensity",
        intensity,
        "Filter intensity (used by some commands)"
    );
    app.add_option(
    "--width",
    resize_w,
    "Resize width (default: 256)"
    );

    app.add_option(
        "--height",
        resize_h,
        "Resize height (default: 256)"
    );
    

    CLI11_PARSE(app,argc,argv);

    Image real_image = loadPPM(ifile);

 

    std::unordered_map<std::string,std::function<Image()>> cmd_map = {
        {"bw",          [&](){ return bw(real_image); }},
        {"1",           [&](){ return bw(real_image); }},
        {"blur",        [&](){ return blur(real_image,intensity); }},
        {"depthblur",   [&](){ return depthBlur(real_image,intensity); }},
        {"depth",       [&](){ return depthImage(real_image); }},
        {"negative",    [&](){ return negative(real_image); }},
        {"resize",      [&](){ return resize(real_image, resize_w, resize_h); }},
    };

    auto it = cmd_map.find(cmd);
    if(it != cmd_map.end()){
        const Image output = it->second();
        savePPM(output,ofile);
    }else{
        std::cerr <<"Unknow command: "<< cmd<<'\n';
        return 1;
    }


    return 0;
}