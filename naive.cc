#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <string>
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


string term_from_disk(int i);

// da sistemare il partition

//vedere perché non funziona il term from disk con lo while e la spaziatura

  

int partition (vector<int> &disk_index, int low, int high) 
{ 
    int pivot = disk_index[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high-1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (term_from_disk(disk_index[j]) <= term_from_disk(disk_index[pivot]) ) 
        { 
            i++;    // increment index of smaller element 
            std::iter_swap(disk_index.begin()+i, disk_index.begin()+high);  
        } 
    } 
    std::iter_swap(disk_index.begin()+i + 1, disk_index.begin()+high); 
    return (i + 1); 
} 

void quickSort(vector<int> &disk_index, int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, disk_index[p] is now 
           at right place */
        int pi = partition(disk_index, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(disk_index, low, pi - 1); 
        quickSort(disk_index, pi + 1, high); 
    } 
} 



vector<int> make_term_list (string file_name){
    std::ifstream g{file_name};
    std::ofstream save_term_toDisk{"terms_list.csv"};
    std::ofstream save_posting_toDisk{"posting_list.csv"};


    string line;
    string term = "";
    short int i;
    long int doc_ID=1;
    int disk_index=0;

    
    while(getline(g,line)) {
        i = 0 ;
        while( i < line.length()) {

            term=get_term(line,i); //get the terms starting from position i in line

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



string term_from_disk(int i){
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

  
// Driver program to test above functions 
int main() 
{ 
  
    index_ptr=make_term_list(file_name);
     
    
    size_t filesize = getFilesize();
    int fd = open(terms_list.c_str(), O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap
    void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(mmappedData != MAP_FAILED);

    chunk = reinterpret_cast<char*>(mmap(NULL, filesize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
    //void * adress=reinterpret_cast<void *> (&chunk[3]);


    for (int i=0; i<20; i++) 
    cout<<index_ptr[i]<<"  "<<term_from_disk(index_ptr[i])<<endl;

    quickSort(index_ptr,0,index_ptr.size()-1);

    for (int i=0; i<20; i++) 
    cout<<index_ptr[i]<<"  "<<term_from_disk(index_ptr[i])<<endl;

    //remove("posting_list.csv");
    //remove("terms_list.csv");

    return 0; 
} 