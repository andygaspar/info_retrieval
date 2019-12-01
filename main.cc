#include "IR_comp.h"
#include <ctime>
#include<cstdio>


void bin(unsigned n) ;


int main(){

    clock_t t,t0,t1,t2,t3,t4;

    cout<<"Naive"<<endl;

    t=clock();
    IR_naive N;
    t=clock()-t;
    //cout<<N.search_word("2");
    //cout<<N.search("l","a","NOT");
    t0=clock();
    N.search_word("voce");
    t0=clock()-t0;


    cout<<endl<<"Front"<<endl;
    t1=clock();
    IR_front M;
    t1=clock()-t1;
    //cout<<M.search_word("2");
    //cout<<M.search("l","a","NOT"); 
    t2=clock(); 
    M.search_word("voce"); 
    t2=clock()-t2;
    //cout<<M.search_word("casa");
    //cout<<M.search_word("zapoteca");
    //cout<<M.search_word("uccellina");


    cout<<endl<<"Comp"<<endl;
    t3=clock();
    IR_comp C;
    t3=clock()-t3;
    //cout<<C.search_word("2");
    //cout<<C.search("l","a","NOT"); 
    t4=clock();
    C.search_word("voce"); 
    t4=clock()-t4;
    //cout<<C.search_word("casa");
    //cout<<C.search_word("zapoteca");
    //cout<<C.search_word("uccellina");  

    



    printf( "%-10f %-10f %-10f %-10f %-10f %-10f \n" ,
         
        (float)t/CLOCKS_PER_SEC,
        (float)t1/CLOCKS_PER_SEC,
        (float)t3/CLOCKS_PER_SEC,
        (float)t0/CLOCKS_PER_SEC,
        (float)t2/CLOCKS_PER_SEC,
        (float)t4/CLOCKS_PER_SEC);
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