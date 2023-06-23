#include"JackTokenizer.h"
JackTokenizer::JackTokenizer(std::string inputFile):input{inputFile}{
    if(!input.is_open()){
        std::cerr << "failed to open the file " << inputFile << std::endl;
        exit(EXIT_FAILURE);
    }
    index = 0;
}

bool JackTokenizer::hasMoreLine() const{
    return !input.eof();
}

/* advance a token*/
Token JackTokenizer::advance(){
    Token currToken = {TOKENTYPE::BLANK, ""};
    while(!input.eof()){
        while(!input.eof() && index >= line.length()){
            advanceLine();
        }
        currToken = advanceToken();
        if(currToken.first != TOKENTYPE::BLANK) break;
    }
    return currToken;
}

Token JackTokenizer::advanceToken(){
    while(index < line.length()){
        if(isspace(line[index])){   // space
            index += 1;
        }
        else if(isdigit(line[index])){  // digits
            size_t endOfNum = line.find_first_not_of(DIGITS, 1+index);
            Token currToken = {TOKENTYPE::INT_CONST, line.substr(index, endOfNum-index)};
            index = endOfNum;
            return currToken;
        }
        else if(line[index] == '"'){    // quote
            size_t endOfQuote = line.find_first_of('"', index+1);
            if(endOfQuote == std::string::npos){    // unable to find another quote at this line -- syntax error
                throw std::logic_error("String ended abruptly");
            }
            Token currToken;
            currToken = {TOKENTYPE::STRING_CONST, line.substr(index+1, endOfQuote-index-1)};
            index = endOfQuote + 1;
            return currToken;
        }
        else if(SYMBOLS.find(line[index]) != std::string::npos){ // symbols
           Token currToken = {TOKENTYPE::SYMBOL, std::string{line[index]}};
           index ++;
           return currToken;
        }
        else if(VALID_IDENTIFIER.find(line[index]) != std::string::npos){   // identifer or keyword
            size_t endOfIdentifer = line.find_first_not_of(VALID_IDENTIFIER, index);
            std::string token = line.substr(index, endOfIdentifer-index);
            index = endOfIdentifer;
            if(KEYWORDS.count(token)){
                return {TOKENTYPE::KEYWORD, token};
            }
            return {TOKENTYPE::INDENTIFER, token};
        }
        else{
            throw std::logic_error("Invalid Character" + line[index]);
        }
    }
    return {TOKENTYPE::BLANK, ""};
}

void JackTokenizer::advanceLine(){
    index = 0;
    line.clear();
    std::getline(input, line);
    // remove comments
    size_t pos1 = line.find("/*");
    size_t pos2 = line.find("//");
    std::string comment{};
    // remove /*
    if(pos1 != std::string::npos && pos1 < pos2){   // find /* and it is after //
        comment = line.substr(pos1);    // the words after /*(included)
        line = line.substr(0, pos1);    // the words before /*(excluede)
        
        while((pos1 = comment.find("*/")) == std::string::npos){ // repeat until */ is found 
            std::getline(input, comment);
        }
        line = line + " " + comment.substr(pos1+2);
    }
    // remove //
    if(pos2 != std::string::npos){
        line.erase(pos2);
    }

    // trim the leading and trailing space
    size_t start = line.find_first_not_of(WHITESPACE);
    size_t end = line.find_last_not_of(WHITESPACE);
    line = (start == std::string::npos)? "": line.substr(start);
    line = (end == std::string::npos)? "" : line.substr(0, end+1);
}
