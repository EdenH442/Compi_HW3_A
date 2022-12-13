#include "TypeVar.h"

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
