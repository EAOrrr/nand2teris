#include"codeWriter.h"
std::string outname(std::string inname){
    size_t pos = inname.find(".jack");
    if(pos == std::string::npos){
        throw std::runtime_error("Invalid input filename ");
    }
    return inname.replace(pos, 5, ".vm");
}

CodeWriter::CodeWriter(const std::string& inname):output(outname(inname)){}

void CodeWriter::writePush(const std::string& segment, size_t index){
    output << "push" << " " << segment << " " << index << std::endl;
}

void CodeWriter::writePop(const std::string &segment, size_t index){
    output << "pop" << " " << segment << " " << index << std::endl;
}

void CodeWriter::writeLabel(const std::string &label){
    output << "label " << label << std::endl;
}

void CodeWriter::writeGoto(const std::string &label){
    output << "goto " << label <<std::endl;
}

void CodeWriter::writeIf(const std::string &label){
    output << "if-goto " << label << std::endl;
}

void CodeWriter::writeCall(const std::string &funcName, size_t nArgs){
    output << "call " << funcName << " " << nArgs << std::endl;
}

void CodeWriter::writeFunction(const std::string &funcName, size_t nVars){
    output << "function " << funcName << " " << nVars << std::endl;
}

void CodeWriter::writeReturn(){
    output << "return" << std::endl;
}

void CodeWriter::writeArithmetic(char ch){
    switch(ch){
        case '+':
            output << "add" << std::endl;
            break;
        case '-':
            output << "sub" << std::endl;
            break;
        case '~':
            output << "not" << std::endl;
            break;
        case '<':
            output << "lt" << std::endl;
            break;
        case '>':
            output << "gt" << std::endl;
            break;
        case '&':
            output << "and" << std::endl;
            break;
        case '|':
            output << "or" << std::endl;
            break;
        case '=':
            output << "eq" << std::endl;
            break;
        case '_':
            output << "neg" << std::endl;
            break;
        case '*':
            writeCall("Math.multiply", 2);
            break;
        case '/':
            writeCall("Math.divide", 2);
            break;
        default:
            throw std::runtime_error("unknown op: " + std::string{ch});
    }
}
