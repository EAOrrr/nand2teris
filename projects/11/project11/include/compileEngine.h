#ifndef COMPILEENGINE_H
#define COMPILEENGINE_H

#include"tools.h"
#include"JackTokenizer.h"
#include"symbolTable.h"

class CompileEngine{
    public:
        CompileEngine(std::string inname);
        void compileClass();
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void compileSubroutineBody();
        void compileVarDec();
        void compileStatements();
        void compileLet();
        void compileIf();
        void compileWhile();
        void compileDo();
        void compileReturn();
        void compileExpression();
        void compileTerm();
        int compileExpressionList();
    private:
        void printXMLtoken(Token token);
        void process(TOKENTYPE);
        void process(std::string);
        void process(std::set<std::string>);
        bool processVar(std::string);
        void defineVarDec(std::string type, VARIABLE_KIND kind);
        std::string processType();
        std::string getTokenType(Token token) const;
        std::string getCurrTokenStr() const;
        bool advance();
        Token currToken;
        JackTokenizer tokenizer; 
        symbolTable classVarTable;
        symbolTable subrountineVarTable;
        std::string className;
        std::ofstream output;
};

#endif