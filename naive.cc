
#include "IR.h"


struct IR_naive:IR {
    IR_naive(): 
        IR{"terms_list.csv","posting_list.csv","term_to_postings.csv","terms_map.csv"} {}
    ~IR_naive() {}

    vector<int> load_postings(int index) {
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

    vector<int> search_word(string term){
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
        vector<int> v=load_postings(ran.index*block_size+j);
        return v;
    }

};




int main(){
    set_files();

    

    IR_naive N;


    cout<<N.search_word("l");
    
    
}