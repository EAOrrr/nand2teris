#ifndef SYMBOLTABEL_H
#define SYMBOLTABEL_H

#include"tools.h"
class SymbolTable{
    public:
        SymbolTable();
        void reset();
        void define(std::string name, std::string type, VARIABLE_KIND);
        size_t varCount(VARIABLE_KIND) const;
        VARIABLE_KIND kindOf(std::string) const;
        std::string typeOf(std::string) const;
        size_t indexOf(std::string) const;
        bool contain(std::string) const;
        static std::string strOfKind(VARIABLE_KIND);
    private:
        struct variable{
            std::string type;
            VARIABLE_KIND kind;
            size_t index;
        };
        size_t argumentCount, localCount;
        size_t staticCount, fieldCount;
        std::map<std::string, variable> table;
};

#endif