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
    string type;
    string id;
    std::vector<string> types_vec;
    std::vector<string> names_vec;
    bool null_offset;
    bool is_func = false;

    symTableBlockEntry() = default;

    symTableBlockEntry(int offset, string type, string id, bool null_offset, bool is_func) :
            offset(offset), type(type), id(id), null_offset(null_offset), is_func(is_func){};

    symTableBlockEntry(int offset, string type, string id,std::vector<string> types_vec, std::vector<string> names_vec,
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
    std::vector<symTableBlock> sym_tab_stack;
    std::stack<int> offset_stack;
    bool main_exists;
//    int while_scope = 0;
} SemanticsManager;


#endif //COMPI_HW3_SYMBOLTABLE_H
