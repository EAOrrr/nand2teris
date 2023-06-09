#include"tool.h"
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<sstream>

int main(int argc, char**argv){
    if(false){
        /*debug section*/
        // auto print = [](std::vector<std::string> vec){
        //     for(auto& elem:vec)
        //         std::cout << elem << "***";
        //     std::cout << std::endl;
        // };
       std::ifstream input(argv[1]);
       std::cout << outname(argv[1]) << std::endl;
        if(!input.is_open()){
            std::cerr << "failed to open " << argv[1] << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string currentLine;
        size_t pos;
        while(std::getline(input, currentLine)){
            
        if((pos = currentLine.find("//")) != std::string::npos){
            currentLine.erase(pos);
        }
            // std::cout << currentLine << std::endl;
        Parser parser(currentLine);
        std::cout << parser.getCommandType() <<'*'<< parser.getArg1() << '*' << parser.getArg2() <<std::endl; 
        }
    return 0;
    }
    else{
        if(argc != 2){
            std::cerr << "invalid arguments" << std::endl;
            return EXIT_FAILURE;
        }
        std::string outName = outname(argv[1]);
        CodeWriter codewriter(outName);

        std::ifstream input(argv[1]);
        if(!input.is_open()){
            std::cerr << "failed to open " << argv[1] << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string currentLine;

        while(std::getline(input, currentLine)){
            size_t pos;
            if((pos = currentLine.find("//")) != std::string::npos){
                currentLine.erase(pos);
            }
            Parser parser(currentLine);
            codewriter.writeCode(currentLine, parser);
        }
        codewriter.close();
        return EXIT_SUCCESS;
    }

}