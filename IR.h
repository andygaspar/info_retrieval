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

    virtual void compression(int k=4) {}
    virtual vector<int> search_word(string term) {}



    virtual vector<int> load_postings(int index) {
        int* ter_to_pos_ptr=set_disk_ptr<int>(term_to_postings_file);
        int* disk_pos_ptr=set_disk_ptr<int>(posting_list_file);
        vector<int> postings;

        int i=ter_to_pos_ptr[index];
        int end=ter_to_pos_ptr[index+1];

        while(&disk_pos_ptr[i]!=&disk_pos_ptr[end]){
            postings.push_back(disk_pos_ptr[i]);
            i++;
        }
        return postings;
    }

    virtual vector<int> search(string a,string b,string CASE) {
        vector<int> p_a=search_word(a);
        vector<int> p_b=search_word(b);
        vector<int> res;
        int i=0;
        int j=0;
        while (i<p_a.size() and j<p_b.size()){
            if(p_a[i]==p_b[j]) {
                if (CASE=="OR" or CASE=="AND") res.push_back(p_a[i]);
                i++;
                j++;
            }
            else if(p_a[i]<p_b[j]) {
                if (CASE=="OR" or CASE=="NOT") res.push_back(p_a[i]);
                i++;
            }
            else {
                if (CASE=="OR") res.push_back(p_b[j]);
                j++;
            }
        }
        return res;

    }

};