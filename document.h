#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <string>
#include <iostream>


size_t getFilesize(const string filename) {
    struct stat st;
    stat(filename.c_str(), &st);
    return st.st_size;
}

void get_doc(long int doc_ID,string  filename){

    size_t filesize = getFilesize(filename.c_str());
    //Open file
    int fd = open(filename.c_str(), O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap
    void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(mmappedData != MAP_FAILED);


    char *chunk = reinterpret_cast<char*>(mmap(NULL, filesize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));


    cout<<chunk[2]<<endl;
    void * adress=reinterpret_cast<void *> (&chunk[3]);

    close(fd);
}



