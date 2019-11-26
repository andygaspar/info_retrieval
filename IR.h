//abstract class
#include "set_files.h"

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
        int last=len-1;
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

struct IR{
    string terms_file;
    string posting_list_file;
    string term_to_postings_file;
    string terms_map_file;
    int block_size;
    char* terms_ptr;
    RAM_map map;

    IR(string tf,string plf,string ttpf,string tmf): 
        terms_file{tf},
        posting_list_file{plf},
        term_to_postings_file{ttpf},
        terms_map_file{tmf} {
            block_size=sqrt(getFilesize(term_to_postings_file)/sizeof(int));
            terms_ptr=set_disk_ptr<char>(terms_file);
            map=RAM_map(terms_map_file,terms_ptr);
        }
    ~IR() {}

    void compression();
    vector<int> load_postings(int i);
    string search(string term);

};