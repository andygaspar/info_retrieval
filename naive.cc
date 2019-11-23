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
vector<int> index_ptr;
char* chunk=nullptr;
string file_name="../doc/doc_test_small.txt";

string terms_list="terms_list.csv";
string posting_list="posting_list.csv";
string terms_list_TEMP="terms_list_TEMP.csv";
string posting_list_TEMP="posting_list_TEMP.csv";

 
string get_term_from_disk(int i);



//vedere perché non funziona il term from disk con lo while e la spaziatura

  

 //quick sort ***********************
    int partition(vector<int> &disk_index,int first,int last) { 
    
        int i{first+1};
        int j{last};
        while(i<=j) {

            //the comparison is maid with the words written in the disk
            if (get_term_from_disk(disk_index[i])>get_term_from_disk(disk_index[first])) 
                {
                    std::iter_swap(disk_index.begin()+i, disk_index.begin()+j);
                    j--;
                    }
            else {
                    i++;
                }
        }
        std::iter_swap(disk_index.begin()+first, disk_index.begin()+j);
        return j;
    }

    void quick_sort_rec(vector<int> &disk_index,int first, int last) {
        if(first<last) {
            int pivot{partition(disk_index,first,last)};
            quick_sort_rec(disk_index,first,pivot-1);
            quick_sort_rec(disk_index,pivot+1,last);
        };
    }

    void quick_sort(vector<int> &disk_index) {
        if (disk_index.size()>0) {
            quick_sort_rec(disk_index,0,disk_index.size()-1);
        }
    }  





vector<int> make_term_list_TEMP (string file_name){
    std::ifstream g{file_name};
    std::ofstream save_term_toDisk{terms_list_TEMP};
    std::ofstream save_posting_toDisk{posting_list_TEMP};


    string line;
    string term = "";
    short int i;
    long int doc_ID=1;
    int disk_index=0;

    
    while(getline(g,line)) {
        i = 0 ;
        while( i < line.length()) {

            term=get_term(line,i); //get the term starting from position i in line

            if ( is_valid_term(term)) { 
                
                save_term_toDisk<<term<<' ';
                save_posting_toDisk<<doc_ID<<endl;

                index_ptr.push_back(disk_index); //save term's beginnig index
                disk_index+=term.length()+1;
            }

            term="";           
            i++;
        }
        term="";
        doc_ID++;
    }

    g.close();

    return index_ptr;
} 



string get_term_from_disk(int i){
    string term="";
    while(chunk[i]!=' '){
        term+=chunk[i];
        i++;
    }
    return term;
}

size_t getFilesize() {
    struct stat st;
    stat(file_name.c_str(), &st);
    return st.st_size;
}

char * set_disk_ptr() {
    size_t filesize = getFilesize();
    int fd = open(terms_list_TEMP.c_str(), O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap
    void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(mmappedData != MAP_FAILED);

   return reinterpret_cast<char*>(mmap(NULL, filesize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
  }
// Driver program to test above functions 
int main() 
{ 
  
    index_ptr=make_term_list_TEMP(file_name);
     
    chunk=set_disk_ptr();

    quick_sort(index_ptr);
 

    std::ofstream ter_list(terms_list);
    std::ofstream pos_list(posting_list);
    string current="";
    string prev="";

    //writing 
    ter_list<<get_term_from_disk(index_ptr[0])<<endl;

    for(int i=1; i< index_ptr.size(); i++) {
        current=get_term_from_disk(index_ptr[i]);
        prev=get_term_from_disk(index_ptr[i-1]);

        if(current!=prev)
            ter_list<<current<<endl;        
    }

    remove(terms_list_TEMP.c_str());
    remove(posting_list_TEMP.c_str());


    return 0; 
} 