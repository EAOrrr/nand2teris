#ifndef TOOL_INCLUDED
#define TOOL_INCLUDED
#include<iostream> // for test and getline
#include<fstream>  // to deal with file
#include<string>  // to deal with each line in the file
#include<map>   // for table and c-instruction
#include<cstdlib> // for exit the program
#include<algorithm>
#include<iomanip>
#include<cctype>
class SYMBOL_TABLE{
    private:
        std::map<std::string, int> symbol_table;
        int curr = 16;
    public:
        SYMBOL_TABLE();
        void addEntry(const std::string&, const int);
        void addEntry(const std::string&);
        bool contain(const std::string&) const;
        int getAddress(const std::string&);
        void print() const; // for test
};

const std::map<std::string, std::string> DEST{
    {"null" , "000"},
    {"M" , "001"},
    {"D" , "010"},
    {"DM" , "011"},
    {"MD", "011"},
    {"A" , "100"},
    {"AM" , "101"},
    {"MA", "101"},
    {"AD", "110"},
    {"DA", "110"},
    {"ADM", "111"},
    {"DAM", "111"},
    {"AMD", "111"},
    {"DMA", "111"},
    {"MDA", "111"},
    {"MAD", "111"}
};
const std::map<std::string, std::string> JUMP{
    {"null", "000"},
    {"JGT", "001"},
    {"JEQ", "010"},
    {"JGE", "011"},
    {"JLT", "100"},
    {"JNE", "101"},
    {"JLE", "110"},
    {"JMP", "111"}
};
const std::map<std::string, std::string> COMP{
    {"0", "0101010"},
    {"1", "0111111"},
    {"-1", "0111010"},
    {"D", "0001100"},
    {"A", "0110000"},
    {"!D", "0001101"},
    {"!A", "0110001"},
    {"-D", "0001111"},
    {"-A", "0110011"},
    {"1+D", "0011111"},
    {"D+1", "0011111"},
    {"1+A", "0110111"},
    {"A+1", "0110111"},
    {"D-1", "0001110"},
    {"A-1", "0110010"},
    {"D+A", "0000010"},
    {"A+D", "0000010"},
    {"D-A", "0010011"},
    {"A-D", "0000111"},
    {"A&D", "0000000"},
    {"D&A", "0000000"},
    {"A|D", "0010101"},
    {"D|A", "0010101"},
    {"M", "1110000"},
    {"!M", "1110001"},
    {"-M", "1110011"},
    {"1+M", "1110111"},
    {"M+1", "1110111"},
    {"M-1", "1110010"},
    {"D+M", "1000010"},
    {"M+D", "1000010"},
    {"D-M", "1010011"},
    {"M-D", "1000111"},
    {"M&D", "1000000"},
    {"D&M", "1000000"},
    {"M|D", "1010101"},
    {"D|M", "1010101"}
};
// to deal with files
bool hasMoreLine(std::ifstream& input);
std::string outname(const std::string& inname);
// to remove blank
void removeBlank(std::string& line);
// to identity the instruction
bool isSymb(const std::string& line);
bool isAinstruction(const std::string& line);
bool isCinstruction(const std::string& line);
// parsers
void parseSymb(const std::string& line, const int row, SYMBOL_TABLE& symB);
void parseAInst(const std::string& line, std::ofstream& hack, SYMBOL_TABLE& symB);
void parseCInst(const std::string& line, std::ofstream& hack);
// for parse A
std::string convertToBinary(int value);
// for parse C
std::string dest(const std::string& line);
std::string comp(const std::string& line);
std::string jump(const std::string& line);
#endif