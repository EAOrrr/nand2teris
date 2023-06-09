#include"tool.h"
using std::cout;
using std::endl;    using std::cerr;
using std::string;  using std::map; 

// implementation for symbol table class
SYMBOL_TABLE::SYMBOL_TABLE(){
    symbol_table["R0"] = 0;    
    symbol_table["R1"] = 1;   
    symbol_table["R2"] = 2;
    symbol_table["R3"] = 3;
    symbol_table["R4"] = 4;
    symbol_table["R5"] = 5;    
    symbol_table["R6"] = 6;    
    symbol_table["R7"] = 7;    
    symbol_table["R8"] = 8;    
    symbol_table["R9"] = 9;    
    symbol_table["R10"] = 10;    
    symbol_table["R11"] = 11;    
    symbol_table["R12"] = 12;    
    symbol_table["R13"] = 13;    
    symbol_table["R14"] = 14;
    symbol_table["R15"] = 15;
    symbol_table["SCREEN"] = 16384;
    symbol_table["KBD"] = 24576;
    symbol_table["SP"] = 0;
    symbol_table["LCL"] = 1;
    symbol_table["ARG"] = 2;
    symbol_table["THIS"] = 3;
    symbol_table["THAT"] = 4;
}
void SYMBOL_TABLE::addEntry(const std::string& symbol, const int address){
    symbol_table[symbol] = address;
}
void SYMBOL_TABLE::addEntry(const std::string& symbol){
    symbol_table[symbol] = curr;
    curr++;
}
bool SYMBOL_TABLE::contain(const std::string& symbol) const{
    return symbol_table.count(symbol);
}
int SYMBOL_TABLE::getAddress(const std::string& symbol){
    return symbol_table[symbol];
}
void SYMBOL_TABLE::print() const{
    for(auto& symaddr: symbol_table){
        cout << symaddr.first << ":" << symaddr.second << endl;
    }
}


std::string outname(const std::string& inname){
    std::string result(inname);
    size_t pos = result.find(".asm");
    if(pos == std::string::npos){
        cerr << "wrong input file type" << endl;
        std::exit(EXIT_FAILURE);
    }
    result.replace(pos, 4, ".hack");
    return result;
}

bool hasMoreLine(std::ifstream& inputfile){
    return !inputfile.eof();
}

void removeBlank(std::string& line){
    auto found = line.find("//");
    if(found != std::string::npos){
        line.erase(found);
    }
    std::erase_if(line, [](char c){return isspace(c) || isblank(c);});
}

bool isSymb(const std::string& line){
    auto start = line.find('('), end = line.find(')');
    if(start != std::string::npos && end != std::string::npos && end-start > 1)
        return true;
    return false;
}

bool isAinstruction(const std::string& line){
    return line[0] == '@';
}

bool isCinstruction(const std::string& line){
    return line.find('=') != std::string::npos || line.find(';') != std::string::npos; 
}

void parseSymb(const std::string& line, const int row, SYMBOL_TABLE& symB){
    size_t start = line.find('(')+1,  end = line.find(')');
    // auto addr = line.substr(start, end-start);
    // std::erase_if(addr, [](char c){
    //     // return !isalnum(c) && c != '_'&&c!='.';
    //     return isspace(c) || isblank(c);
    //     });
    symB.addEntry(line.substr(start, end-start), row);
}

void parseAInst(const std::string& line, std::ofstream& hack, SYMBOL_TABLE& symB){
   string addr = line.substr(1);    //@addr, to get address
   int value;   // to get the int value addr
   try{
        value = std::stoi(addr);    // if addr is a value
                                    // e.g @34, no exception should be raised
   }
   catch(std::invalid_argument const& ex){
        // // addr is a symbol
        // std::erase_if(addr, [](char c){
        //     // return !isalnum(c) && c!='_'&&c != '.';
        //     return isspace(c) || isblank(c);});
        if(!symB.contain(addr)){
            symB.addEntry(addr);
        }
        value = symB.getAddress(addr);
   }
//    cout << value;
   hack << std::setfill('0') << std::setw(16) << convertToBinary(value) << endl; 

}
void parseCInst(const std::string& line, std::ofstream& hack){
    std::string d = dest(line);
    auto c = comp(line);
    auto j = jump(line);
    hack << "111" << c << d << j <<endl;
}

std::string convertToBinary(int value){
    std::string result;
    while(value > 0){
        result = std::to_string(value%2) + result;
        value /= 2;
    }
    return result;
}
std::string dest(const std::string& line){
    auto end = line.find('=');
    if(end != std::string::npos){
        std::string d = line.substr(0, end);
        // cout << d.length() <<endl;
        if(DEST.count(d) == 0) throw std::logic_error("dest"+d);
        return DEST.at(d);
    //     cout << d << endl;
    //     std::string result = "000";
    //     cout << result;
    //    if(line.find('M') != std::string::npos) result[2] = '1';
    //    cout << "AfterM" << result << endl;
    //    if(line.find('A') != std::string::npos) result[0] = '1';
    //    cout << "AfterA" << result << endl;
    //    if(line.find('D') != std::string::npos) result[1] = '1';
    //    cout << "AfterD" << result << endl;
    // return result;
    }
    return "000";
}
std::string comp(const std::string& line){
    size_t start = line.find("="), end = line.find(';');
    if(start == std::string::npos) start = 0;
    else start += 1;
    if(end == std::string::npos) end  = line.length();
    std::string c = line.substr(start, end-start);
    // std::erase_if(c, [](char ch){
    //     std::string check = "+-!|&";
    //     return !std::isalnum(ch) && check.find(ch) == std::string::npos;});
    // cout << endl<< c << endl << c.length() << endl;
    if(COMP.count(c) == 0) throw std::logic_error("comp"+c); 
    return COMP.at(c);
}
std::string jump(const std::string& line){
    size_t start = line.find(';');
    if(start != std::string::npos){
        std::string j = line.substr(start+1);
        // std::cout << "j: " << j << endl;
        // std::erase_if(j, [](char c){
        //     return !std::isalnum(c);
        // });
        // cout<< j.length() << endl;
        if(JUMP.count(j) == 0) throw std::logic_error("jump"+j+" " + std::to_string(j.length()));
        return JUMP.at(j);
    }
    return "000";
}