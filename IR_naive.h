
#include "IR.h"


struct IR_naive:IR {
    IR_naive(): 
        IR{} {
            map_size=sqrt(getFilesize(term_to_postings_file)/sizeof(int));
            map=RAM_map(terms_map_file,terms_ptr);
        }
    ~IR_naive() {}


    vector<int> search_word(string term) override{

        
        int range_index=map.search_range(term);
        int block_begin=map.indexes[range_index];
        int block_end=map.indexes[range_index+1];
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
        if(i>=block_end and found==false) {
            perror("term__not found");
            exit(1);
        }
        vector<int> v=load_postings(range_index*map_size+j);
        return v;
    }


   

};