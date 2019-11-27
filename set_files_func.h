#include <algorithm>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>
#include <bits/stdc++.h>
#include "func.h" 



//enviromental variables ********

vector<int> ter_index_ptr;
vector<int> pos_index_ptr;
char* text_ptr=nullptr;
int* doc_ID_ptr=nullptr;
char* terms_ptr=nullptr;
string text_file_name="../doc/doc_test_small.txt";


string terms_list="terms_list.csv";
string posting_list="posting_list.csv";
string terms_map="terms_map.csv";
string term_to_postings="term_to_postings.csv";

string terms_list_TEMP="terms_list_TEMP.csv";
string posting_list_TEMP="posting_list_TEMP.csv";





// methods list *****************
 
string get_term_from_disk(int i,char* ptr_to_disk);
string get_doc_ID_from_disk(int i);
void quick_sort_file();

size_t getFilesize(string &file_name);
template<class T>
T * set_disk_ptr(string &file_name);

int sort_and_save_postings(vector<int> postings,std::ofstream &disk_ptr);



  


 //quick sort ***********************
    int partition_file(int first,int last) { 
    
        int i{first+1};
        int j{last};
        while(i<=j) {

            //the comparison is maid with the words written in the disk
            if (get_term_from_disk(ter_index_ptr[i],text_ptr)>get_term_from_disk(ter_index_ptr[first],text_ptr)) 
                {
                    std::iter_swap(ter_index_ptr.begin()+i, ter_index_ptr.begin()+j);
                    std::iter_swap(pos_index_ptr.begin()+i, pos_index_ptr.begin()+j);
                    j--;
                    }
            else {
                    i++;
                }
        }
        std::iter_swap(ter_index_ptr.begin()+first, ter_index_ptr.begin()+j);
        std::iter_swap(pos_index_ptr.begin()+first, pos_index_ptr.begin()+j);
        return j;
    }

    void quick_sort_rec_file(int first, int last) {
        if(first<last) {
            int pivot{partition_file(first,last)};
            quick_sort_rec_file(first,pivot-1);
            quick_sort_rec_file(pivot+1,last);
        };
    }

    void quick_sort_file() {
        if (ter_index_ptr.size()>0) {
            quick_sort_rec_file(0,ter_index_ptr.size()-1);
        }
    }  









string get_term_from_disk(int i, char* ptr_to_disk){
    string term="";
    while(ptr_to_disk[i]!=' '){
        term+=ptr_to_disk[i];
        i++;
    }
    return term;
}

string get_doc_ID_from_disk(int i){
    string d_ID="";
    while(doc_ID_ptr[i]!=' '){
        d_ID+=doc_ID_ptr[i];
        i++;
    }
    return d_ID;
}







size_t getFilesize(string &file_name) {
    struct stat st;
    stat(file_name.c_str(), &st);
    return st.st_size;
}


template<class T>

T * set_disk_ptr(string &file_name) {
    size_t filesize = getFilesize(file_name);

    int fd = open(file_name.c_str(), O_RDONLY, 0);
    assert(fd != -1);
    
    T* ptr=(T*)(mmap(NULL, filesize, PROT_READ,  MAP_SHARED, fd, 0));
    if(ptr==MAP_FAILED) cout<<"problemi"<<endl;
    
    return ptr;
 
}



  



int sort_and_save_postings(vector<int> postings,std::ofstream &disk_ptr) {
    
    if (postings.size()==0) return 0;

    int counter=1;

    sort(postings.begin(),postings.end());
    disk_ptr.write((const char*)&postings[0],sizeof(int));
    for(int i=1; i<postings.size();i++) {
        if(postings[i]!=postings[i-1]) {
            disk_ptr.write((const char*)&postings[i],sizeof(int));
            counter++;
        }
    }
    return counter;
}







