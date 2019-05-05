#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myfilesystem.h"

#include <pthread.h>

int threads;
char *file1;
char *file2;
char *file3;
// these files are fixed in size. they will not run out of space
FILE *file_data;
FILE *directory_table;
FILE *hash_data;

// this is the first function run. initialise all data structures needed.
// Return a pointer to a memory area (void *) that you can use to store data for
// the virtual file system. The pointer will be void * helper.
// f1 is the filename of the file_data file.
// f2 is the filename of the directory table file.
// f3 is filename of the hash_data file.
// Make sure to create these files if they don't exist.

// Set up threads. I guess I can only go up to n_processors.

void * init_fs(char * f1, char * f2, char * f3, int n_processors) {
   void* space = (void*) malloc(100);
   file1 = f1;
   file2 = f2;
   file3 = f3;
   //set up the files. If they don't exist, it will be created
   file_data = fopen(f1,"rb+");
   directory_table = fopen(f2,"rb+");
   hash_data = fopen(f3, "rb+");
   // create threads
   threads = n_processors;
   return space;
}

void close_fs(void * helper) {
   fclose(file_data);
   fclose(directory_table);
   fclose(hash_data);
   free(helper);
}

int create_file(char * filename, size_t length, void * helper) {
   fseek(directory_table,0,SEEK_END);
   int len = ftell(directory_table);
   fseek(directory_table,0,SEEK_SET);
   char buf[len];
   fread(buf,sizeof(buf),1,directory_table); // buf should now contain every bye of the directory_table file

   int l = strlen(filename);
   int progress = 0;
   // put this in one thread
   // Checking if filename exists
   for(int i = 0; i < sizeof(buf);i++){
      if(progress<l){
         if(buf[i]==filename[progress]){
            progress++;
         }else{
            progress=0;
         }
      }
      //at the end of this, if the whole thing is reached, progress should be l
      if(progress==l){
         return 1;
      }
   }
   // putting this in the current thread
   // Checking for empty spaces
   for(int i = 0; i < sizeof(buf); i++){
      if(progress<l){
         if(buf[i]==NULL){
            s
         }
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
