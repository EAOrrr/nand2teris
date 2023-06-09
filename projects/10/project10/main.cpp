#include"tools.h"

int main(int argc, char** argv){
    // JackTokenizer jt{"/mnt/d/practice/h_programming_basis/nand2tetris/nand2tetris/projects/10/ArrayTest/Main.jack"};
    JackTokenizer jt{argv[1]};
    std::ofstream output{"Main.xml"};
    output << "<tokens>" << std::endl;
    while(jt.hasMoreLine()){
        Token currToken = jt.advance();
        if(currToken.first!=TOKENTYPE::BLANK){
            output << "<" << JackTokenizer::getType(currToken) << "> ";
            output << currToken.second;
            output << "</" << JackTokenizer::getType(currToken) << '>' << std::endl;
            // std::cout << "<" << JackTokenizer::getType(currToken) << "> ";
            // std::cout << currToken.second;
            // std::cout << "</" << JackTokenizer::getType(currToken) << '>' << std::endl;
        }
    }
    output << "</tokens>" << std::endl;
    return 0;
}