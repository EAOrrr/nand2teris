#include"tools.h"
std::string outname(std::string inname){
    size_t pos = inname.find(".jack");
    if(pos == std::string::npos){
        throw std::logic_error("Invalid input file");
    }
    return inname.replace(pos, 5, "Eao.xml");
}

CompileEngine::CompileEngine(std::string inname):tokenizer(inname), output(outname(inname)){};

void CompileEngine::printXMLtoken(Token token){
    output << "<" << getTokenType(token) << "> ";
    if(token.first == TOKENTYPE::SYMBOL && std::string{"<>&"}.find(token.second) != std::string::npos){
        switch(token.second[0]){ // special symbol
            case '&':
                output << "&amp;";
                break;
            case '<':
                output << "&lt;";
                break;
            case '>':
                output <<  "&gt;";
                break;
            default:
                std::cerr << "error" << std::endl;
        }
    }
    output << token.second;
    output << " </" << getTokenType(token) << '>' << std::endl;
}

std::string CompileEngine::getTokenType(Token token) const{
    switch(token.first){
        case(TOKENTYPE::INDENTIFER):
            return "identifier";
        case(TOKENTYPE::KEYWORD):
            return "keyword";
        case(TOKENTYPE::SYMBOL):
            return "symbol";
        case(TOKENTYPE::INT_CONST):
            return "integerConstant";
        case(TOKENTYPE::STRING_CONST):
            return "stringConstant";
        default:
            throw std::logic_error("Invalid getType BLANK"); 
    }
}

std::string CompileEngine::getCurrTokenStr() const{
    return currToken.second;
}

bool CompileEngine::advance(){
    currToken = tokenizer.advance();
    if(currToken.first == TOKENTYPE::BLANK){
        return false;
    }
    return true;
}
void CompileEngine::run(){
    output << "<tokens>" << std::endl;
    while(advance()){
        printXMLtoken(currToken);
    }
    output << "</tokens>" << std::endl;
}

void CompileEngine::process(TOKENTYPE tokenType){
    if(currToken.first == tokenType){
        printXMLtoken(currToken);
    }
    else{
        std::cerr << "Expected type: " << getTokenType({tokenType, ""});
        std::cerr << " but get " << getTokenType(currToken) << " with token " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}

void CompileEngine::process(std::string str){
    if(getCurrTokenStr() == str){
        printXMLtoken(currToken);
    }
    else{
        std::cerr << "Expected specifier: " << str << " but get " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}

void CompileEngine::process(std::set<std::string> strs){
    if(strs.count(getCurrTokenStr())){
        printXMLtoken(currToken);
    }
    else{
        std::cerr << "Invalid specifier " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}
void CompileEngine::processType(){
    if(currToken.first == TOKENTYPE::INDENTIFER){
        printXMLtoken(currToken);
    }
    else if(currToken.first == TOKENTYPE::KEYWORD){
        if(getCurrTokenStr() == "int" ||
         getCurrTokenStr() == "char" || currToken.second == "boolean"){
            printXMLtoken(currToken);
        }
        else{
            std::cerr << "Invalid Type: " << getCurrTokenStr() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else{
        std::cerr << "Invalid Type: " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}

/* Class Structure: 'class' className(identifier) '{' classVarDec* subroutineDec* '}'*/
void CompileEngine::compileClass(){
    output << "<class>" << std::endl;
    advance();
    process("class");   // 'class'
    process(TOKENTYPE::INDENTIFER); // className(identifier)
    process("{");   // '{'
    while(getCurrTokenStr() == "static" || currToken.second == "field"){ // classVarDec
        compileClassVarDec();
    }
    while(getCurrTokenStr() == "constructor" || currToken.second == "function" || currToken.second == "method"){ // subroutineDec
        compileSubroutine();
    }
    process("}");
    output << "</class>" << std::endl;
}

/* ClassVarDec structure: {'static'|'field'} type varName(identifier) (, varName)* ; */
void CompileEngine::compileClassVarDec(){
    output << "<classVarDec>" << std::endl;

    /* static | field */
    // if(getCurrTokenStr() == "static") process("static");     
    // else if(getCurrTokenStr() == "field") process("field");
    // else{
    //     std::cerr << "Invalid identifier: " << getCurrTokenStr() << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    process(std::set<std::string>{"static", "field"});

    /* type */
    processType();
    /* varName(identifier) */
    process(TOKENTYPE::INDENTIFER);

    /* (, varName) */
    while(getCurrTokenStr() == ","){
        process(",");
        process(TOKENTYPE::INDENTIFER);
    }

    /* ';' */
    process(";");
    output << "</classVarDec>" << std::endl;

}

/* subroutine: (constructor| function|method) (void|type)
     subroutineName ( parameterList ) subroutineBody */
void CompileEngine::compileSubroutine(){
    output << "<subroutineDec>" << std::endl;
    /* constructor | function | method */
    if(getCurrTokenStr() == "constructor"){
        process("constuctor");
    }else if(getCurrTokenStr() == "function"){
        process("function");
    }else if(getCurrTokenStr() == "method"){
        process("method");
    }else{
        std::cerr << "Invalid identifier: " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }

    /* void | type */
    if(getCurrTokenStr() == "void") process("void");
    else processType();

    /* subroutineName(identifier) */
    process(TOKENTYPE::INDENTIFER);

    /*( parameterList )*/
    process("(");
    compileParameterList();
    process(")");

    /* subroutineBody */
    compileSubroutineBody();
    output << "</subroutineDec>" << std::endl;
}

/* parameterList: (empty) or type varName (, type varName)* */
void CompileEngine::compileParameterList(){
    output << "<parameterList>" << std::endl;
    if(getCurrTokenStr() != ")"){    // not empty
        /* type varName */
        processType();
        process(TOKENTYPE::INDENTIFER);
        /* (, type varName)* */
        while(getCurrTokenStr() == ","){
            process(",");
            processType();
            process(TOKENTYPE::INDENTIFER);
        }
    }
    output << "</parameterList>" << std::endl;
}

/* varDec: var type varName (, varName)* */
void CompileEngine::compileVarDec(){
    output << "<varDec>" << std::endl;
    process("var");
    processType();
    process(TOKENTYPE::INDENTIFER);
    while(getCurrTokenStr() == ","){
        process(",");
        process(TOKENTYPE::INDENTIFER);
    } 
    process(";");
    output << "</varDec>" << std::endl;
}

/* subroutineBody: { varDec* statements }*/
void CompileEngine::compileSubroutineBody(){
    output << "<subroutineBody>" << std::endl;
    process("{");
    while(getCurrTokenStr() == "var"){
        compileVarDec();
    }
    compileStatements();
    process("}");
    output << "</subroutineBody>" << std::endl;
}

/* Statements: statement*
    i.e letStament | ifStatement | whileStatement | doStatement | returnStatement */
void CompileEngine::compileStatements(){
    output << "<statements>" << std::endl;
    while(getCurrTokenStr() == "let" ||getCurrTokenStr() == "do" ||getCurrTokenStr() == "if"
        || getCurrTokenStr() == "while" ||getCurrTokenStr() == "return"){
        if(getCurrTokenStr() == "let"){
            compileLet();
        }
        else if(getCurrTokenStr() == "do"){
            compileDo();
        }
        else if(getCurrTokenStr() == "if"){
            compileIf();
        }
        else if(getCurrTokenStr() == "while"){
            compileWhile();
        }
        else if(getCurrTokenStr() == "return"){
            compileReturn();
        }
    }
    if(getCurrTokenStr() != "}"){
        std::cerr << "Expected statement but get " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    output << "</statements>" << std::endl;
}

/* ifStatement: if ( expr ) { statements } (else { statements } )? */
void CompileEngine::compileIf(){
    output << "<ifStatement>" << std::endl;
    process("if");
    process("(");
    compileExpression();
    process(")");
    process("{");
    compileStatements();
    process("}");
    if(getCurrTokenStr() == "else"){
        process("else");
        process("{");
        compileStatements();
        process("}");
    }
    output << "</ifStatement>" << std::endl;
}

/*letStatement: let varName ([expr])? = expr ;*/
void CompileEngine::compileLet(){
    output << "<letStatement>" << std::endl;
    process("let");
    process(TOKENTYPE::INDENTIFER);
    if(getCurrTokenStr() == "["){
        process("[");
        compileExpression();
        process("]");
    }
    process("=");
    compileExpression();
    process(";");
    output << "</letStatement>" << std::endl;
}

/* whileStaement: while ( expr ) { statements }*/
void CompileEngine::compileWhile(){
    output << "<whileStatement>" << std::endl;
    process("while");
    process("(");
    compileExpression();
    process(")");
    process("{");
    compileStatements();
    process("}");
    output << "</whileStatement>" << std::endl;
}

/* returnStatement: return expr? ;*/
void CompileEngine::compileReturn(){
    output << "<returnStatement>" << std::endl;
    process("return");
    if(getCurrTokenStr() != ";") compileExpression();
    process(";");
    output << "</returnStatement>" << std::endl;
}

/*doStatement: do subroutineCall*/
void CompileEngine::compileDo(){
    output << "<doStatement>" << std::endl;
    process("do");
    // subroutineCall
    process(";");
    output << "</doStatement>" << std::endl;
}

/* compileExpression*/
void CompileEngine::compileExpression(){
    process(TOKENTYPE::INDENTIFER);
}