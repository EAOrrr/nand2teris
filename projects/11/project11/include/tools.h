#ifndef TOOLS_H
#define TOOLS_H
#include<iostream>
#include<fstream>
#include<set>
#include<map>
#include<vector>
#include<cctype>
#include<algorithm>
#include<cstdlib>
enum class TOKENTYPE{
    KEYWORD, SYMBOL, INDENTIFER,
    INT_CONST, STRING_CONST, BLANK
};
enum class VARIABLE_KIND{
    STATIC, FIELD, ARG, VAR, NONE
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
const std::set<std::string> OP{"+", "-", "*", "/","&", "|",">", "<", "="};
const std::set<std::string> UNARY_OP{"-", "~"};
const std::set<std::string> KEYWORD_CONSTANT{"true", "false", "null", "this"};

using Token = std::pair<TOKENTYPE, std::string>;


#endif