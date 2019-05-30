/* Do not change! */
#define FUSE_USE_VERSION 29
#define _FILE_OFFSET_BITS 64
/******************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fuse.h>
#include <errno.h>

#include "myfilesystem.h"
#include "myfilesystem.c"

char * file_data_file_name = NULL;
char * directory_table_file_name = NULL;
char * hash_data_file_name = NULL;


/*
Pass in the name of a file
from that, place in the metadata into stat structures
st_mode, st_nlink, st_size

if it doesn't exist, return -ENOENT, defined in errno.h

*/

int file_exists(char* filename, void* helper){
   FILE * dir = ((Help*)helper)->directory_table;
   FILE * files = ((Help*)helper)->file_data;
   int filesize = ((Help*)helper)->file_size;
   int size;
   fseek(dir, 0, SEEK_END);
   size = ftell(dir);
   char truncated[64];
   truncateIt(filename,truncated);
   for(int i = 0; i< size; i++){
      char name[64];
      fseek(dir,72*i,SEEK_SET);
      fread(&name,64,1,dir);
      if(bothCharSame(name, truncated)==true){
         return 0;
      }
   }
   return -1;
}


// fuse_get_context()->private_data; // this is the helper as a void*, (the thing returned from init)
int myfuse_getattr(const char * name, struct stat * result) {
    // MODIFY THIS FUNCTION
    Help* helper =  (Help*)(fuse_get_context()->private_data); // this is the helper as a void*, (the thing returned from init)

    memset(result, 0, sizeof(struct stat));
    if (strcmp(name, "/") == 0) {
        result->st_mode = S_IFDIR;

    } else {
        result->st_mode = S_IFREG;
        char* buf[1];
        if( file_exists(name+1,helper)==0 ){
           return 0;
        }else{
           return -ENOENT;
        }
    }
    return 0;
}

int myfuse_readdir(const char * name, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi) {
    // MODIFY THIS FUNCTION
    Help* helper =  (Help*)(fuse_get_context()->private_data);
    if (strcmp(name, "/") == 0) { // this is all name does
        // filler(buf, "test_file", NULL, 0);
        //while
        filler(buf, "test_file", NULL, 0);
    }
    return 0;
}

int myfuse_unlink(const char *filename); // delete
    // FILL OUT
// don't pass in name, pass in name+1
int myfuse_rename(const char *filename, const char * newname);
    // FILL OUT

int myfuse_truncate(const char *filename, off_t offset);
    // FILL OUT

int myfuse_open(const char *filename, struct fuse_file_info * asdf);
    // FILL OUT


    //reutnr number of succesffully read bytes
//if filesize is 20, but offset is 5, and you want to read in 50 bytes, return 15.
int myfuse_read(const char * filename, char *output, size_t length, off_t offset, struct fuse_file_info * asdf );
    // FILL OUT
{
   Help* helper =  (Help*)(fuse_get_context()->private_data);

}
//reutnr number of succesffully written bytes
// if offset is greater than the length of the file, resize first.
int myfuse_write(const char * filename, const char * input, size_t length, off_t offset, struct fuse_file_info * asdf);
    // FILL OUT

int myfuse_release(const char *filename, struct fuse_file_info *asdf);
    // FILL OUT

void * myfuse_init(struct fuse_conn_info * asdf);
    // FILL OUT

void myfuse_destroy(void * helper);
    // FILL OUT

int myfuse_create(const char * filename, mode_t ignore, struct fuse_file_info *asdf);
    // FILL OUT

struct fuse_operations operations = {
    .getattr = myfuse_getattr,
    .readdir = myfuse_readdir,
    // FILL OUT BELOW FUNCTION POINTERS
    .unlink = myfuse_unlink,
    .rename = myfuse_rename,
    .truncate = myfuse_truncate,
    .open = myfuse_open,
    .read = myfuse_read,
    .write = myfuse_write,
    .release = myfuse_release,
    .init = myfuse_init,
    .destroy = myfuse_destroy,
    .create = myfuse_create
};

int main(int argc, char * argv[]) {
    // MODIFY (OPTIONAL)
    if (argc >= 5) {
        if (strcmp(argv[argc-4], "--files") == 0) {
            file_data_file_name = argv[argc-3];
            directory_table_file_name = argv[argc-2];
            hash_data_file_name = argv[argc-1];
            argc -= 4;
        }
    }
    // After this point, you have access to file_data_file_name, directory_table_file_name and hash_data_file_name
    int ret = fuse_main(argc, argv, &operations, NULL);
    return ret;
}



int myfuse_unlink(const char * filename){
   return 0;
}// delete
    // FILL OUT
// don't pass in name, pass in name+1
int myfuse_rename(const char * filename, const char * newname){
   return 0;
}
    // FILL OUT

int myfuse_truncate(const char *filename, off_t offset){ //resize
   return 0;
}
    // FILL OUT

int myfuse_open(const char *filename, struct fuse_file_info * asdf){
   return 0;
}
    // FILL OUT


    //reutnr number of succesffully read bytes
//if filesize is 20, but offset is 5, and you want to read in 50 bytes, return 15.
int myfuse_read(const char * filename, char *output, size_t length, off_t offset, struct fuse_file_info * asdf )    // FILL OUT
{
   Help* helper =  (Help*)(fuse_get_context()->private_data);
   return 0;
}
//reutnr number of succesffully written bytes
// if offset is greater than the length of the file, resize first.
int myfuse_write(const char * filename, const char * input, size_t length, off_t offset, struct fuse_file_info * asdf){
   return 0;
}
    // FILL OUT

int myfuse_release(const char * filename, struct fuse_file_info * asdf){
   return 0;
}
    // FILL OUT

void * myfuse_init(struct fuse_conn_info * asdf){}
    // FILL OUT

void myfuse_destroy(void * helper){}
    // FILL OUT

int myfuse_create(const char * filename, mode_t ignore, struct fuse_file_info * adsf ){
   return 0;
}
