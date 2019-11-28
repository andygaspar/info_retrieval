#include "IR_naive.h"


struct IR_front:IR{

        string terms_file_compressed;
        u_char* terms_comp_ptr;

        int block_size;

        IR_front(int k=4): 
            IR{},block_size{k} {
                terms_file_compressed = "storage/dict_compressed.csv";
                terms_comp_ptr = nullptr;
                compression();
                terms_map_file="terms_map_comp.csv";
                
                map=RAM_map(terms_map_file,terms_comp_ptr);
                map_size=sqrt(getFilesize(terms_map_file)/sizeof(u_char*));
            }
        ~IR_front() {}


        vector<string> get_k_terms(char* &terms_ptr){
            vector<string> block;  //forse meglio block={} e passare &block
            string term="";
            int i=0;
            while( i< block_size  and  *terms_ptr!='#'){
                term=get_term_from_disk(0,terms_ptr);
                block.push_back(term);
                i++;
                terms_ptr=&terms_ptr[term.length()+1];
            }
            return block;
        }

        void compress_and_write(vector<string> block,std::ofstream &save_to_disk) {

            string suffix="";
            u_char prefix=0;
            u_char len=block[0].length();


            save_to_disk.write((const char*)&len,sizeof(u_char));
            save_to_disk<<block[0];

            for(int i=1; i<block.size(); i++) {
                while(block[i-1][prefix]==block[i][prefix]) prefix++;
                len=prefix;
                

                while(len < block[i].length()) {
                    suffix+=block[i][len];
                    len++;
                }

                save_to_disk.write((const char*)&len,sizeof(u_char));
                save_to_disk.write((const char*)&prefix,sizeof(u_char));
                save_to_disk<<suffix;
                suffix="";
                prefix=0;
            }

            
        }

        vector<string> read_block(u_char* &ptr, int i){
            vector<string> block;
            string term="";
            u_char len;
            u_char prefix;

            char* term_ptr;
            int block_len;
            if(num_terms-i < block_size){
                block_len = num_terms % i;
            }
            else{
                block_len=block_size;
            }
     

            for(int t=0;t<block_len; t++){
                
                if (t>0) {
                    ptr=reinterpret_cast<u_char*> (term_ptr); 
                    len=*ptr;
                    ptr++;
                    prefix=*ptr;
                    term=block[t-1].substr(0,prefix);
                }
                else{
                    len=*ptr;
                    prefix=0;
                }

                ptr++;

                term_ptr=reinterpret_cast<char*> (ptr);


                for(int i=0; i<len-prefix;i++){
                    term+=*term_ptr;
                    term_ptr++;
                }
                block.push_back(term);
                term="";
                
            }
            ptr=reinterpret_cast<u_char*> (term_ptr);
            cout<<block;
            return block;
        }



                
            
        

        void set_indeces(u_char* ptr){
            int terms_counter=0;
            int block_terms_counter=0;
            vector<string> block;
            int i=0;
            std::ofstream save_map("terms_map_comp.csv",std::ios::binary);
            map_size=sqrt(num_terms/block_size);

            while (i < num_terms) {
                block=read_block(ptr,i);

                if(i%map_size==0) save_map.write((const char*)&ptr,sizeof(u_char*));
                i+=block_size;
            }
            save_map.write((const char*)&ptr,sizeof(u_char*));
            save_map.close();
        }


        void compression() override {
            string file=terms_file_compressed;
            std::ofstream save_to_disk(file, std::ios::binary);
            vector<string> block;

            while (*terms_ptr!='#'){
                block=get_k_terms(terms_ptr);
                compress_and_write(block,save_to_disk);
                cout<<block;
            }

                        //questa è da controllare
            u_char end=0;
            save_to_disk.write((const char*)&(end),sizeof(u_char));
            save_to_disk.close();

            terms_comp_ptr=set_disk_ptr<u_char>(file);

            set_indeces(terms_comp_ptr);      

            save_to_disk.close();  


        }



        vector<int> search_word(string term) override{

        
            range range_term=map.search_range(term);
            int block_begin=*range_term.ptr;
            range_term.ptr++;
            int block_end=*(range_term.ptr);
            string word="";


            int i=block_begin;
            int j=-1;
            bool found=false;

            while(i<block_end and found==false){
                
                if (terms_ptr[i]==' ') {
                    j++;
                    if (word==term) found=true;
                    word="";
                }      
                else word+=terms_ptr[i];    
                i++;            
            }
            if(i==block_end) {
                perror("term__not found");
                exit(1);
            }
            vector<int> v=load_postings(range_term.index*map_size+j);
            return v;
    }


};

