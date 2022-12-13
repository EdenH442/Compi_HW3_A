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
    VOID,
    EMPTY
};

class TypeVar {
public:
    std::string id;
    int value{};
    ExpType type;
    std::vector<std::string> var_names;
    std::vector<ExpType> var_types;

    TypeVar() = default;
    ~TypeVar() = default;

};

ExpType GetType(std::shared_ptr<TypeVar> const &var1, std::shared_ptr<TypeVar> const &var2);
void CheckIsNum(std::shared_ptr<TypeVar> const &var, int lineno);
void CheckIsBool(std::shared_ptr<TypeVar> const &var, int lineno);
void CheckAssign(std::shared_ptr<TypeVar> left_var, std::shared_ptr<TypeVar> right_var, int lineno);
void CheckByteOverflow(std::shared_ptr<TypeVar> var, int lineno);



#endif //COMPI_HW3_TYPEVAR_H
