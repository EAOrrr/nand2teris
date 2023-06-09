#include"tool.h"
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<filesystem>

int main(int argc, char**argv){
    // check if there are 2 arguments on command line
    if(argc != 2){
        std::cerr << "invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }
    // bind inname to the argv[1]
    std::string inname{argv[1]};
    // if the filename contains .vm, read a single file
    if(inname.find(".vm") != std::string::npos){
        // init codewriter
        CodeWriter codewriter(argv[1]);
        // read the line
        readFile(inname, codewriter);
        // close ifstream and add end loop
        codewriter.close();
    }
    else{
        // the filname is a dir
        CodeWriter codewriter(argv[1]);
        // write call sys.init
        codewriter.writeSysInit();
        std::string path = argv[1];
        for(const auto& entry: std::filesystem::directory_iterator(path)){
            std::string filename = entry.path().string();
            size_t pos = filename.find(".vm");
            if(pos == std::string::npos) continue;  // it is not a vm file
            else{
                readFile(filename, codewriter);
            }
        }
        // close ifstream and add end loop
        codewriter.close();            
    }
    
    return EXIT_SUCCESS;


}


        // codewriter.setFilename(inname);
        // std::ifstream input(argv[1]);
        // if(!input.is_open()){
        //     std::cerr << "failed to open " << argv[1] << std::endl;
        //     std::exit(EXIT_FAILURE);
        // }

        // std::string currentLine;

        // while(std::getline(input, currentLine)){
        //     size_t pos;
        //     // remove comment   
        //     if((pos = currentLine.find("//")) != std::string::npos){
        //         currentLine.erase(pos);
        //     }
        //     Parser parser(currentLine);
        //     codewriter.writeCode(currentLine, parser);
        // }

        // codewriter.setFilename(filename);
                // std::ifstream input(filename);
                // if(!input.is_open()){
                //     std::cerr << "failed to open " << argv[1] << std::endl;
                //     std::exit(EXIT_FAILURE);
                // }
                // std::string currentLine{};
                // while(std::getline(input, currentLine)){
                //     size_t pos;
                //     // remove comment   
                //     if((pos = currentLine.find("//")) != std::string::npos){
                //         currentLine.erase(pos);
                //     }
                //     Parser parser(currentLine);
                //     codewriter.writeCode(currentLine, parser);
                // }