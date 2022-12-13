#ifndef COMPI_HW3_SYMBOLTABLE_H
#define COMPI_HW3_SYMBOLTABLE_H

#include <stack>

class symTableBlockEntry
{
public:
    int offset;
    string type;
    string id;
    std::vector<string> types_vec;
    std::vector<string> names_vec;
    bool null_offset;
    bool is_func = false;

    symTableBlkEntry() = default;

    symTableBlkEntry(int offset, string id, string type, bool null_offset, bool is_func) :
            offset(offset), id(id), type(type),null_offset(null_offset), is_func(is_func){}

    symTableBlkEntry(int offset, string id, std::vector<string> names_vec, std::vector<string> types_vec, string type, bool null_offset, bool is_func) :
            offset(offset), id(id),names_vec(names_vec), types_vec(types_vec), type(type), null_offset(null_offset), is_func(is_func){}
};

typedef struct{
    vector<symTableBlockEntry> sym_tab;
    string ret_type;
    bool is_while;
} symTableBlock;

typedef struct{
    std::vector<symTableBlock> sym_tab_stack;
    std::stack<int> offset_stack;
    bool main_exists = false;
//    int while_scope = 0;
} SemanticsManager;


#endif //COMPI_HW3_SYMBOLTABLE_H
