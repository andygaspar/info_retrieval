#include "make_dict_func.h"




struct RAM_map{
    vector<string> terms_map;
    vector<int> indexes;
    //vector<u_char*> terms_comp_adrss;
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

 
    RAM_map(string& terms_map_file,u_char* terms_comp_ptr)   {
        
        len=getFilesize(terms_map_file)/sizeof(int); //number of elements of map
        u_char term_len;
        int* term_index=set_disk_ptr<int>(terms_map_file);
 
        char* term_ptr;
        string term="";

        for (int i=0; i<len; i++) {

            indexes.push_back(term_index[i]);
            term_len=terms_comp_ptr[term_index[i]];

            //the actual term's begin is one step ahead
            term_ptr=reinterpret_cast<char*>(& terms_comp_ptr[term_index[i]]);
            term_ptr++;

            //getting the term
            for(int j=0;j<term_len;j++){
                term+=*term_ptr;
                term_ptr++;
            }
            
            terms_map.push_back(term);
            term="";
        }
    }
 
    
    ~RAM_map() {}

    int search_range(string t){
        int i=0;
        int last=len-2;

        if(t<terms_map[0]) {
            perror("term not found");
            exit(1);
        }

        if(t>=terms_map[last]) return last;


        while(!(t>=terms_map[i] and t<terms_map[i+1])) {
            if (t>=terms_map[(i+last)/2]) i=(i+last)/2;
            else last=(i+last)/2;
        }

        return i;
    }


    


};



std::ostream& operator<<(std::ostream& os, RAM_map& m){
    for(int i=0; i<m.len;i++){
        os<<m.terms_map[i]<<"  "<<m.indexes[i]<<endl;
    }
    return os;
}