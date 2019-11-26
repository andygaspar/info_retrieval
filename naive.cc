
#include "IR.h"


struct IR_naive:IR {
    IR_naive(): 
        IR{"terms_list.csv","posting_list.csv","term_to_postings.csv","terms_ptrs.csv"} {}
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

    int* doc_ID_ok_ptr=set_disk_ptr<int>(posting_list);
    int* ter_to_pos_ptr=set_disk_ptr<int>(term_to_postings);
    int* p=set_disk_ptr<int>(terms_ptrs);
    //1 3 1 3 1 3
    for (int i=ter_to_pos_ptr[3];i<ter_to_pos_ptr[4];i++) cout<<doc_ID_ok_ptr[i]<<endl;
    cout<<endl<<endl;


    int num_elem=getFilesize(terms_ptrs)/sizeof(int);
    for (int i=0; i<num_elem; i++)
        cout<<get_term_from_disk(p[i],terms_ptr)<<endl;

    string s="terms_list.csv";
    char* pippo=set_disk_ptr<char>(s);
    string f="terms_ptrs.csv";
    RAM_map M(f,pippo);
    cout<<endl<<M<<endl;
    cout<<(M.search_range("ciccio")).index<<endl;
    

    IR_naive N;
    vector<int> v=N.load_postings(17+9);
    cout<<v;

    cout<<N.search_word("l");
    
    
}