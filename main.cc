#include "IR_comp.h"


void bin(unsigned n) ;


int main(){


    cout<<"Naive"<<endl;
    IR_naive N;
    //cout<<N.search_word("2");
    //cout<<N.search("l","a","NOT");
    cout<<N.search_word("voce");


    cout<<endl<<"Front"<<endl;
    IR_front M;
    //cout<<M.search_word("2");
    //cout<<M.search("l","a","NOT");  
    cout<<M.search_word("voce"); 
    cout<<M.search_word("casa");
    cout<<M.search_word("zapoteca");
cout<<M.search_word("uccellina");
    cout<<endl<<"Comp"<<endl;
    IR_comp C;
    //cout<<C.search_word("2");
    //cout<<C.search("l","a","NOT"); 
    cout<<C.search_word("voce"); 
    cout<<C.search_word("casa");
    cout<<C.search_word("zapoteca");
cout<<C.search_word("uccellina");  

    
}






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