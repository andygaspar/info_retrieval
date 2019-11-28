
#include "IR.h"


struct IR_naive:IR {
    IR_naive(): 
        IR{} {
            map_size=sqrt(getFilesize(term_to_postings_file)/sizeof(int));
            map=RAM_map(terms_map_file,terms_ptr);
        }
    ~IR_naive() {}

    vector<int> load_postings(int index) override{


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

    vector<int> search_word(string term) override{

        
        range range_term=map.search_range(term);
        int block_begin=*range_term.ptr;
        range_term.ptr++;
        int block_end=*(range_term.ptr);
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
        vector<int> v=load_postings(range_term.index*map_size+j);
        return v;
    }


   

};