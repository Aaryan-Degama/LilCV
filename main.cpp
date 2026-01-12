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

    

    CLI11_PARSE(app,argc,argv);

    Image real_image = loadPPM(ifile);

    

 

    std::unordered_map<std::string,std::function<Image()>> cmd_map = {
        {"bw",      [&](){ return bw(real_image); }},
        {"1",       [&](){ return bw(real_image); }},
        {"blur",    [&](){ return blur(real_image,intensity); }},
        // {"rf",      [&](){ redfilter(ifile,ofile,intensity); }},
        // {"2",           [&](){ redfilter(ifile,ofile,intensity); }},
        // {"rf", [&](){ greenfilter(ifile,ofile,intensity); }},
        // {"3",           [&](){ greenfilter(ifile,ofile,intensity); }},
        // {"in",      [&](){invert(ifile,ofile);}},
        // {"",           [&](){invert(ifile,ofile);}}
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