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


int main(int argc, char**argv){
    
    if(argc != 2){
        std::cerr << "invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }
    std::string outName = outname(argv[1]);
    CodeWriter codewriter(outName);

    std::ifstream input(argv[1]);
    if(!input.is_open()){
        std::cerr << "failed to open " << argv[1] << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string currentLine;

    while(std::getline(input, currentLine)){
        size_t pos;
        if((pos = currentLine.find("//")) != std::string::npos){
            currentLine.erase(pos);
        }
        Parser parser(currentLine);
        codewriter.writeCode(currentLine, parser);
    }
    codewriter.close();
    return EXIT_SUCCESS;


}


using std::endl;
std::string outname(std::string inname){
    size_t pos = inname.find(".vm");
    if(pos == std::string::npos){
        std::cerr << "invaild extension name" << std::endl;
        exit(EXIT_FAILURE);
    }
    return inname.erase(pos) + ".asm";
}

/*For Class CodeWriter*/
CodeWriter::CodeWriter(std::string outname):output(outname){
    filename = outname.erase(outname.find(".asm"));
    size_t pos = filename.rfind("/");
    if(pos != std::string::npos){
        filename = filename.substr(pos+1);
    }
}
void CodeWriter::close(){
    output << "(END)" << endl << "@END" <<endl <<  "0;JMP" <<endl;
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
    // else{
    //     std::cerr << "Invalid ParserType" << ParserType << std::endl;
    //     std::exit(EXIT_FAILURE);
    // }
}

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

    // }
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
