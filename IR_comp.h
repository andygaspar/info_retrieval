#include "IR_front.h"

struct IR_comp:IR_front{


    string postings_list_comp_file;
    string ter_to_pos_comp_file;

    u_char* postings_list_comp;


    IR_comp(): IR_front{}{
        postings_list_comp_file="storage/compressed/postings_list_comp.csv";
        ter_to_pos_comp_file="storage/compressed/ter_to_pos_comp.csv";

        vector<string> all_files={postings_list_comp_file,ter_to_pos_comp_file};

        if(files_not_present(all_files)) posting_compression();
        
        ter_to_pos_ptr=set_disk_ptr<int>(ter_to_pos_comp_file);
        postings_list_comp=set_disk_ptr<u_char>(postings_list_comp_file);
    }
    

    ~IR_comp() {}



    int compress_number(int num,std::ofstream& save_to_disk,int &tp_index);
    int uncompress_number(u_char* &ptr);

    vector<int> load_old_postings(int index) {


        vector<int> postings;

        int i=ter_to_pos_ptr[index];
        int end=ter_to_pos_ptr[index+1];

        while(&disk_pos_ptr[i]!=&disk_pos_ptr[end]){
            postings.push_back(disk_pos_ptr[i]);
            i++;
        }
        return postings;

    }

    void posting_compression(){
        vector<int> v;
        std::ofstream save_pos_toDisk(postings_list_comp_file, std::ios::binary);
        std::ofstream save_tp_toDisk(ter_to_pos_comp_file, std::ios::binary);

        int comp_num;
        int tp_index=0;

        for(int i=0; i<num_terms;i++) {
            v=load_old_postings(i);
            
            save_tp_toDisk.write((const char*)&(tp_index),sizeof(int));
            comp_num=compress_number(v[0],save_pos_toDisk,tp_index);
            
            for (int j=1;j<v.size();j++)  compress_number(v[i]-v[i-1],save_pos_toDisk,tp_index);


        }

        //end of file
        u_char end=0;
        save_pos_toDisk.write((const char*)&(end),sizeof(u_char));
        save_tp_toDisk.write((const char*)&(tp_index),sizeof(int));
        save_tp_toDisk.close();


    }

    vector<int> load_postings(int index) override{
        
        vector<int> postings;
        u_char* ptr=postings_list_comp;

        int i=ter_to_pos_ptr[index];
        int end=ter_to_pos_ptr[index+1];

        //the first doesn't require any transformation
        int num=uncompress_number(ptr);
        postings.push_back(num);
        i++;

        while(&disk_pos_ptr[i]!=&disk_pos_ptr[end]){
            num=uncompress_number(ptr);
            num+=postings[i-1];
            postings.push_back(num);
            i++;
        }
        return postings;
    }




    
};



int IR_comp::compress_number(int num,std::ofstream& save_to_disk, int &tp_index){
    if(num<128){
        //simple conversion to u_char. num<128 guarantes already the last bit to be 0, so free to use
        u_char byte_to_store = static_cast<u_char> (num);
        save_to_disk.write((const char*)&byte_to_store,sizeof(u_char));
        
        tp_index+=1;
    }
    else if (num<16384) {
                
        u_char byte_to_store=(u_char)(num);  //first byte
        byte_to_store|=(1<<7);
        save_to_disk.write((const char*)&(byte_to_store),sizeof(u_char));
        
        byte_to_store=static_cast<u_char>((num>>7)); //second byte
        save_to_disk.write((const char*)&(byte_to_store),sizeof(u_char));
        
        tp_index+=2;
        
    }
    else if(num<4194303){

        u_char byte_to_store=(u_char)(num);  //first byte
        byte_to_store|=(1<<7);
        save_to_disk.write((const char*)&(byte_to_store),sizeof(u_char));

        byte_to_store=static_cast<u_char>((num>>7)); //second byte
        byte_to_store|=(1 << 7);
        save_to_disk.write((const char*)&(byte_to_store),sizeof(u_char));

        byte_to_store=static_cast<u_char>((num>>14)); //third byte
        save_to_disk.write((const char*)&(byte_to_store),sizeof(u_char));

        tp_index+=3;


    }
        
}


int IR_comp::uncompress_number(u_char* &ptr) {
    

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