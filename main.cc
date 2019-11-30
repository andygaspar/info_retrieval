#include "IR_comp.h"

void bin(unsigned n) 
{ 
    int j=0;
    unsigned i; 
    for (i = 1 << 31; i > 0; i = i / 2) {
        if (j%8==0) cout<<" ";
        (n & i)? printf("1"): printf("0"); 
        j++;
    }
    cout<<endl;
} 

u_char get_kth_byte(int num, int byte){

}



int compress_number(int num,std::ofstream& o){
    if(num<128){
        //simple conversion to u_char. num<128 guarantes already the last bit to be 0, so free to use
        u_char byte_to_store = static_cast<u_char> (num);
        o.write((const char*)&byte_to_store,sizeof(u_char));
    }
    else if (num<16384) {
                
        u_char byte_to_store=(u_char)(num);  //first byte
        byte_to_store|=(1<<7);
        o.write((const char*)&(byte_to_store),sizeof(u_char));
        
        byte_to_store=static_cast<u_char>((num>>7)); //second byte
        o.write((const char*)&(byte_to_store),sizeof(u_char));
    }
    else if(num<4194303){

        u_char byte_to_store=(u_char)(num);  //first byte
        byte_to_store|=(1<<7);
        o.write((const char*)&(byte_to_store),sizeof(u_char));

        byte_to_store=static_cast<u_char>((num>>7)); //second byte
        byte_to_store|=(1 << 7);
        o.write((const char*)&(byte_to_store),sizeof(u_char));

        byte_to_store=static_cast<u_char>((num>>14)); //third byte
        o.write((const char*)&(byte_to_store),sizeof(u_char));


    }
        
}


    int uncompress_number(u_char* &ptr) {

        int num;
        int byte=*ptr;
        ptr++; //sposta il pointer al prossimo numero o byte del numero
        if(!(byte & (1<<7))) return byte;

        num=byte;
        byte=*ptr;
        ptr++; //sposta il pointer al prossimo numero o byte del numero
        if(!(byte & (1<<7))) return num-(1<<7)+(byte<<7);
    
        byte-=(1<<7);
        num-=(1<<7)-(byte<<7);
        byte=*ptr;
        ptr++;  //sposta il pointer al prossimo numero
        return num+(byte<<14);

    }

    //(n & (1 << (k - 1))) 

        //2147483647  int 
    //32767       short int
    //255         u_char





int main(){

    



    string file="pippo.csv";
    std::ofstream g(file, std::ios::binary);

    int N=200000;
        

    for (int i=0;i<N;i++) compress_number(i,g); 
    g.close();
    u_char* ptr=set_disk_ptr<u_char> (file);  

    int unc;

    //IR_comp C;
    for (int i=0;i<N;i++){
        if(i%1000==0) cout<<i<<endl;
        unc=uncompress_number(ptr);
        if(i!=unc) cout<<"errore in "<<i<<"  "<<unc<<endl; 
    }
    remove(file.c_str());

 /*  
    int limite=pow(2,10);
    for(int j=2;j<33;j++) {
        limite=pow(2,j)-1;
        cout<<j<<"   "<<static_cast<unsigned>(limite)<<endl;
    }
 */ 



    //2147483647  int 
    //32767       short int
    //255         u_char
    
    
}

/* 

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


