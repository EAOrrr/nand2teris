#ifndef CODEWRITER_H
#define CODEWRITER_H
#include"tools.h"
std::string outname(std::string inname);
class CodeWriter{
    public:
        CodeWriter(const std::string&);
        void writePush(const std::string&, size_t);
        void writePop(const std::string&, size_t);
        void writeArithmetic(char);
        void writeLabel(const std::string&);
        void writeGoto(const std::string&);
        void writeIf(const std::string&);
        void writeCall(const std::string&, size_t);
        void writeFunction(const std::string&, size_t);
        void writeReturn();
    private:
        std::ofstream output; 
};
#endif
