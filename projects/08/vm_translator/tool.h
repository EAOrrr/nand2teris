#ifndef TOOL_H
#define TOOL_H
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<cctype>
#include<stack>
#include<cstdlib>
#include<filesystem>
enum C_TYPE{C_ARITHMETIC, C_PUSH, C_POP,
         C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, EMPTY};
class CodeWriter;    
void readFile(const std::string& filename, CodeWriter& codewriter);
class Parser{
    public:
        Parser(const std::string& line);
        C_TYPE analyseType(const std::string& command)const;
        std::string getArg1() const;
        int getArg2() const;
        C_TYPE getCommandType() const;
    private:
        C_TYPE commandType;
        std::string arg1;
        int arg2;
};

class CodeWriter{
    public:
        CodeWriter(std::string outname);
        void writeCode(const std::string& line, const Parser& Parser);
        void setFilename(const std::string& file);
        void writeSysInit();
        void close();
    private:
        std::stack<std::string> funcs;
        std::ofstream output;
        std::string filename;
        std::string renameLabel(std::string label);
        std::string outPutName(std::string inname);
        std::string getRet();
        void writeReturn();
        void writeCall(const std::string& funcname, int nArgs);
        void writeFunction(const std::string& funcname, int nVals);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeArithemic(const std::string& line);
        void writePush(const std::string& segment, int pos);
        void writePop(const std::string& segment, int pos);
        void addrPush(const std::string& segment, char obj, int index);
        void addrPop(const std::string& segment, char obj, int index);
        void BinaryArithemic(char operators);
        void UnaryArithemic(char oper);
        int CompareArothemic(const std::string& comp, int time);
        void callHelper(const std::string& pointer);
        void returnHelper(const std::string& pointer, int index);
};


#endif