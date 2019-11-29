
#include "temp_variables.h" 










// methods list *****************
 
string get_term_from_disk(int i,char* ptr_to_disk);
string get_doc_ID_from_disk(int i);
void quick_sort_file();

size_t getFilesize(string &file_name);
template<class T>
T * set_disk_ptr(string &file_name);

int sort_and_save_postings(vector<int> postings,std::ofstream &disk_ptr);



  


 //quick sort ***********************
    int partition_file(int first,int last,temp_variables& TEMP) { 
    
        int i{first+1};
        int j{last};
        while(i<=j) {

            //the comparison is maid with the words written in the disk
            if (get_term_from_disk(TEMP.ter_index[i],TEMP.terms_ptr)>get_term_from_disk(TEMP.ter_index[first],TEMP.terms_ptr)) 
                {
                    std::iter_swap(TEMP.ter_index.begin()+i, TEMP.ter_index.begin()+j);
                    std::iter_swap(TEMP.pos_index.begin()+i, TEMP.pos_index.begin()+j);
                    j--;
                    }
            else {
                    i++;
                }
        }
        std::iter_swap(TEMP.ter_index.begin()+first, TEMP.ter_index.begin()+j);
        std::iter_swap(TEMP.pos_index.begin()+first, TEMP.pos_index.begin()+j);
        return j;
    }

    void quick_sort_rec_file(int first, int last,temp_variables& TEMP) {
        if(first<last) {
            int pivot{partition_file(first,last,TEMP)};
            quick_sort_rec_file(first,pivot-1,TEMP);
            quick_sort_rec_file(pivot+1,last,TEMP);
        };
    }

    void quick_sort_file(temp_variables& TEMP) {
        if (TEMP.ter_index.size()>0) {
            quick_sort_rec_file(0,TEMP.ter_index.size()-1,TEMP);
        }
    }  









string get_term_from_disk(int i, char* ptr_to_disk){
    string term="";

    if (ptr_to_disk[i]=='#') return "#";

    while(ptr_to_disk[i]!=' '){
        term+=ptr_to_disk[i];
        i++;
    }
    return term;
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







