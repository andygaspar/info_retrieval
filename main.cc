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






    

    //(n & (1 << (k - 1))) 

        //2147483647  int 
    //32767       short int
    //255         u_char





int main(){


    cout<<"Naive"<<endl;
    IR_naive N;
    //cout<<"***"<<endl<<N.map.terms_map;
    cout<<N.search_word("2");
    cout<<N.search("l","a","NOT");
    cout<<N.search_word("voce");

    IR_front M;


    cout<<endl<<"Front"<<endl;
    //cout<<"***"<<endl<<M.map.terms_map;
    cout<<M.search_word("2");
    cout<<M.search("l","a","NOT");  
    cout<<M.search_word("voce"); 



    cout<<endl<<"Comp"<<endl;
    IR_comp C;
    cout<<C.search_word("2");
    cout<<C.search("l","a","NOT");  
    cout<<C.search_word("voce"); 
  

    
}
