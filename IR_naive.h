
#include "IR.h"


struct IR_naive:IR {
    IR_naive(): 
        IR{"terms_list.csv","posting_list.csv","term_to_postings.csv","terms_map.csv"} {}
    ~IR_naive() {}

    

    vector<int> search_word(string term) override{
        range ran=map.search_range(term);
        int block_begin=*ran.ptr;
        ran.ptr++;
        int block_end=*(ran.ptr);
        string word="";
        int i=block_begin;
        int j=-1;
        bool found=false;
        while(i<block_end and found==false){
            
            if (terms_ptr[i]==' ') {
                j++;
                if (word==term) found=true;
                word="";
            }      
            else word+=terms_ptr[i];    
            i++;            
        }
        if(i==block_end) {
            perror("term__not found");
            exit(1);
        }
        vector<int> v=load_postings(ran.index*block_size+j);
        return v;
    }


   

};