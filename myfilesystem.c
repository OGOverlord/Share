#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myfilesystem.h"

#include <pthread.h>
#define true 1
#define false 0

typedef struct HelpStruct{
   // these files are fixed in size. they will not run out of space
   FILE *file_data;
   FILE *directory_table;
   FILE *hash_data;
   int dir_size;
   int file_size;
   int threads_max;
   int threads;
}Help;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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


   struct HelpStruct * helper = (struct HelpStruct*) malloc(sizeof(struct HelpStruct));
   helper->file_data = fopen(f1,"rb+");
   helper->directory_table = fopen(f2,"rb+");
   helper->hash_data = fopen(f3, "rb+");
   FILE* file1 = helper->file_data;
   FILE* file2 = helper->directory_table;
   //FILE* file3 = helper->hash_data;
   fseek(file2,0,SEEK_END);
   int len = ftell(file2);
   helper->dir_size = len;
   fseek(file1,0,SEEK_END);
   len = ftell(file1);
   helper->file_size = len;
   fseek(file1,0,SEEK_SET);
   fseek(file2,0,SEEK_SET);
   helper->threads_max = n_processors;
   helper->threads = 0;
   return helper;
}

void close_fs(void * helper) {
   fclose(((Help*)helper)->file_data);
   fclose(((Help*)helper)->directory_table);
   fclose(((Help*)helper)->hash_data);
   free(helper);
}


void truncate(char* filename,char truncated[]){
   for(int i = 0; i<63; i++){
      if(strlen(filename)>i){
         truncated[i]=filename[i];
      }else{
         truncated[i]='\0';
      }
   }
   truncated[63]='\0';
}

int create_file(char * filename, size_t length, void * helper) {

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
