#include<iostream>
#include<fstream>
#include<set>
#include<cctype>
#include<algorithm>
enum class KEYWORD{
    CLASS,  METHOD, FUNCTION,
    CONSTRUCTOR, INT, BOOLEAN,
    CHAR, VOID, VAR, STATIC, FIELD,
    LET, DO, IF, ELSE, WHILE, RETURN,
    TRUE, FALSE, Null, THIS
};

enum class TOKENTYPE{
    KEYWORD, SYMBOL, INDENTIFER,
    INT_CONST, STRING_CONST, BLANK
};
const std::string SYMBOLS{"{}()[].,;+-*/&|<>=~"};
const std::string DIGITS{"0123456789"};
const std::string VALID_IDENTIFIER{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"};
const std::string WHITESPACE{" \n\r\v\t\f"};
const std::set<std::string> KEYWORDS{"class", "constructor",
                                "function", "method", "field",
                                "static", "var", "int", "char",
                                "boolean", "void", "true", "false",
                                "null", "this", "let", "do", "if",
                                "else", "while", "return"};
using Token = std::pair<TOKENTYPE, std::string>;

class JackTokenizer{
    public:
        JackTokenizer(std::string inputFilename);
        bool hasMoreLine() const;
        Token advance();
        static std::string getType(Token token);
    private:
        std::string line;   size_t index;
        // std::pair<TOKENTYPE, std::string> currToken;
        std::ifstream input;
        Token advanceToken();
        void advanceLine();
};

// class JackAnalyzer{

// }