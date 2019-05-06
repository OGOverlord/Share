#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myfilesystem.h"

#include <pthread.h>

struct HelpStruct{
   // these files are fixed in size. they will not run out of space
   FILE *file_data;
   FILE *directory_table;
   FILE *hash_data;
   int dir_size;
   int threads_max;
   int threads;
}Help;

// this is the first function run. initialise all data structures needed.
// Return a pointer to a memory area (void *) that you can use to store data for
// the virtual file system. The pointer will be void * helper.
// f1 is the filename of the file_data file.
// f2 is the filename of the directory table file.
// f3 is filename of the hash_data file.
// Make sure to create these files if they don't exist.

// Set up threads. I guess I can only go up to n_processors.

void * init_fs(char * f1, char * f2, char * f3, int n_processors) {
   //set up the files. If they don't exist, it will be created


   Help * helper = (Help*) malloc(sizeof(Help));
   helper->file_data = fopen(f1,"rb+");
   helper->directory_table = fopen(f2,"rb+");
   helper->hash_data = fopen(f3, "rb+");
   FILE* file1 = helper->file_data;
   FILE* file2 = helper->directory_table;
   FILE* file3 = helper->hash_data;
   fseek(file2,0,SEEK_END);
   int len = ftell(file2);
   helper->dir_size = len;
   fseek(file2,0,SEEK_SET);
   helper->thread_max = n_processors;
   helper->threads = 0;
   return helper;
}

void close_fs(void * helper) {
   fclose(helper->file_data);
   fclose(helper->directory_table);
   fclose(helper->hash_data);
   free(helper);
}


void truncate(char* filename){
   char truncated[64];
   truncated[63]='\0';
   for(int i = 0; i<63; i++){
      if(strlen(filename)>i){
         truncated[i]=filename[i];
      }else{
         truncated[i]='\0';
      }
   }
   return truncated;
}

int create_file(char * filename, size_t length, void * helper) {

   //int l = strlen(filename);
   char * buf[helper->dir_size];
   fread(buf,sizeof(char),helper->dir_size,helper->directory_table);

   //check if the filename already exists
   for(int i = 0; i< helper->dir_size;i+=72){
      char* truncated = truncate(filename);
      if(strncmp(buf[i],truncated,64)==0){
         return 1;
      }
   }
   //check an empty space
   int success = -1;
   for(int i = 0; i< helper->dir_size; i+=72){
      if(i==0){
         fseek(helper->directory_table,0,SEEK_SET);
      }else{
         fseek(helper->directory_table,72,SEEK_CUR);
      }
      char *test;
      test = fgets(test, 1,helper->directory_table);
      if(*test=='\0'){
         success = i;
      }
   }
   fseek(helper->directory_table,0,SEEK_SET);
   if(success !=-1){
      // just do it
      for(int i = 0; i< 72; i++){
         buf[72*success+i] = truncated[i];
      }
   }else{
      // repack then do it.
      if(){
         // if it fails, then return 1;

      }
   }

   return 0;

}

int resize_file(char * filename, size_t length, void * helper) {
   return 0;
};

void repack(void * helper) {
   return;
}

int delete_file(char * filename, void * helper) {
   return 0;
}

int rename_file(char * oldname, char * newname, void * helper) {
   return 0;
}

int read_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
   return 0;
}

int write_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
   return 0;
}

ssize_t file_size(char * filename, void * helper) {
   return 0;
}

void fletcher(uint8_t * buf, size_t length, uint8_t * output) {
   return;
}

void compute_hash_tree(void * helper) {
   return;
}

void compute_hash_block(size_t block_offset, void * helper) {
   return;
}
