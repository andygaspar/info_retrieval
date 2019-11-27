#include "IR_naive.h"


struct IR_front:IR{
        IR_front(): 
            IR{"terms_list.csv","posting_list.csv","term_to_postings.csv","terms_map.csv"} {}
        ~IR_front() {}


        vector<string> get_k_terms(char* &terms_ptr,int k){
            vector<string> block;
            string term="";
            int i=0;
            while(i<k and *terms_ptr!='#'){
                term=get_term_from_disk(0,terms_ptr);
                block.push_back(term);
                i++;
                terms_ptr=&terms_ptr[term.length()+1];
            }
            return block;
        }

        void compress_and_write(vector<string> block) {
            //write index of beginning of the block
            //wrrit compressed block in memory
        }


        void compression(int k=4) override {
            string file="dict_compressed.csv";
            std::ofstream g(file, std::ios::binary);
            char* terms_ptr=set_disk_ptr<char>(terms_file);
            vector<string> block;

            while (*terms_ptr!='#'){
                block=get_k_terms(terms_ptr,k);
                compress_and_write(block);
                cout<<block;
            }


        }


};

int main(){
    set_files();

    

    IR_naive N;

    cout<<N.search_word("l");
    cout<<N.search("l","a","NOT");


    string file="pippo.csv";
    std::ofstream g(file, std::ios::binary);
    u_char i=5;
    /* 
   // g.write((const char*)&i,sizeof(u_char));
    string term="pippo";
   // g<<term;
    i=7;
    term="plutone";
    //g.write((const char*)&i,sizeof(u_char));
    //g<<term;
    
    

    u_char* p=set_disk_ptr<u_char>(file);
    cout<<static_cast<unsigned>(*p)<<endl;
    u_char end=*p;
    p++;
    char* pt= reinterpret_cast<char*> (p);
    for(int i=0;i< end; i++) 
        {cout<<pt[i];}
    cout<<endl;
    p=reinterpret_cast<u_char*>(&pt[end]);

    p++;
    end=*p;
    pt= reinterpret_cast<char*> (p);
    for(int i=0;i< end; i++) 
        {cout<<pt[i];}
    cout<<endl;



    string s="utilizzano102re102to";
    cout<<sizeof(u_char)<<"  "<<sizeof(char*)<<endl;

    u_char a=15;
    
    u_char b=2;

    u_char c=(a<<4)+b;

    u_char gg=c>>4;
    cout<<static_cast<unsigned>(gg)<<endl;

    c=c-(gg<<4);
    cout<<static_cast<unsigned>(c)<<endl;



    vector<u_char> v=from_byte(to_byte(a,b));

    cout<<static_cast<unsigned>(v[0])<<" "<<static_cast<unsigned>(v[1])<<endl;

    */
    string dict="usanze utilizzano utilizzare utilizzato ";

    u_char term_counter=1;
    string prev="utilizzano ";
    string next="utilizzare ";
    string suffix="";
    u_char prefix=0;
    u_char len=prev.length()-1;
    g.write((const char*)&len,sizeof(u_char));
    g<<"utilizzano";

    while(prev[prefix]==next[prefix]) prefix++;
    len=prefix;

    while(next[len]!=' ') {
        suffix+=next[len];
        len++;
    }
    cout<<suffix<<endl;

    g.write((const char*)&len,sizeof(u_char));
    g.write((const char*)&prefix,sizeof(u_char));
    g<<suffix;

g.close();
    u_char* p=set_disk_ptr<u_char>(file);
    len=*p;
    p++;
    char* ptr=reinterpret_cast<char*>(p);
    string term="";
    i=0;
    while(i<len) {
        term+=*ptr;
        i++;
        ptr++;
    }
    cout<<term<<endl;


    IR_front M;
    //M.compression();

    
}


/*    u_char to_byte(u_char len, u_char shared) {
        len=len<<4;
        return len+shared;
    }

    vector<u_char> from_byte(u_char byte){
        vector<u_char> decompressed;
        decompressed.push_back(byte>>4);
        decompressed.push_back(byte-((byte>>4)<<4));
        return decompressed;
    } */
