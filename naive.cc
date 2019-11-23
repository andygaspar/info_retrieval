#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <iterator>
#include <vector>
#include <ctype.h>
#include <locale>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include "naive_dict.h" 


using std::cout;
using std::endl;
using std::string;
using std::vector;

//enviromental variables
vector<int> ter_index_ptr;
vector<int> pos_index_ptr;
char* text_ptr=nullptr;
char* doc_ID_ptr=nullptr;
string text_file_name="../doc/doc_test_small.txt";


string terms_list="terms_list.csv";
string posting_list="posting_list.csv";
string terms_list_TEMP="terms_list_TEMP.csv";
string posting_list_TEMP="posting_list_TEMP.csv";

 
string get_term_from_disk(int i);



//vedere perché non funziona il term from disk con lo while e la spaziatura

  

 //quick sort ***********************
    int partition(int first,int last) { 
    
        int i{first+1};
        int j{last};
        while(i<=j) {

            //the comparison is maid with the words written in the disk
            if (get_term_from_disk(ter_index_ptr[i])>get_term_from_disk(ter_index_ptr[first])) 
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
        return j;
    }

    void quick_sort_rec(int first, int last) {
        if(first<last) {
            int pivot{partition(first,last)};
            quick_sort_rec(first,pivot-1);
            quick_sort_rec(pivot+1,last);
        };
    }

    void quick_sort() {
        if (ter_index_ptr.size()>0) {
            quick_sort_rec(0,ter_index_ptr.size()-1);
        }
    }  





void make_term_list_TEMP (string file_name){
    std::ifstream g{file_name};
    std::ofstream save_term_toDisk{terms_list_TEMP};
    std::ofstream save_posting_toDisk{posting_list_TEMP};


    string line;
    string term = "";
    short int i;
    int doc_ID=1;
    string doc="1";
    int ter_index=0;
    int pos_index=0;

    
    while(getline(g,line)) {
        i = 0 ;
        while( i < line.length()) {

            term=get_term(line,i); //get the term starting from position i in line

            if ( is_valid_term(term)) { 
                
                save_term_toDisk<<term<<' ';
                save_posting_toDisk<<std::to_string(doc_ID)<<' ';

                ter_index_ptr.push_back(ter_index); //save term's beginnig index
                pos_index_ptr.push_back(pos_index);
                ter_index+=term.length()+1;
                pos_index+=(std::to_string(doc_ID)).length()+1;
            }

            term="";           
            i++;
        }
        term="";
        doc_ID++;

    }

    g.close();

} 



string get_term_from_disk(int i){
    string term="";
    while(text_ptr[i]!=' '){
        term+=text_ptr[i];
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
    /*
    void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(mmappedData != MAP_FAILED);*/

   return reinterpret_cast<T*>(mmap(NULL, filesize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
  }



int main() 
{   remove(terms_list.c_str());
    remove(posting_list.c_str());
  
    make_term_list_TEMP(text_file_name);
     
    text_ptr=set_disk_ptr<char>(terms_list_TEMP);

    doc_ID_ptr=set_disk_ptr<char>(posting_list_TEMP);

    quick_sort();
 

    std::ofstream ter_list(terms_list);
    std::ofstream pos_list(posting_list);



    string current="";
    string prev="";

    ter_list<<get_term_from_disk(ter_index_ptr[0]);
    pos_list<<get_doc_ID_from_disk(pos_index_ptr[0]);

    for(int i=1; i< ter_index_ptr.size(); i++) {
        current=get_term_from_disk(ter_index_ptr[i]);
        prev=get_term_from_disk(ter_index_ptr[i-1]);

        if(current!=prev){
            ter_list<<endl<<current;
            pos_list<<endl<<get_doc_ID_from_disk(pos_index_ptr[i]);
            //pos_list.write((const char*)&ter_index_ptr[i],sizeof(int));//488
        }

        else pos_list<<" "<<get_doc_ID_from_disk(pos_index_ptr[i]);
        

    }

    remove(terms_list_TEMP.c_str());
    remove(posting_list_TEMP.c_str());





    return 0; 
} 