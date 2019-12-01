#include "IR_naive.h"


struct IR_front:IR{

        string terms_comp_file;
        u_char* terms_comp_ptr;
        int map_step_size;

        string term_comp_TEMP;

        int block_size;

        IR_front(int k=4): 
            IR{},block_size{k} {
                terms_comp_file = "storage/front/dict_compressed.csv";
                terms_map_file="storage/front/term_map_comp.csv";
                map_step_size=(int(sqrt(num_terms))-int(sqrt(num_terms))%block_size);
                map_size=num_terms/ map_step_size ;

                vector<string> all_files={terms_comp_file};
                if(files_not_present(all_files)) compression();
                else terms_comp_ptr=set_disk_ptr<u_char>(terms_comp_file);

                map=RAM_map(terms_map_file,terms_comp_ptr);
                map_size=getFilesize(terms_map_file)/sizeof(u_char*)-1;//the last is just a end of file index
            }

        ~IR_front() {}



        // get a block of k terms from uncompressed dictionary made by IR 
        vector<string> get_k_terms(char* &terms_ptr){
            vector<string> block;  
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




        void compress_and_write(vector<string> block,std::ofstream &save_term_to_disk, int &map_index) {

            string suffix="";
            u_char prefix=0;

            //writing first term's length and writing term
            u_char len=block[0].length();
            save_term_to_disk.write((const char*)&len,sizeof(u_char));
            save_term_to_disk<<block[0];

            map_index+=1+block[0].length();

            for(int i=1; i<block.size(); i++) {
                while(block[i-1][prefix]==block[i][prefix]) prefix++;
                len=prefix;
                

                while(len < block[i].length()) {
                    suffix+=block[i][len];
                    len++;
                }

                save_term_to_disk.write((const char*)&len,sizeof(u_char));
                save_term_to_disk.write((const char*)&prefix,sizeof(u_char));
                save_term_to_disk<<suffix;

                map_index+= 2 + suffix.length();
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




        void compression() override {
            std::ofstream save_term_toDisk(terms_comp_file, std::ios::binary);
            std::ofstream seve_map_index_toDisk(terms_map_file,std::ios::binary);
            vector<string> block;

            int map_index=0;
            int terms_counter=0;

            //seve_map_index_toDisk.write((const char*)&map_index,sizeof(int));


            while (*terms_ptr!='#'){

                if(terms_counter % map_step_size  == 0 ) 
                    seve_map_index_toDisk.write((const char*)&map_index,sizeof(int));
  

                //get_k_terms pushes forward terms_ptr
                block=get_k_terms(terms_ptr);  

                //map_index is increased by compress_and_block
                compress_and_write(block,save_term_toDisk,map_index);
                terms_counter+=block.size();
            }

            
            u_char end=0;
            save_term_toDisk.write((const char*)&(end),sizeof(u_char));
            save_term_toDisk.close();
            seve_map_index_toDisk.write((const char*)&map_index,sizeof(int)); //last is already indexing last element
            seve_map_index_toDisk.close();

            terms_comp_ptr=set_disk_ptr<u_char>(terms_comp_file);
   

        }




 
        vector<int> search_word(string term) override{

        
            int range_index=map.search_range(term);
            u_char* ptr       =   &terms_comp_ptr[map.indexes[range_index]];
            u_char* block_end =   &terms_comp_ptr[map.indexes[range_index+1]];
            int i=map_step_size  *  range_index;
            vector<string> block;

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

