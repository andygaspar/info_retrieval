#include "IR_front.h"

struct IR_comp:IR_front{

    string postings_list_comp_file;
    string ter_to_pos_comp_file;

    IR_comp(): IR_front{}{

        posting_compression();
    }
    

    ~IR_comp() {}

    void posting_compression(){
        vector<int> v;

        for(int i=0; i<num_terms;i++) {
            v=load_postings(i);
            //cout<<v;
            for (int j=v.size();j>0;j--) v[j]-=v[j-1];
            //cout<<v;
            for (int j=1;j<v.size();j++) v[j]+=v[j-1];
            //cout<<v;
            //cout<<i<<" :  "<<num_terms<<"  "<<v;
        }
    }
    
};