#include "make_dict_func.h"




struct range{
    int* ptr;
    int index;

    range(int* p,int i): ptr{p}, index{i} {}
    ~range() {}
};

struct RAM_map{
    vector<string> terms_map;
    vector<int> indexes;
    int len;

    RAM_map() {}

    RAM_map(string& terms_map_file, char* terms_ptr)   {
        
        len=getFilesize(terms_map_file)/sizeof(int);
        int* p=set_disk_ptr<int>(terms_map_file);

        for (int i=0; i<len; i++) {
            terms_map.push_back(get_term_from_disk(p[i],terms_ptr));
            indexes.push_back(p[i]);
        }
    }
    ~RAM_map() {}

    range search_range(string t){
        int i=0;
        int last=len-2;

        if(t<terms_map[0]) {
            perror("term not found");
            exit(1);
        }

        if(t>terms_map[last]) return range(&indexes[last],last);


        while(!(t>=terms_map[i] and t<terms_map[i+1])) {
            if (t>=terms_map[last/2]) i=(i+last)/2;
            else last=(i+last)/2;
        }

        return range(&indexes[i],i);
    }


    


};



std::ostream& operator<<(std::ostream& os, RAM_map& m){
    for(int i=0; i<m.len;i++){
        os<<m.terms_map[i]<<"  "<<m.indexes[i]<<endl;
    }
    return os;
}