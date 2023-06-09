#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<cctype>
#include<stack>
#include<filesystem>
#include<cstdlib>
#include<dirent.h>
using std::endl;
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
        CodeWriter(std::string inname, bool is_file);
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

void readFile(const std::string& filename, CodeWriter& codewriter){
        codewriter.setFilename(filename);
        std::ifstream input(filename);
        if(!input.is_open()){
            std::cerr << "failed to open " << filename << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string currentLine;

        while(std::getline(input, currentLine)){
            size_t pos;
            // remove comment   
            if((pos = currentLine.find("//")) != std::string::npos){
                currentLine.erase(pos);
            }
            Parser parser(currentLine);
            codewriter.writeCode(currentLine, parser);
        }
}

/*For Class CodeWriter*/
CodeWriter::CodeWriter(std::string inname, bool is_file):funcs{},filename{}{
    std::string outname = outPutName(inname);
    if(is_file) output.open(outname);
    // else output.open(inname+"/" +outname);
    std::cout << outname << std::endl;
    filename = outname.erase(outname.find(".asm"));
    size_t pos1 = filename.rfind("/");
    if(pos1 != std::string::npos){
        filename.erase(pos1);
    }
}
std::string CodeWriter::outPutName(std::string inname){
    size_t pos = inname.find(".vm");
    std::string outname{};
    if(pos != std::string::npos){
        outname = inname.erase(pos) + ".asm";
    }
    else{
        outname = inname;
        size_t pos1 = inname.rfind("/");
        if(pos1 != std::string::npos)
            outname = outname + inname.erase(0, pos1)+ ".asm";
        else
            outname = outname +".asm";
    }
    // std::cout << outname<< endl;
    return outname;
}
std::string CodeWriter::renameLabel(std::string label){
    if(funcs.empty()) return label;
    else{
        return funcs.top() + "$" + label;
    }
}
std::string CodeWriter::getRet(){
    return funcs.top() + "$ret";
}
void CodeWriter::setFilename(const std::string& file){
    filename = file;
    size_t pos1 = filename.find(".vm");
    if(pos1 != std::string::npos){
        filename.erase(pos1);
    }
    size_t pos2 = filename.rfind("/");
    if(pos2 != std::string::npos){
        filename = filename.substr(pos2+1);
    }
}
void CodeWriter::writeSysInit(){
    output << "// call Sys.init" << endl;
    output << "@256" << endl << "D=A" << endl;
    output << "@SP" << endl << "M=D" << endl;
    writeCall("Sys.init", 0);
    // output << "@Sys.init" << endl << "0;JMP" << endl;
}

void CodeWriter::close(){
    // add the end loop
    output << "// return " << endl;
    // writeReturn();
    output << "(END)" << endl << "@END" <<endl <<  "0;JMP" <<endl;
    output.close();
}

void CodeWriter::writeCode(const std::string& line, const Parser& parser){
    C_TYPE ParserType = parser.getCommandType();
    if(ParserType==EMPTY) return;
    output << "// " << line << endl;
    if(ParserType == C_ARITHMETIC){
        writeArithemic(parser.getArg1());
    }
    else if(ParserType == C_PUSH){
        writePush(parser.getArg1(), parser.getArg2());
    }
    else if(ParserType == C_POP){
        writePop(parser.getArg1(), parser.getArg2());
    }
    else if(ParserType == C_CALL){
        writeCall(parser.getArg1(), parser.getArg2());
    }
    else if(ParserType == C_FUNCTION){
        writeFunction(parser.getArg1(), parser.getArg2());
    }
    else if(ParserType == C_RETURN){
        writeReturn();
    }
    else if(ParserType == C_LABEL){
        writeLabel(parser.getArg1());
    }
    else if(ParserType == C_IF){
        writeIf(parser.getArg1());
    }
    else if(ParserType == C_GOTO){
        writeGoto(parser.getArg1());
    }
}
// those write method was written in project 7, assume that there are no errors in them
void CodeWriter::writeArithemic(const std::string& command){
    static int _eq = 0, _lt=0, _gt=0;
    if(command == "add"){
        BinaryArithemic('+');
    }
    else if(command == "sub"){
        BinaryArithemic('-');
    }
    else if(command == "and"){
        BinaryArithemic('&');
    }
    else if(command == "or"){
        BinaryArithemic('|');
    }
    else if(command == "neg"){
        UnaryArithemic('-');
    }
    else if(command == "not"){
        UnaryArithemic('!');
    }
    else if (command == "eq"){
        _eq = CompareArothemic("EQ", _eq);
    }
    else if(command == "lt"){
        _lt = CompareArothemic("LT", _lt);
    }
    else{
        _gt = CompareArothemic("GT", _gt);
    }
}
void CodeWriter::writePush(const std::string& segment, int pos){
    if(segment == "constant"){
        output << "@" << pos << endl << "D=A"<<endl;
        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;
        output << "@SP" << endl << "M=M+1"<<endl;
    }
    else if(segment == "static"){
        output <<"@" << filename << "." << pos << endl;
        output << "D=M"<< endl;
        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;
        output << "@SP" << endl << "M=M+1"<<endl;
    }
    else{
        if(segment == "local"){
            addrPush("LCL", 'M',pos);
        }
        else if(segment == "argument"){
            addrPush("ARG", 'M',pos);
        }
        else if(segment == "this"){
            addrPush("THIS", 'M',pos);
        }
        else if(segment == "that"){
            addrPush("THAT", 'M', pos);
        }
        else if(segment == "temp"){
            addrPush("5", 'A',pos);
        }
        else if(segment == "pointer"){
            addrPush("3",'A', pos);
        }
        else{
            std::cerr << "invaild segment" << std::endl;
             exit(EXIT_FAILURE);
        }
    }
}
void CodeWriter::writePop(const std::string& segment, int pos){
    if(segment == "static"){
        output << "@SP" << endl << "M=M-1" << endl;
        output << "A=M" << endl << "D=M" << endl;
        output << "@" << filename << "." << pos << endl << "M=D" << endl;
    }
    else{
        if(segment == "local"){
            addrPop("LCL", 'M',pos);
        }
        else if(segment == "argument"){
            addrPop("ARG", 'M',pos);
        }
        else if(segment == "this"){
            addrPop("THIS", 'M',pos);
        }
        else if(segment == "that"){
            addrPop("THAT", 'M', pos);
        }
        else if(segment == "temp"){
            addrPop("5", 'A',pos);
        }
        else if(segment == "pointer"){
            addrPop("3",'A', pos);
        }
        else{
            std::cerr << "invaild segment" << std::endl;
             exit(EXIT_FAILURE);
        }
    }
}

void CodeWriter::writeReturn(){
    // inner
    // frame(*R13) = LCL
    output << "@LCL" << endl << "D=M" << endl;
    output << "@R13" << endl << "M=D" << endl;
    // retAddr(*R14) = *(frame-5) // now D=*LCL= frame
    output <<  "@5" << endl << "D=D-A" << endl;
    output << "A=D" << endl << "D=M" << endl; // D=*(frame-5)
    output << "@R14" << endl << "M=D" << endl;
    //*ARG = pop()
    output << "@SP" << endl << "AM=M-1" << endl;
    output << "D=M" << endl; // D=pop()
    output << "@ARG" << endl << "A=M" << endl;
    output <<  "M=D" << endl; // D=*ARG
    // SP=ARG+1
    output << "@ARG" << endl << "D=M" << endl;
    output << "D=D+1" << endl; // D=ARG+1 
    output << "@SP" << endl << "M=D" << endl;
    // THAT = *(frame-1)  THIS = *(frame-2)
    // ARG = *(frame-3) THAT = *(frame-4)
    returnHelper("THAT", 1);    returnHelper("THIS", 2);
    returnHelper("ARG", 3);     returnHelper("LCL", 4);
    // goto retAddr
    output << "@R14" << endl << "A=M" << endl; // A=retAddr
    output << "0;JMP" << endl;
    // outer
}
void CodeWriter::writeCall(const std::string& funcname, int nArgs){
    static int call_cnt = 0;
    // innerPart
    // push retAddr
    output << "@" << funcname << "$ret" << call_cnt << std::endl;
    output << "D=A"<< std::endl << "@SP" << std::endl << "AM=M+1" << std::endl;
    output << "A=A-1" << std::endl << "M=D" << std::endl;
    // push LCL/ARG/THIS/THAT
    callHelper("LCL");  callHelper("ARG");
    callHelper("THIS"); callHelper("THAT");
    // ARG = SP-5-nArgs
    output << "@SP" << std::endl << "D=M" << std::endl;
    output << "@5" << std::endl <<  "D=D-A" << std::endl;
    output << "@" << nArgs << std::endl << "D=D-A" << std::endl;
    output << "@ARG" << endl << "M=D" << endl;
    // LCL = SP
    output << "@SP" << endl << "D=M" << endl;  // D = *SP
    output << "@LCL" << endl << "M=D" << endl;
    // goto (function)
    output << "@" << funcname << endl << "0;JMP" << endl;
    // (returnAddr)
    output << "(" << funcname << "$ret" << call_cnt << ")"<< std::endl;
    call_cnt += 1;
}
void CodeWriter::writeFunction(const std::string& funcname, int nVals){
    if(!funcs.empty())
        funcs.pop();
    funcs.push(funcname);
    output << "(" << funcname << ")" << std::endl;
    while(nVals--){
        output << "@SP" << std::endl << "AM=M+1" << std::endl; 
        output << "A=A-1" << std::endl << "M=0" << std::endl; 
    }
}
void CodeWriter::writeLabel(const std::string& label){
    std::string finalLabel = renameLabel(label);
    output << "(" << finalLabel << ")" << std::endl;
}
void CodeWriter::writeGoto(const std::string& label){
    std::string finalLabel = renameLabel(label);
    output << "@" << finalLabel << std::endl;
    output << "0;JMP" << std::endl;
}
void CodeWriter::writeIf(const std::string& label){
    std::string finalLabel = renameLabel(label);
    output << "@SP" << std::endl << "AM=M-1"<< std::endl;
    output << "D=M" << std::endl << "@" << finalLabel << std::endl;
    output << "D;JNE" << std::endl;
}

void CodeWriter::addrPop(const std::string& segment, char obj, int index){
        output << "@SP" << endl << "M=M-1" << endl;
        output << "@" << segment <<endl<< "D=" << obj << endl;
        output << "@" << index << endl;
        output << "D=D+A" << endl;  // to get the address of ram
        output << "@SP" << endl << "A=M" << endl;
        output << "D=M+D" << endl << "A=D-M" << endl;   // M=value, D=addr+value, A =addr,M=val of A
        output << "M=D-A" << endl;

}
void CodeWriter::addrPush(const std::string& segment, char obj, int index){
    output << "@" << segment <<endl<< "D=" << obj << endl;
    output << "@" << index << endl << "D=D+A" <<endl;
    output << "A=D" << endl << "D=M"<< endl;
    output << "@SP" << endl << "A=M" << endl << "M=D" << endl;
    output << "@SP" << endl << "M=M+1"<<endl;
}

void CodeWriter::BinaryArithemic(char oper){
    output << "@SP" << endl << "AM=M-1" << endl;
    output << "D=M" << endl << "A=A-1" << endl;
    output << "D=M" << oper << "D" << endl << "M=D" <<endl;
}

void CodeWriter::UnaryArithemic(char oper){
    output << "@SP" << endl << "A=M-1" << endl;
    output << "M=" << oper << "M" << endl; 
}

int CodeWriter::CompareArothemic(const std::string& comp, int time){
    output << "@SP" << endl << "AM=M-1" << endl;
    output << "D=M" << endl << "A=A-1" << endl << "D=M-D" << endl;
    output << "@" << comp << "_" << time << endl;
    output << "D;J" << comp << endl;
    output << "@SP" << endl << "A=M-1"<< endl << "M=0" << endl;
    output << "@END_" << comp << "_" << time << endl;
    output << "0;JMP" << endl;
    output << "(" << comp << "_" << time <<")" << endl;
    output << "@SP" << endl << "A=M-1"<< endl << "M=-1" << endl;
    output << "(END_" << comp << "_" << time << ")" << endl;
    return time + 1;
}

void CodeWriter::callHelper(const std::string& pointer){
    output << "@" << pointer << std::endl;
    output << "D=M" << std::endl << "@SP" << std::endl;
    output << "AM=M+1" << std::endl << "A=A-1" << std::endl;
    output << "M=D" << std::endl;
}
void CodeWriter::returnHelper(const std::string& pointer, int index){
    // *point=*(frame-index)  frame=*R13
    output << "@R13" << endl << "D=M" << endl; // D=frame
    output << "@" << index << endl << "D=D-A" << endl; // D=frame-index
    output << "A=D" << endl << "D=M" << endl; // D=*(frame-index)
    output << "@" << pointer << endl << "M=D" << endl;
}
/*for Class Parser*/
Parser::Parser(const std::string& line){
    // parse
    std::istringstream iss(line);
    std::string arg;    std::vector<std::string> args;
    while(iss >> arg){
        args.push_back(arg);
    }
    if(args.empty()){
        commandType = EMPTY;
        return;
    }
    commandType = analyseType(args[0]);
    if(commandType == C_ARITHMETIC){
        arg1 = args[0];
    }
    else if(commandType != C_RETURN){
        arg1 = args[1];
    }
    if(commandType == C_PUSH || commandType == C_POP ||
            commandType == C_FUNCTION || commandType == C_CALL){
         arg2 = std::stoi(args[2]);
    }
}

C_TYPE Parser::analyseType(const std::string& command) const{
    if(command == "push"){
        return C_PUSH;
    }
    if(command == "pop"){
        return C_POP;
    }
    if(command == "return"){
        return C_RETURN;
    }
    if(command == "call"){
        return C_CALL;
    }
    if(command == "function"){
        return C_FUNCTION;
    }
    if(command == "label"){
        return C_LABEL;
    }
    if(command == "if-goto"){
        return C_IF;
    }
    if(command == "goto"){
        return C_GOTO;
    }
    std::vector<std::string>arithmetic{"add", "sub", "neg", "eq", 
            "lt", "gt", "and", "or", "not"};
    for(auto& key:arithmetic){
        if(command == key){
            return C_ARITHMETIC;
        }
    }
    std::cerr << "invalid C_TYPE" << std::endl;
    std::exit(EXIT_FAILURE);
    
}

std::string Parser::getArg1() const{
    return arg1;
}
int Parser::getArg2() const{
    return arg2;
}
C_TYPE Parser::getCommandType() const{
    return commandType;
}


std::vector<std::string> GetListFiles(const std::string& path, const std::string& exten = "*")
{
    std::vector<std::string> list;
    list.clear();
 
    DIR* dp = nullptr;
    struct dirent* dirp = nullptr;
    if ((dp = opendir(path.c_str())) == nullptr) {
        return list;
    }
 
    while ((dirp = readdir(dp)) != nullptr) {
        if (dirp->d_type == DT_REG) {
            if (exten.compare("*") == 0)
                list.emplace_back(static_cast<std::string>(dirp->d_name));
            else
                if (std::string(dirp->d_name).find(exten) != std::string::npos)
                    list.emplace_back(static_cast<std::string>(dirp->d_name));
        }
    }
 
    closedir(dp);
 
    return list;
}

int main(int argc, char**argv){
    // check if there are 2 arguments on command line
    if(argc != 2){
        std::cerr << "invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }
    // bind inname to the argv[1]
    std::string inname{argv[1]};
    // if the filename contains .vm, read a single file
    if(inname.find(".vm") != std::string::npos){
        // init codewriter
        CodeWriter codewriter(argv[1], true);
        // read the line
        readFile(inname, codewriter);
        // close ifstream and add end loop
        codewriter.close();
    }
    else{
        // the filname is a dir
        CodeWriter codewriter(argv[1], false);
        // // write call sys.init
        codewriter.writeSysInit();
        std::vector<std::string> files = GetListFiles(argv[1]);
        // std::cout << "here" << std::endl;
        std::string path = argv[1];
        for(auto& elem: files){
            // std::cout << std::string(argv[1])+ "/" + elem << std::endl;
        // }
        // for(const auto& entry: std::filesystem::directory_iterator(path)){
            // std::string filename = entry.path().string();
            std::string filename = path + "/" + elem;
            size_t pos = filename.find(".vm");
            if(pos == std::string::npos) continue;  // it is not a vm file
            else{
                readFile(filename, codewriter);
            }
        }
        // close ifstream and add end loop
        codewriter.close();            
    }
    
    return EXIT_SUCCESS;


}
