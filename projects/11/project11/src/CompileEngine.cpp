#include"compileEngine.h"
std::string outname(std::string inname){
    size_t pos = inname.find(".jack");
    if(pos == std::string::npos){
        throw std::logic_error("Invalid input file");
    }
    return inname.replace(pos, 5, ".xml");
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
    }else{
        output << token.second;
    }
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
std::string CompileEngine::processType(){
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
    std::string type = currToken.second;
    advance();
    return type;
}

bool CompileEngine::processVar(std::string name){
    if(currToken.first == TOKENTYPE::INDENTIFER){
        
        std::string name = currToken.second;
        if(subrountineVarTable.contain(name)){
            output << "<varName>" << std::endl;
            switch(subrountineVarTable.kindOf(name)){
                case VARIABLE_KIND::ARG:
                    output << "arg ";
                    break;
                case VARIABLE_KIND::VAR:
                    output << "local ";
                    break;
                default:
                    break;
            }
            output << subrountineVarTable.typeOf(name) << " " << name << " " << subrountineVarTable.indexOf(name) << std::endl;
        }else if(classVarTable.contain(name)){
            output << "<varName>" << std::endl;
            switch (classVarTable.kindOf(name)){
            case VARIABLE_KIND::STATIC:
                output << "static ";
                break;
            case VARIABLE_KIND::FIELD :
                output << "field ";
                break;
            default:
                break;
            }
            output << classVarTable.typeOf(name) << " " << name << " " << classVarTable.indexOf(name) << std::endl;
        }
        else{
            return false;
        }
        output << "</varName>" << std::endl;
        advance();
        return true;
    }else{
        return false;
    }
}

void CompileEngine::defineVarDec(std::string type, VARIABLE_KIND kind){
    if(kind == VARIABLE_KIND::ARG || kind == VARIABLE_KIND::VAR){
        subrountineVarTable.define(getCurrTokenStr(), type, kind);
    }else if(kind == VARIABLE_KIND::FIELD || kind == VARIABLE_KIND::STATIC){
        classVarTable.define(getCurrTokenStr(), type, kind);
    }else{
        std::cerr << "unknown kind of variable " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    process(TOKENTYPE::INDENTIFER);
}

/* Class Structure: 'class' className(identifier) '{' classVarDec* subroutineDec* '}'*/
void CompileEngine::compileClass(){
    output << "<class>" << std::endl;
    advance();
    process("class");   // 'class'
    className = getCurrTokenStr();
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
    VARIABLE_KIND kind; std::string type;
    /* static | field */
    if(currToken.second == "static"){
        kind = VARIABLE_KIND::STATIC;
        process("static");
    }else if(currToken.second == "field"){
        kind = VARIABLE_KIND::FIELD;
        process("field");
    }else{
        std::cerr << "Invalid kind of class level variable" << std::endl;
        exit(EXIT_FAILURE);
    }
    /* type */
    type = processType();
    /* varName(identifier) */
    defineVarDec(type, kind);
    /* (, varName) */
    while(getCurrTokenStr() == ","){
        process(",");
        defineVarDec(type, kind);
    }

    /* ';' */
    process(";");
    output << "</classVarDec>" << std::endl;

}

/* subroutine: (constructor| function|method) (void|type)
     subroutineName ( parameterList ) subroutineBody */
void CompileEngine::compileSubroutine(){
    output << "<subroutineDec>" << std::endl;
    subrountineVarTable.reset();
    /* constructor | function | method */
    if(getCurrTokenStr() == "constructor"){
        process("constructor");
    }else if(getCurrTokenStr() == "method"){
        subrountineVarTable.define("this", className, VARIABLE_KIND::ARG);
        process("method"); 
    }else if(getCurrTokenStr() == "function"){
        process("function");
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
        std::string type = processType();
        defineVarDec(type, VARIABLE_KIND::ARG);
        /* (, type varName)* */
        while(getCurrTokenStr() == ","){
            process(",");
            type = processType();
            // process(TOKENTYPE::INDENTIFER);
            defineVarDec(type, VARIABLE_KIND::ARG);
        }
    }
    output << "</parameterList>" << std::endl;
}

/* varDec: var type varName (, varName)* */
void CompileEngine::compileVarDec(){
    output << "<varDec>" << std::endl;
    process("var");
    std::string type = processType();
    subrountineVarTable.define(currToken.second, type, VARIABLE_KIND::VAR);
    process(TOKENTYPE::INDENTIFER);
    while(getCurrTokenStr() == ","){
        process(",");
        subrountineVarTable.define(currToken.second, type, VARIABLE_KIND::VAR);
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
    // process(TOKENTYPE::INDENTIFER);
    processVar(getCurrTokenStr());
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

/*doStatement: do subroutineCall subroutineName(exprlst) | (clsname |varname) . subroutineName (exprlst)*/
void CompileEngine::compileDo(){
    output << "<doStatement>" << std::endl;
    process("do");
    /* subroutineCall: */
    std::string name = getCurrTokenStr();
    bool isVarName = processVar(name);
    if(isVarName){  // varName.subroutineCall
        process(".");
        name = name + "." + getCurrTokenStr();
        process(TOKENTYPE::INDENTIFER);
    }else{
        process(TOKENTYPE::INDENTIFER);
        if(getCurrTokenStr() == "."){   // className.subRoutineName
            process(".");
            name = name + "." + getCurrTokenStr();    // whole name
            process(TOKENTYPE::INDENTIFER);
        }else{  // subroutine

        }
    }
    process("(");
    compileExpressionList();
    process(")");

    process(";");
    output << "</doStatement>" << std::endl;
}

/* compileExpression*/
void CompileEngine::compileExpression(){
    output << "<expression>" << std::endl;
    compileTerm();
    while(OP.count(getCurrTokenStr())){
        process(OP);
        compileTerm();
    }
    output << "</expression>" << std::endl;
}

void CompileEngine::compileTerm(){
    output << "<term>" << std::endl;
    if(currToken.first == TOKENTYPE::INT_CONST){    // integerConstant
        process(TOKENTYPE::INT_CONST);
    }
    else if(currToken.first == TOKENTYPE::STRING_CONST){    // stringConstant
        process(TOKENTYPE::STRING_CONST);
    }
    else if(currToken.first == TOKENTYPE::KEYWORD){
        if(KEYWORD_CONSTANT.count(getCurrTokenStr())){  // keywordConstant
            process(KEYWORD_CONSTANT);
        }
        else{
            std::cerr << "invalid keyword: " << getCurrTokenStr() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(currToken.first == TOKENTYPE::INDENTIFER){
        std::string name = getCurrTokenStr();
        if(processVar(name)){
            // varName
            if(getCurrTokenStr() == "["){   // varName[expr]
            process("[");
            compileExpression();
            process("]");
            }
            else if(getCurrTokenStr()  == "."){
                process(".");
                process(TOKENTYPE::INDENTIFER);
                process("(");
                compileExpressionList();
                process(")");
            }
        }
        else{
             process(TOKENTYPE::INDENTIFER);
            if(getCurrTokenStr() == "("){  // subroutine call: subroutineName.(exprList)
                process("(");
                compileExpressionList();
                process(")");
                }
             else if(getCurrTokenStr() == "."){  // subroutine call: className|varName . subroutineName (exprList)
                process(".");
                process(TOKENTYPE::INDENTIFER);
                process("(");
                compileExpressionList();
                process(")");
            }
        }
    }
    else if(currToken.first == TOKENTYPE::SYMBOL){
        if(getCurrTokenStr() == "("){   // (expr)
            process("(");
            compileExpression();
            process(")");
        }
        else if(UNARY_OP.count(getCurrTokenStr())){ // unaryOp term
            process(UNARY_OP);
            compileTerm();
        }
        else{
            std::cerr << "invalid symbol: " << getCurrTokenStr() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    output << "</term>" << std::endl;
}

int CompileEngine::compileExpressionList(){
    output << "<expressionList>" << std::endl;
    if(getCurrTokenStr() == ")"){
        output << "</expressionList>" << std::endl;
        return 0;
    }
    int cnt = 1;
    compileExpression();
    while(getCurrTokenStr() == ","){
        process(",");
        compileExpression();
        cnt++;
    }
    output << "</expressionList>" << std::endl;
    return cnt;
}