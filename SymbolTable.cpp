#include "SymbolTable.h"
#include <iostream>
#include <unordered_set>

SemanticsManager semnatic_manager;

string EnumToString(ExpType type)
{
    switch (type) {
        case NONE:
            return "NONE";
        case INT_EXP:
            return "INT";
        case BYTE_EXP:
            return "BYTE";
        case BOOL_EXP:
            return "BOOL";
        case STRING_EXP:
            return "STRING";
        case VOID:
            return "VOID";
        case EMPTY:
            return "";
    }
}

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
            case EMPTY:
                res.push_back("");
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

void VarExistsInScope(std::shared_ptr<TypeVar> var, int lineno)
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

void CallFunction(std::shared_ptr<TypeVar> id_var,  std::shared_ptr<TypeVar> params,
                            std::shared_ptr<TypeVar> caller, int lineno)
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

void CheckPrevDeclID(std::shared_ptr<TypeVar> var, int lineno)
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

void InsertToSymTable(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar>id, bool func_type ,int lineno)
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

void ValidateAssign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> exp, int lineno)
{
    bool found = false;
    symTableBlockEntry entry_found;
    for (auto blk = semnatic_manager.sym_tab_stack.rbegin();
            blk != semnatic_manager.sym_tab_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (entry.id == var->id)
            {
                found = true;
                entry_found = entry;
                break;
            }
        }
        if (found)
        {
            break;
        }
    }

    if (!found || entry_found.is_func)
    {
        output::errorUndef(lineno, var->id);
        exit(1);
    }
    if (entry_found.type != exp->type && !(entry_found.type == INT_EXP && exp->type == BYTE_EXP))
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void CheckVoidScope(int lineno)
{
    if (semnatic_manager.sym_tab_stack.back().ret_type != VOID)
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void ValidateRetType(std::shared_ptr<TypeVar> var, int lineno)
{
    ExpType type = var->type;
    if (!var->id.empty())
    {
        bool found = false;
        symTableBlockEntry entry_found;
        for (auto blk = semnatic_manager.sym_tab_stack.rbegin();
             blk != semnatic_manager.sym_tab_stack.rend(); blk++)
        {
            for (auto entry : blk->sym_tab)
            {
                if (entry.id == var->id)
                {
                    found = true;
                    entry_found = entry;
                    break;
                }
            }
            if (found)
            {
                type = entry_found.type;
                break;
            }
        }
    }

    if (semnatic_manager.sym_tab_stack.back().ret_type != type &&
                !(semnatic_manager.sym_tab_stack.back().ret_type == INT_EXP && var->type == BYTE_EXP))
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void CheckWhileScope(int lineno, bool to_break)
{
    if (!semnatic_manager.while_scope)
    {
        if (to_break)
        {
            output::errorUnexpectedBreak(lineno);
            exit(1);

        }
        else{
            output::errorUnexpectedContinue(lineno);
            exit(1);
        }
    }
}

void InsertParamsToSymTab(std::vector<string> var_names, std::vector<ExpType> var_types, int lineno)
{
    int counter = 0;
    std::unordered_set<string> params;
    for (int i = var_names.size() - 1; i >= 0; i--)
    {
        if (params.find(var_names[i]) != params.end()){
            output::errorDef(lineno, var_names[i]);
            exit(1);
        }
        counter--;
        params.insert(var_names[i]);
        symTableBlockEntry entry(counter, var_types[i], var_names[i], false, false);
        semnatic_manager.sym_tab_stack.back().sym_tab.push_back(entry);
    }
}



void OpenNewBlock(bool is_while_scope, ExpType ret_type)
{
    symTableBlock blk;
    if (is_while_scope )
    {
        blk.is_while = true;
    }
    else if (!semnatic_manager.sym_tab_stack.empty() && semnatic_manager.sym_tab_stack.back().is_while)
    {
        blk.is_while = true;
    }
    else {
        blk.is_while = false;
    }

    if (ret_type != EMPTY)
    {
        blk.ret_type = ret_type != EMPTY ? ret_type : semnatic_manager.sym_tab_stack.back().ret_type;
    }
    else if (!semnatic_manager.sym_tab_stack.empty() && !semnatic_manager.sym_tab_stack.back().ret_type == EMPTY)
    {
        blk.ret_type = ret_type != EMPTY ? ret_type : semnatic_manager.sym_tab_stack.back().ret_type;
    }
    else {
        blk.ret_type = EMPTY;
    }

    semnatic_manager.sym_tab_stack.push_back(blk);
    if (semnatic_manager.offset_stack.empty())
    {
        semnatic_manager.offset_stack.push(0);
    }
    else
    {
        semnatic_manager.offset_stack.push(semnatic_manager.offset_stack.top());
    }
}

void CloseBlock()
{
    semnatic_manager.offset_stack.pop();
    output::endScope();
    for (auto entry : semnatic_manager.sym_tab_stack.back().sym_tab)
    {
        string type = EnumToString(entry.type);
        if (entry.is_func)
        {
            vector<string> reversed_vector = ReverseVector(entry.types_vec);
            string type_string = EnumToString(entry.type);
            type = output::makeFunctionType(type_string,reversed_vector);
        }
        int offset_to_print = entry.null_offset ? 0 : entry.offset;
        output::printID(entry.id, offset_to_print, type);
    }
    semnatic_manager.sym_tab_stack.pop_back();
}

void WhileBlockState(bool start_while)
{
    if(start_while)
    {
        semnatic_manager.while_scope +=1;
        return;
    }
    semnatic_manager.while_scope -=1;
}

void InsertFuncSymTab(ExpType type, string id, std::vector<string> names_vec, std::vector<ExpType> types_vec)
{
    if (id == "main" && type == VOID && types_vec.empty())
    {
        semnatic_manager.main_exists = true;
    }
    symTableBlockEntry entry((int)0, type, id, types_vec, names_vec, true , true);
    semnatic_manager.sym_tab_stack.back().sym_tab.push_back(entry);
    entry.is_func = true;
}

void InitMainScope()
{
    OpenNewBlock(false, EMPTY);
    InsertFuncSymTab(VOID, "print", {"input"}, {STRING_EXP});
    InsertFuncSymTab(VOID, "printi", {"input"}, {INT_EXP});
}

void CloseMainScope()
{
    if (!semnatic_manager.main_exists)
    {
        output::errorMainMissing();
        exit(1);
    }
    CloseBlock();
}




