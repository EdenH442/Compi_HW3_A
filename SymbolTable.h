#ifndef COMPI_HW3_SYMBOLTABLE_H
#define COMPI_HW3_SYMBOLTABLE_H

#include <string>
#include <vector>
#include <stack>
#include "TypeVar.h"

using std::string;

class symTableBlockEntry{
public:
    int offset;
    ExpType type;
    string id;
    std::vector<ExpType> types_vec;
    std::vector<string> names_vec;
    bool null_offset;
    bool is_func = false;

    symTableBlockEntry() = default;

    symTableBlockEntry(int offset, ExpType type, string id, bool null_offset, bool is_func) :
            offset(offset), type(type), id(id), null_offset(null_offset), is_func(is_func){};

    symTableBlockEntry(int offset, ExpType type, string id,std::vector<ExpType> types_vec, std::vector<string> names_vec,
                       bool null_offset, bool is_func) :
            offset(offset), type(type), id(id), types_vec(types_vec), names_vec(names_vec),
            null_offset(null_offset), is_func(is_func){};
};

typedef struct{
    std::vector<symTableBlockEntry> sym_tab;
    ExpType ret_type;
    bool is_while;
} symTableBlock;


typedef struct{
public:
    std::vector<symTableBlock> sym_tab_stack;
    std::stack<int> offset_stack;
    bool main_exists= false;
    int while_scope = 0;
} SemanticsManager;


void VarExistsInScope(std::shared_ptr<TypeVar> var, int lineno);
void CallFunction(std::shared_ptr<TypeVar> id_var,  std::shared_ptr<TypeVar> params,
                            std::shared_ptr<TypeVar> caller, int lineno);
void CheckPrevDeclID(std::shared_ptr<TypeVar> var, int lineno);
void InsertToSymTable(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar>id, bool func_type ,int lineno);
void ValidateAssign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> exp, int lineno);
void CheckVoidScope(int lineno);
void ValidateRetType(std::shared_ptr<TypeVar> var, int lineno);
void CheckWhileScope(int lineno, bool to_break);
void InsertParamsToSymTab(std::vector<string> var_names, std::vector<ExpType> var_types, int lineno);

void OpenNewBlock(bool is_while_scope, ExpType ret_type);
void CloseBlock();
void WhileBlockState(bool start_while);
void InsertFuncSymTab(ExpType type, string id, std::vector<string> names_vec, std::vector<ExpType> types_vec);
void InitMainScope();
void CloseMainScope();



#endif //COMPI_HW3_SYMBOLTABLE_H
