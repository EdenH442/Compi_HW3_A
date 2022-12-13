#include "TypeVar.h"

ExpType GetType(std::shared_ptr<TypeVar> const &var1, std::shared_ptr<TypeVar> const &var2)
{
    if (var1->type == INT_EXP || var2->type == INT_EXP)
    {
        return INT_EXP;
    }
    else
    {
        return BYTE_EXP;
    }
}

void CheckIsNum(std::shared_ptr<TypeVar> const &var, int lineno)
{
    if(!(var->type == INT_EXP || var->type == BYTE_EXP || var->type == NONE))
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void CheckIsBool(std::shared_ptr<TypeVar> const &var, int lineno)
{
    if(! (var->type == BOOL_EXP || var->type == NONE))
    {
        output::errorMismatch(lineno);
        exit(1);
    }

}

void CheckAssign(std::shared_ptr<TypeVar> left_var, std::shared_ptr<TypeVar> right_var, int lineno)
{
    if(left_var->type == right_var->type)
    {
        return;
    }
    if(left_var->type == INT_EXP && right_var->type == BYTE_EXP)
    {
        return;
    }
    output::errorMismatch(lineno);
    exit(1);
}

void CheckByteOverflow(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->value > 0xFF)
    {
        output::errorByteTooLarge(lineno, to_string(var->value));
        exit(1);
    }
}


std::vector<string> PushBackVarID(const string& var, std::vector<string> vars_list)
{
    vars_list.push_back(var);
    return vars_list;
}

std::vector<ExpType> PushBackVarType(const ExpType& var_type, std::vector<ExpType> types_list)
{
    types_list.push_back(var_type);
    return types_list;
}

