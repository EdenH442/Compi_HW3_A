#ifndef COMPI_HW3_TYPEVAR_H
#define COMPI_HW3_TYPEVAR_H

#define YYSTYPE std::shared_ptr<Var>

#include <string>
#include "hw3_output.hpp"
#include <iostream>
#include <memory>



enum ExpType {
    NONE,
    INT_EXP,
    BYTE_EXP,
    BOOL_EXP,
    STRING_EXP,
    VOID
};

class TypeVar {
public:
    std::string id;
    int value{};
    ExpType type;
    std::vector<std::string> var_names;
    std::vector<std::string> var_types;

    TypeVar() = default;
    ~TypeVar() = default;

};

void CheckIsNum(std::shared_ptr<TypeVar> const &var, int lineno);
void CheckIsBool(std::shared_ptr<TypeVar> const &var, int lineno);
void CheckAssign(std::shared_ptr<TypeVar> left_var, std::shared_ptr<TypeVar> right_var, int lineno);



#endif //COMPI_HW3_TYPEVAR_H
