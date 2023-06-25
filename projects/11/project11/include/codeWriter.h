#ifndef CODEWRITER_H
#define CODEWRITER_H
#include"tools.h"
std::string outname(std::string inname);
class CodeWriter{
    public:
        CodeWriter(std::string);
        void writePush(std::string, size_t);
        void writePop(std::string, size_t);
        void writeArithmetic(char);
        void writeLabel(std::string);
        void writeGoto(std::string);
        void writeIf(std::string);
        void writeCall(std::string, size_t);
        void writeFunction(std::string, size_t);
        void writeReturn();
    private:
        std::ofstream output; 
};
#endif
