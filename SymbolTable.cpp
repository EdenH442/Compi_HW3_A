#include "SymbolTable.h"
#include <iostream>

SemanticsManager semnatic_manager;
vector<string> ConvertExpTypeToString(vector<ExpType> vec)
{
    vector<string> res;
    for (int i = 0; i < vec.size(); i++)
    {
        switch (vec[i])
        {
            case NONE:
              res.push_back("NONE");
              continue;
            case INT_EXP:
                res.push_back("INT");
                continue;
            case BYTE_EXP:
                res.push_back("BYTE");
                continue;
            case BOOL_EXP:
                res.push_back("BOOL");
                continue;
            case STRING_EXP:
                res.push_back("STRING");
                continue;
            case VOID:
                res.push_back("VOID");
                continue;
        }
    }
    return res;
}

static vector<string> ReverseVector(vector<ExpType> vec)
{
    vector<ExpType> reverse_vec;
    for (int i = vec.size() - 1; i >= 0; i--)
    {
        reverse_vec.push_back(vec[i]);
    }
    return ConvertExpTypeToString(reverse_vec);
}

void VarExistsInScope(std::shared_ptr<TypeVar> const &var, int lineno)
{
    bool found = false;
    symTableBlockEntry entry_found;
    for (auto blk = semnatic_manager.sym_tab_stack.rbegin();
         blk != semnatic_manager.sym_tab_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (!entry.is_func && entry.id == var->id)
            {
                found = true;
                entry_found = entry;
                break;
            }
        }
        if (found)
        {
            var->type = entry_found.type;
            return;
        }
    }
    output::errorUndef(lineno, var->id);
    exit(1);
}

void CallFunction(const std::shared_ptr<TypeVar>& id_var,  std::shared_ptr<TypeVar>& params,
                            std::shared_ptr<TypeVar>& caller, int lineno)
{
    symTableBlockEntry entry_found;
    auto curr_block = semnatic_manager.sym_tab_stack.front();
    bool found = false;
    for (auto entry: curr_block.sym_tab)
    {
        if (entry.is_func && entry.id == id_var->id)
        {
            found = true;
            entry_found = entry;
            caller->type = entry_found.type;
            break;
        }
    }
    if (!found)
    {
        output::errorUndefFunc(lineno, id_var->id);
        exit(1);
    }
    //////////////////////////////////////////////////////////////////

    for (int i = 0; i < params->var_names.size(); i++)
    {
        if (params->var_types[i] != NONE)
        {
            continue;
        }
        bool found = false;
        symTableBlockEntry entry_found;
        for (auto blk = semnatic_manager.sym_tab_stack.rbegin();
             blk != semnatic_manager.sym_tab_stack.rend(); blk++)
        {
            for (auto entry : blk->sym_tab)
            {
                if (entry.id == params->var_names[i])
                {
                    found = true;
                    entry_found = entry;
                    break;
                }
            }
            if (found)
            {
                params->var_types[i] = entry_found.type;
                break;
            }
        }

        if (!found || entry_found.is_func)
        {
            output::errorUndef(lineno, params->var_names[i]);
            exit(1);
        }
    }

    if (params->var_types.size() != entry_found.types_vec.size())
    {
        vector<string> reversedArgs = ReverseVector(entry_found.types_vec);
        output::errorPrototypeMismatch(lineno,id_var->id,reversedArgs);
        exit(1);
    }
    for (int i = 0; i < entry_found.types_vec.size(); i++)
    {
        ExpType call_type = params->var_types[i];
        ExpType sym_tab_type = entry_found.types_vec[i];
        if (!(sym_tab_type == call_type || (sym_tab_type == INT_EXP && call_type == BYTE_EXP)))
        {
            vector<string> reversedArgs = ReverseVector(entry_found.types_vec);
            output::errorPrototypeMismatch(lineno, id_var->id, reversedArgs);
            exit(1);
        }
    }
}

void CheckPrevDeclID(const std::shared_ptr<TypeVar>& var, int lineno)
{
    for (auto blk = semnatic_manager.sym_tab_stack.rbegin();
            blk !=semnatic_manager.sym_tab_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (entry.id == var->id)
            {
                output::errorDef(lineno, var->id);
                exit(1);
            }
        }
    }
}

void InsertToSymTable(const std::shared_ptr<TypeVar>& type, const std::shared_ptr<TypeVar>& id,
                      bool func_type ,int lineno)
{
    if (type->type == VOID )
    {
        output::errorMismatch(lineno);
        exit(1);
    }

    symTableBlockEntry curr_entry((semnatic_manager.offset_stack.top())++, type->type, id->id,
                                  false, func_type);
    semnatic_manager.sym_tab_stack.back().sym_tab.push_back(curr_entry);
}


