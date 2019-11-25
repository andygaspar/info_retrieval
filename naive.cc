
#include "make_lists.h"

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
}