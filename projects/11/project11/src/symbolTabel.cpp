#include"symbolTable.h"
SymbolTable::SymbolTable(){
    reset();
}

void SymbolTable::reset(){
    table.clear();
    argumentCount = 0;  localCount = 0;
    staticCount = 0;    fieldCount = 0;
}

void SymbolTable::define(std::string name, std::string type, VARIABLE_KIND kind){
    if(table.count(name)){
        std::cerr << "duplicate name" << std::endl;
        exit(EXIT_FAILURE);
    }
    size_t index;
    switch (kind)
    {
    case VARIABLE_KIND::ARG:
        index = argumentCount;
        argumentCount++;
        break;
    case VARIABLE_KIND::FIELD:
        index = fieldCount;
        fieldCount++;
        break;
    case VARIABLE_KIND::STATIC:
        index = argumentCount;
        argumentCount++;
        break;
    case VARIABLE_KIND::VAR:
        index = localCount;
        localCount++;
    default:
        break;
    }
    table[name] = {type, kind, index};
}

size_t SymbolTable::varCount(VARIABLE_KIND kind) const{
    switch(kind){
        case VARIABLE_KIND::ARG:
            return argumentCount;
        case VARIABLE_KIND::FIELD:
            return fieldCount;
        case VARIABLE_KIND::STATIC:
            return staticCount;
        case VARIABLE_KIND::VAR:
            return localCount;
        default:
            std::cerr << "Invalid variable kind" << std::endl;
            exit(EXIT_FAILURE);
    }
}

VARIABLE_KIND SymbolTable::kindOf(std::string name) const{
    if(!table.count(name)){
        std::cerr << "Cannot find variable " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return table.at(name).kind;
}
size_t SymbolTable::indexOf(std::string name) const{
    if(!table.count(name)){
        std::cerr << "Cannot find variable " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return table.at(name).index;
}

std::string SymbolTable::typeOf(std::string name) const{
    if(!table.count(name)){
        std::cerr << "Cannot find variable " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return table.at(name).type;
}

bool SymbolTable::contain(std::string name) const{
    return table.count(name);
}

std::string SymbolTable::strOfKind(VARIABLE_KIND kind){
    switch (kind){
    case VARIABLE_KIND::ARG:
        return "argument";
    case VARIABLE_KIND::VAR:
        return "local";
    case VARIABLE_KIND::FIELD:
        return "field";
    case VARIABLE_KIND::STATIC:
        return "static"; 
    default:
        return "";
    }
}