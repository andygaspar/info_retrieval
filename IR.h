//abstract class
#include "RAM_map.h"





struct IR{
    string documents_file;
    string terms_file;
    string posting_list_file;
    string term_to_postings_file;
    string terms_map_file;
    int num_terms;
    int block_size;

    char* terms_ptr;
    RAM_map map;


    bool files_present();

    void make_term_list_TEMP (temp_variables& TEMP);
    int make_Ter_Posts(temp_variables& TEMP);
    void make_ptr_to_terms_list();
    void set_dictonary_files();
    



    IR() {

            documents_file         ="../doc/doc_test_small.txt";
            terms_file             ="terms_list.csv";
            posting_list_file      ="posting_list.csv";
            term_to_postings_file  ="term_to_postings.csv";
            terms_map_file         ="terms_map.csv";

            if (!files_present()) set_dictonary_files();

            block_size=sqrt(getFilesize(term_to_postings_file)/sizeof(int));
            terms_ptr=set_disk_ptr<char>(terms_file);
            map=RAM_map(terms_map_file,terms_ptr);
        }


    ~IR() {}

    virtual void compression(int k=4) {}
    virtual vector<int> search_word(string term) {}



    virtual vector<int> load_postings(int index) {


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




    virtual vector<int> search(string a,string b,string CASE) {


        vector<int> p_a=search_word(a);
        vector<int> p_b=search_word(b);
        vector<int> res;
        int i=0;
        int j=0;
        while (i<p_a.size() and j<p_b.size()){
            if(p_a[i]==p_b[j]) {
                if (CASE=="OR" or CASE=="AND") res.push_back(p_a[i]);
                i++;
                j++;
            }
            else if(p_a[i]<p_b[j]) {
                if (CASE=="OR" or CASE=="NOT") res.push_back(p_a[i]);
                i++;
            }
            else {
                if (CASE=="OR") res.push_back(p_b[j]);
                j++;
            }
        }
        return res;

    }

};










void IR::set_dictonary_files() {

    temp_variables TEMP;
    
    make_term_list_TEMP(TEMP);

    
    TEMP.terms_ptr=set_disk_ptr<char>(TEMP.terms_list);
    TEMP.posting_list_ptr=set_disk_ptr<int>(TEMP.posting_list);
    
    quick_sort_file(TEMP);

    num_terms=make_Ter_Posts(TEMP);

    terms_ptr=set_disk_ptr<char>(terms_file);
    make_ptr_to_terms_list();   
    
} 




void IR::make_term_list_TEMP (temp_variables& TEMP){

    std::ifstream documents{documents_file};
    std::ofstream save_term_toDisk{TEMP.terms_list};
    std::ofstream save_posting_toDisk{TEMP.posting_list};


    string line;
    string term = "";
    long int i;
    int doc_ID=1;

    int ter_index=0;  //positional index of the TERM into file_temp
    int pos_index=0;  //positional index of the POSTING into file_temp

    
    while(getline(documents,line)) {
        i = 0 ;
        while( i < line.length()) {

            term=get_term(line,i); //get the term starting from position i in line

            if ( is_valid_term(term)) { 
                
                save_term_toDisk<<term<<' ';
                save_posting_toDisk.write((const char*)&doc_ID,sizeof(int));

                TEMP.ter_index.push_back(ter_index); //save term's beginnig index
                TEMP.pos_index.push_back(pos_index);
                ter_index+=term.length()+1;
                pos_index++;
            }

            term="";           
            i++;
        }
        term="";
        doc_ID++;

    }


    documents.close();
    save_term_toDisk.close();
    save_posting_toDisk.close();

} 


int IR::make_Ter_Posts(temp_variables& TEMP) {

    std::ofstream save_term_toDisk(terms_file);
    std::ofstream save_posting_toDisk(posting_list_file, std::ios::binary);
    std::ofstream save_tp_ptr_toDisk(term_to_postings_file, std::ios::binary);


    string current="";
    string prev="";
    vector<int> postings;
    int doc_ID;
    int terms_counter=0;
    int index=0;
    


    for(int i=0; i< TEMP.ter_index.size(); i++) {
        current=get_term_from_disk(TEMP.ter_index[i],TEMP.terms_ptr);
        doc_ID=TEMP.posting_list_ptr[TEMP.pos_index[i]];

        if(current!=prev){
            terms_counter++;
            save_term_toDisk<<current<<" ";
            index+=sort_and_save_postings(postings,save_posting_toDisk);
            save_tp_ptr_toDisk.write((const char*)&index,sizeof(int));
            postings={doc_ID};
        }

        else {postings.push_back(doc_ID);}

        prev=current;

    }


    //last element and file closure
    sort_and_save_postings(postings,save_posting_toDisk);
    index++;
    save_tp_ptr_toDisk.write((const char*)&index,sizeof(int));

    save_term_toDisk<<"#";
    save_term_toDisk.close();
    save_posting_toDisk.close();
    save_tp_ptr_toDisk.close();

    TEMP.ter_index.clear();
    TEMP.pos_index.clear();

    return terms_counter;
}


void IR::make_ptr_to_terms_list(){


    int size_map=sqrt(num_terms);
    int terms_counter=0;
    int i=0;

    string term="";

    std::ofstream t_p(terms_map_file,std::ios::binary);

    while(terms_counter <= num_terms-size_map){
        term=get_term_from_disk(i,terms_ptr);

        if (terms_counter % size_map==0)  {
            t_p.write((const char*)&i,sizeof(int));
        }
        terms_counter++;
        i+=term.length()+1;
    }

    while(terms_ptr[i]!='#') {
        i++;
    }

    t_p.write((const char*)&i,sizeof(int));

    t_p.close();

}






inline bool IR::files_present () {

    vector<string> all_files={documents_file,terms_file,posting_list_file,term_to_postings_file,terms_map_file};
    for(string file : all_files) {
        if( access( documents_file.c_str(), F_OK ) != -1 ) return false;
    }

    return true;
}