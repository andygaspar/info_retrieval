#include "func.h"


struct temp_variables{
    string terms_list;
    string posting_list;
    char* terms_ptr;
    int* posting_list_ptr;
    vector<int> ter_index;
    vector<int> pos_index;

    temp_variables() {
        terms_list="terms_list_TEMP.csv";
        posting_list="posting_list_TEMP.csv";
        terms_ptr=nullptr;
        ter_index={};
        pos_index={};
    }

    ~temp_variables() {    
        remove(terms_list.c_str());
        remove(posting_list.c_str());
    }

};