#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char* argv[]) {
    
    std::string file_name= "../doc/wiki_it.documents";
    std::string doc_file_small="../doc/doc_test_small.txt";


    remove(doc_file_small.c_str());
    remove("storage/front/dict_compressed.csv");
    remove("storage/info/info.csv");
    remove("storage/naive/posting_list.csv");
    remove("storage/front/term_map_comp.csv");
    remove("storage/naive/term_to_postings.csv");
    remove("storage/naive/terms_list.csv");
    remove("storage/naive/terms_map.csv");
    
    
    std::ifstream g{file_name};
    std::ofstream o{doc_file_small};
    int num_doc;
    std::string line = "";
    if(argc>1) num_doc=(unsigned int)(atoi(argv[1]));
    else num_doc=3;
    
    int j=0;

    

    while(getline(g,line) and j<num_doc) {
        // write to outfile
        o<<line<<std::endl;
        j++;


    }
    g.close();
    o.close();

}
