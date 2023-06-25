#ifndef COMPILEENGINE_H
#define COMPILEENGINE_H

#include"tools.h"
#include"JackTokenizer.h"
#include"symbolTable.h"
#include"codeWriter.h"

class CompileEngine{
    public:
        CompileEngine(const std::string &inname);
        void compileClass();
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void compileSubroutineBody(FUNCTION_TYPE);
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
        void process(TOKENTYPE);
        void process(const std::string&);
        void process(std::set<std::string>);
        bool processVar(const std::string &varName, size_t& index, std::string& kind, std::string& type);
        void defineVarDec(const std::string &type, VARIABLE_KIND kind);
        std::string processType();
        std::string getTokenType(Token token) const;
        std::string getCurrTokenStr() const;
        bool advance();
        Token currToken;
        JackTokenizer tokenizer; 
        CodeWriter writer; 
        SymbolTable classVarTable;
        SymbolTable subrountineVarTable;
        std::string className;  std::string functionName;
        size_t ifCount, whileCount;
};

#endif