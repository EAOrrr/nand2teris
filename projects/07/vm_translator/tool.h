#ifndef TOOL_H
#define TOOL_H
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<cctype>
#include<cstdlib>
enum C_TYPE{C_ARITHMETIC, C_PUSH, C_POP,
         C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, EMPTY};
std::string outname(std::string input);

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
        void close();
    private:
        std::ofstream output;
        std::string filename;
        void writeArithemic(const std::string& line);
        void writePush(const std::string& segment, int pos);
        void writePop(const std::string& segment, int pos);
        void addrPush(const std::string& segment, char obj, int index);
        void addrPop(const std::string& segment, char obj, int index);
        void BinaryArithemic(char operators);
        void UnaryArithemic(char oper);
        int CompareArothemic(const std::string& comp, int time);
};


#endif