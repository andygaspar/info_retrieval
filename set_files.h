#include "set_files_func.h"






//methods list ******
void make_term_list_TEMP (string file_name);
int make_Ter_Posts();
void make_ptr_to_terms_list(int num_terms);





void set_files() {

  
    make_term_list_TEMP(text_file_name);
     
    text_ptr=set_disk_ptr<char>(terms_list_TEMP);
    doc_ID_ptr=set_disk_ptr<char>(posting_list_TEMP);
    
    quick_sort_file();
 
    int num_terms=make_Ter_Posts();

    terms_ptr=set_disk_ptr<char>(terms_list);
    make_ptr_to_terms_list(num_terms);


    remove(terms_list_TEMP.c_str());
    remove(posting_list_TEMP.c_str());
    
    
} 



void make_term_list_TEMP (string file_name){
    std::ifstream g{file_name};
    std::ofstream save_term_toDisk{terms_list_TEMP};
    std::ofstream save_posting_toDisk{posting_list_TEMP};


    string line;
    string term = "";
    long int i;
    int doc_ID=1;

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
    save_term_toDisk.close();
    save_posting_toDisk.close();

} 

int make_Ter_Posts() {

    std::ofstream save_term_toDisk(terms_list);
    std::ofstream save_posting_toDisk(posting_list, std::ios::binary);
    std::ofstream save_tp_ptr_toDisk(term_to_postings, std::ios::binary);


    string current="";
    string prev="";
    int doc_ID;
    int terms_counter=1;
    


    for(int i=0; i< ter_index_ptr.size(); i++) {
        current=get_term_from_disk(ter_index_ptr[i],text_ptr);
        doc_ID=std::atoi (get_doc_ID_from_disk(pos_index_ptr[i]).c_str());

        if(current!=prev){
            terms_counter++;
            save_term_toDisk<<current<<" ";
            save_posting_toDisk.write((const char*)&doc_ID,sizeof(int));
            save_tp_ptr_toDisk.write((const char*)&i,sizeof(int));
        }

        else {save_posting_toDisk.write((const char*)&doc_ID,sizeof(int));}

        prev=current;

    }


    //last element and file closure

    save_term_toDisk<<"#";
    save_term_toDisk.close();
    save_posting_toDisk.close();
    save_tp_ptr_toDisk.close();

    ter_index_ptr.clear();
    pos_index_ptr.clear();

    return terms_counter;
}


void make_ptr_to_terms_list(int num_terms){


    int size_map=sqrt(num_terms);
    int terms_counter=0;
    int i=0;

    string term="";

    std::ofstream t_p(terms_map,std::ios::binary);

    while(terms_counter <= num_terms-size_map){
        term=get_term_from_disk(i,terms_ptr);

        if (terms_counter % size_map==0)  {
            t_p.write((const char*)&i,sizeof(int));
        }
        terms_counter++;
        i+=term.length()+1;
    }
    t_p.close();

}