#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H
#include"tools.h"
std::string outname(std::string inname);
class JackTokenizer{
    public:
        JackTokenizer(std::string inputFilename);
        bool hasMoreLine() const;
        Token advance();
        // Token getToken() const;
    private:
        Token advanceToken();
        void advanceLine();
        std::string line;   size_t index;   // for parsing
        // Token currToken;
        std::ifstream input;
};
#endif