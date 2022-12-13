#ifndef COMPI_HW3_TYPEVAR_H
#define COMPI_HW3_TYPEVAR_H

#include <string>
#include "hw3_output.hpp"
#include <iostream>
#include <memory>

#define YYSTYPE std::shared_ptr<Var>

enum ExpType {
    NONE,
    INT_EXP,
    BYTE_EXP,
    BOOL_EXP,
    STRING_EXP,
    VOID
};

class TypeVar {
    std::string id;
    int value;
    ExpType type;
    std::vector<string> var_names;
    std::vector<string> var_types;

    TypeVar() = default;
    ~TypeVar() = default;

};

void CheckIsNum(std::shared_ptr<TypeVar> var, int lineno);
void CheckIsBool(std::shared_ptr<TypeVar> var, int lineno);


#endif //COMPI_HW3_TYPEVAR_H
