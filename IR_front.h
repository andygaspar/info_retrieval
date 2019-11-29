#include "IR_naive.h"


struct IR_front:IR{

        string terms_file_compressed;
        u_char* terms_comp_ptr;
        int map_step_size;

        string term_comp_TEMP;

        int block_size;

        IR_front(int k=4): 
            IR{},block_size{k} {
                terms_file_compressed = "storage/dict_compressed.csv";
                terms_map_file="storage/term_map_comp.csv";
                terms_comp_ptr = nullptr;
                compression();

                
                map=RAM_map(terms_map_file,terms_comp_ptr);
                map_size=getFilesize(terms_map_file)/sizeof(u_char*)-1;
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
            return block;
        }



                
            
        

        void set_indeces(u_char* ptr){
            int terms_counter=0;
            int block_terms_counter=0;
            vector<string> block;
            int i=0;
            std::ofstream save_map(terms_map_file,std::ios::binary);
            map_step_size=(int(sqrt(num_terms))-int(sqrt(num_terms))%block_size);
            map_size=num_terms/ map_step_size ;

            while (i < num_terms) {
                if(i%map_step_size==0)  save_map.write((const char*)&ptr,sizeof(u_char*));
                block=read_block(ptr,i);

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

        
            int range_index=map.search_range(term);
            u_char* ptr=map.terms_comp_adrss[range_index];
            u_char* block_end=map.terms_comp_adrss[range_index+1];
            int i=map_step_size  *  range_index;
            vector<string> block;
            vector<int> v;

            while (i < num_terms   and  ptr!=block_end) {
                block=read_block(ptr,i);
                for(string t: block){
                    
                    if(t==term) return load_postings(i);
                    i++;
                    
                }
                
            }
            perror("term__not found");
            exit(1);
            
    }


};

