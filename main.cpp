#include <iostream>
#include <string>
#include <unordered_map>
#include<functional>

#include "image_import.hpp"


#include <CLI/CLI.hpp>

int main(int argc, char* argv[]){
    CLI::App app{"LilCV"};
    std::string iofile,oofile;
    std::string cmd;
    double intensity{1};
    int size{1};

    app.add_option(
        "Input_file",
        iofile,
        "Input file"
    )->required();

    app.add_option(
        "Output_file",
        oofile,
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

    std::ifstream ifile(iofile);
    std::ofstream ofile(oofile);

    if(!ifile.is_open()){
        std::cerr<<"Unable to open input file\n";
        return 1;
    }
    if(!ofile.is_open()){
        std::cerr<<"Unable to open output file\n";
    }

    std::unordered_map<std::string,std::function<void()>> cmd_map = {
        {"bf",      [&](){ bf(ifile, ofile,intensity); }},
        {"1",       [&](){ bf(ifile, ofile,intensity); }},
        {"blur",    [&](){ blur(ifile,ofile,intensity); }},
        // {"rf",   [&](){ redfilter(ifile,ofile,intensity); }},
        // {"2",           [&](){ redfilter(ifile,ofile,intensity); }},
        // {"rf", [&](){ greenfilter(ifile,ofile,intensity); }},
        // {"3",           [&](){ greenfilter(ifile,ofile,intensity); }},
        // {"in",      [&](){invert(ifile,ofile);}},
        // {"",           [&](){invert(ifile,ofile);}}
    };

    auto it = cmd_map.find(cmd);
    if(it != cmd_map.end()){
        it->second();
    }else{
        std::cerr <<"Unknow command: "<< cmd<<'\n';
        return 1;
    }


    return 0;
}