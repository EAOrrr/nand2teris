#include"compileEngine.h"

CompileEngine::CompileEngine(const std::string &inname):tokenizer(inname), writer(inname){};

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
    }
    else{
        std::cerr << "Expected type: " << getTokenType({tokenType, ""});
        std::cerr << " but get " << getTokenType(currToken) << " with token " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}

void CompileEngine::process(const std::string &str){
    if(getCurrTokenStr() == str){
    }
    else{
        std::cerr << "Expected specifier: " << str << " but get " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}

void CompileEngine::process(std::set<std::string> strs){
    if(strs.count(getCurrTokenStr())){
    }
    else{
        std::cerr << "Invalid specifier " << getCurrTokenStr() << std::endl;
        exit(EXIT_FAILURE);
    }
    advance();
}
std::string CompileEngine::processType(){
    if(currToken.first == TOKENTYPE::INDENTIFER){
    }
    else if(currToken.first == TOKENTYPE::KEYWORD){
        if(getCurrTokenStr() == "int" ||
         getCurrTokenStr() == "char" || currToken.second == "boolean"){
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

bool CompileEngine::processVar(const std::string &name, size_t& index, std::string& kind, std::string& type){
    if(currToken.first == TOKENTYPE::INDENTIFER){
        
        std::string name = currToken.second;
        if(subrountineVarTable.contain(name)){
            index = subrountineVarTable.indexOf(name);  type = subrountineVarTable.typeOf(name);
            kind = SymbolTable::strOfKind(subrountineVarTable.kindOf(name));
        }else if(classVarTable.contain(name)){
            index = classVarTable.indexOf(name);    type = classVarTable.typeOf(name);
            kind = SymbolTable::strOfKind(classVarTable.kindOf(name));
            if(kind == "field") kind = "this";
        }
        else{
            return false;
        }
        advance();
        return true;
    }else{
        return false;
    }
}

void CompileEngine::defineVarDec(const std::string &type, VARIABLE_KIND kind){
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
    advance();
    process("class");   // 'class'

    /*save clsName for compile subroutine and generate label*/
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
}

/* ClassVarDec structure: {'static'|'field'} type varName(identifier) (, varName)* ; */
void CompileEngine::compileClassVarDec(){
    VARIABLE_KIND kind; std::string type;
    /* static | field */
    if(currToken.second == "static"){
        // add static var to table
        kind = VARIABLE_KIND::STATIC;
        process("static");
    }else if(currToken.second == "field"){
        // add field var to table
        kind = VARIABLE_KIND::FIELD;
        process("field");
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
}

/* subroutine: (constructor| function|method) (void|type)
     subroutineName ( parameterList ) subroutineBody */
void CompileEngine::compileSubroutine(){
    subrountineVarTable.reset();
    ifCount = 0; whileCount = 0;
    FUNCTION_TYPE funcType;
    /* constructor | function | method */
    if(getCurrTokenStr() == "constructor"){
        funcType = FUNCTION_TYPE::CONSTRUCTOR;
        process("constructor");
    }else if(getCurrTokenStr() == "method"){
        funcType = FUNCTION_TYPE::METHOD;
        subrountineVarTable.define("this", className, VARIABLE_KIND::ARG);
        process("method"); 
    }else if(getCurrTokenStr() == "function"){
        funcType = FUNCTION_TYPE::FUNCTION;
        process("function");
    }

    /* void | type */
    if(getCurrTokenStr() == "void"){process("void");}
    else processType();

    /* subroutineName(identifier) */
    functionName = className + "." + getCurrTokenStr();   // save funcName
    process(TOKENTYPE::INDENTIFER);

    /*( parameterList )*/
    process("(");
    compileParameterList();
    process(")");

    /* subroutineBody */
    compileSubroutineBody(funcType);
}

/* parameterList: (empty) or type varName (, type varName)* */
void CompileEngine::compileParameterList(){
    if(getCurrTokenStr() != ")"){    // not empty
        /* type varName */
        std::string type = processType();
        defineVarDec(type, VARIABLE_KIND::ARG);
        /* (, type varName)* */
        while(getCurrTokenStr() == ","){
            process(",");
            type = processType();
            defineVarDec(type, VARIABLE_KIND::ARG);
        }
    }
}

/* varDec: var type varName (, varName)* */
void CompileEngine::compileVarDec(){
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
}

/* subroutineBody: { varDec* statements }*/
void CompileEngine::compileSubroutineBody(FUNCTION_TYPE functionType){
    process("{");
    while(getCurrTokenStr() == "var"){
        compileVarDec();
    }
    // function className.functionName nVar
    writer.writeFunction(functionName, subrountineVarTable.varCount(VARIABLE_KIND::VAR));
    switch(functionType){
        case FUNCTION_TYPE::CONSTRUCTOR:
            writer.writePush("constant", classVarTable.varCount(VARIABLE_KIND::FIELD));
            writer.writeCall("Memory.alloc", 1);       
            writer.writePop("pointer", 0);
            break;
        case FUNCTION_TYPE::METHOD:
            writer.writePush("argument", 0);
            writer.writePop("pointer", 0); 
            break;
        default:
            break;
    }
    compileStatements();
    process("}");
}

/* Statements: statement*
    i.e letStament | ifStatement | whileStatement | doStatement | returnStatement */
void CompileEngine::compileStatements(){
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
}

/* ifStatement: if ( expr ) { statements } (else { statements } )? */
void CompileEngine::compileIf(){
    std::string labeIfElse = "IF_ELSE"+std::to_string(ifCount);
    std::string labelIfEnd = "if_END"+std::to_string(ifCount);
    ifCount++;
    process("if");
    process("(");
    compileExpression();
    writer.writeArithmetic('~');  // not
    writer.writeIf(labeIfElse);
    process(")");
    process("{");
    compileStatements();
    process("}");
    writer.writeGoto(labelIfEnd);
    writer.writeLabel(labeIfElse);
    if(getCurrTokenStr() == "else"){
        process("else");
        process("{");
        compileStatements();
        process("}");
    }
    writer.writeLabel(labelIfEnd);
}

/*letStatement: let varName ([expr])? = expr ;*/
void CompileEngine::compileLet(){
    size_t index;   std::string segment; std::string type;
    bool isArray = false;
    process("let");
    // process(TOKENTYPE::INDENTIFER);
    std::string varName = getCurrTokenStr();
    processVar(varName, index, segment, type);
    if(getCurrTokenStr() == "["){
        isArray = true;
        process("[");
        writer.writePush(segment, index);
        compileExpression();
        writer.writeArithmetic('+');    // add
        process("]");
    }
    process("=");
    compileExpression();
    if(isArray){
        writer.writePop("temp", 0);
        writer.writePop("pointer", 1);
        writer.writePush("temp", 0);
        writer.writePop("that", 0);
    }else{
        writer.writePop(segment, index);
    }
    process(";");
}

/* whileStaement: while ( expr ) { statements }*/
void CompileEngine::compileWhile(){
    std::string labelWhileExp = "WHILE_EXP"+std::to_string(whileCount);
    std::string labelWhileEnd = "WHILE_END"+std::to_string(whileCount);
    whileCount++;
    process("while");
    process("(");
    writer.writeLabel(labelWhileExp);
    compileExpression();
    writer.writeArithmetic('~');    // not
    writer.writeIf(labelWhileEnd);
    process(")");
    process("{");
    compileStatements();
    writer.writeGoto(labelWhileExp);
    process("}");
    writer.writeLabel(labelWhileEnd);
}

/* returnStatement: return expr? ;*/
void CompileEngine::compileReturn(){
    process("return");
    if(getCurrTokenStr() != ";") compileExpression();
    else writer.writePush("constant", 0);
    process(";");
    writer.writeReturn();
}

/*doStatement: do subroutineCall subroutineName(exprlst) | (clsname |varname) . subroutineName (exprlst)*/
void CompileEngine::compileDo(){
    process("do");
    compileExpression();
    process(";");
    writer.writePop("temp", 0);
}

/* compileExpression*/
void CompileEngine::compileExpression(){
    compileTerm();
    while(OP.count(getCurrTokenStr())){
        std::string op = getCurrTokenStr();
        process(OP);
        compileTerm();
        writer.writeArithmetic(op[0]);
    }
}

void CompileEngine::compileTerm(){
    if(currToken.first == TOKENTYPE::INT_CONST){    // integerConstant
        writer.writePush("constant", std::stoi(getCurrTokenStr()));
        process(TOKENTYPE::INT_CONST);
    }
    else if(currToken.first == TOKENTYPE::STRING_CONST){    // stringConstant
        /* manipulate string */
        std::string stringConst  = getCurrTokenStr();
        size_t  len = stringConst.length();
        writer.writePush("constant", len);
        writer.writeCall("String.new", 1);
        for(auto c: stringConst){
            writer.writePush("constant", static_cast<size_t>(c));
            writer.writeCall("String.appendChar", 2);
        }
        process(TOKENTYPE::STRING_CONST);
    }
    else if(currToken.first == TOKENTYPE::KEYWORD){
        if(KEYWORD_CONSTANT.count(getCurrTokenStr())){  // keywordConstant
            if(getCurrTokenStr() == "this") writer.writePush("pointer", 0);
            else if(getCurrTokenStr() == "null" || getCurrTokenStr() == "false") writer.writePush("constant", 0);
            else if(getCurrTokenStr() == "true"){
                writer.writePush("constant", 0);
                writer.writeArithmetic('~');
            }
            process(KEYWORD_CONSTANT);
        }
        else{
            std::cerr << "invalid keyword: " << getCurrTokenStr() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(currToken.first == TOKENTYPE::INDENTIFER){
        std::string name = getCurrTokenStr();
        size_t index;   std::string segment, type;
        if(processVar(name, index, segment, type)){
            /* varName */
            if(getCurrTokenStr() == "["){   // varName[expr]
            writer.writePush(segment, index);
            process("[");
            compileExpression();
            writer.writeArithmetic('+');    // add
            writer.writePop("pointer", 1);
            writer.writePush("that", 0);
            process("]");
            }
            else if(getCurrTokenStr()  == "."){ // subroutine call: varName . subroutine(exprlst)
                                                // method
                process(".");
                std::string subroutineName = getCurrTokenStr();
                process(TOKENTYPE::INDENTIFER);
                writer.writePush(segment, index);
                process("(");
                int nArgs = compileExpressionList();
                process(")");
                writer.writeCall(type+"."+subroutineName, nArgs+1);
            }
            else{   //varName
                writer.writePush(segment, index);
            }
        }
        else{
             process(TOKENTYPE::INDENTIFER);
            if(getCurrTokenStr() == "("){  // subroutine call: subroutineName(exprList)
                                            // method
                process("(");
                writer.writePush("pointer", 0);
                size_t nArgs= compileExpressionList();
                process(")");
                writer.writeCall(className+"."+name, nArgs+1);
                }
             else if(getCurrTokenStr() == "."){  // subroutine call: className . subroutineName (exprList)
                                                 // function/constructor
                process(".");
                std::string subroutineName = getCurrTokenStr();
                process(TOKENTYPE::INDENTIFER);
                process("(");
                size_t nArgs = compileExpressionList();
                process(")");
                writer.writeCall(name+"."+subroutineName, nArgs);
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
            char unaryOp = getCurrTokenStr()[0];
            process(UNARY_OP);
            compileTerm(); 
            switch(unaryOp){
                    case '~':
                        writer.writeArithmetic('~');
                        break;
                    case '-':
                        writer.writeArithmetic('_');
                        break;
                    default:
                        break;
                }
        }
        else{
            std::cerr << "invalid symbol: " << getCurrTokenStr() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

int CompileEngine::compileExpressionList(){
    if(getCurrTokenStr() == ")"){
        return 0;
    }
    int cnt = 1;
    compileExpression();
    while(getCurrTokenStr() == ","){
        process(",");
        compileExpression();
        cnt++;
    }
    return cnt;
}