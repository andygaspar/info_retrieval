#include "IR_comp.h"

void bin(unsigned n) 
{ 
    unsigned i; 
    for (i = 1 << 31; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0"); 
    cout<<endl;
} 



int main(){

    

    IR_naive N;
    cout<<"***"<<endl<<N.map.terms_map;
    cout<<N.search_word("2");
    cout<<N.search("l","a","NOT");
    cout<<N.search_word("voce");

    IR_front M;

    cout<<"***"<<endl<<M.map.terms_map;
    cout<<M.search_word("2");
    cout<<M.search("l","a","NOT");  
    cout<<M.search_word("voce"); 


    string file="pippo.csv";
    std::ofstream g(file, std::ios::binary);

    int num=300;
    bin(num);
    u_char second=(num>>8);
    bin(second);
    u_char first=num;
    bin(first);
    int finale= second;
    bin(finale);
    finale=finale<<8;
    bin(finale);
    finale=finale+first;
    bin(finale);
    cout<<finale<<endl;
    //IR_comp C;




    
}

/* 
    string file="pippo.csv";
    std::ofstream g(file, std::ios::binary);
    u_char i=5;
    
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
*/




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


