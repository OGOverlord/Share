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

/*
HUGE ISSUE


The stuff I am reading in is really dodgy.
*/



int create_file(char * filename, size_t length, void * helper) {

   //int l = strlen(filename);


   char buf[((Help*)helper)->dir_size];
   char buf2[((Help*)helper)->file_size];
   fread(buf,sizeof(char),((Help*)helper)->dir_size,((Help*)helper)->directory_table);
   fread(buf2,sizeof(char),((Help*)helper)->file_size,((Help*)helper)->file_data);
   char truncated[64];
   truncate(filename,truncated);

   //check if the filename already exists
   for(int i = 0; i< ((Help*)helper)->dir_size;i+=72){
      if(strncmp(&(buf[i]),truncated,64)==0){
         for(int j = 0; j< 20; j++){
            printf("%c",filename[j]);
         }
         printf("\n");
         printf("original value is %s\n",filename);
         printf("filename is %s\n",truncated);
         printf("found values are ");
         for(int j = 0; j < 64; j++){
            printf("%c",buf[i+j]);
         }
         printf("\n");
         printf("This has been determiened to be already there\n");
         printf("%c\n",buf[0]);
         if((int)buf[0]==99){
            printf("big error\n");
         }
         return 1;
      }
   }
   //check an empty space in directory
   int success = -1;
   for(int i = 0; i< ((Help*)helper)->dir_size; i+=72){
      if(i==0){
         //success = -1;
         fseek(((Help*)helper)->directory_table,0,SEEK_SET);
      }else{
         //success = -1;
         fseek(((Help*)helper)->directory_table,72,SEEK_CUR);
      }
      char test[1];
      //test = fgets(test, 1,((Help*)helper)->directory_table);
      //fgtes causes a fatal error
      fread(test, 1, 1,((Help*)helper)->directory_table );
      if(test[0]=='\0'){
         success = i;
      }
   }
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);
   printf("%d",success);
   //right now, we will ahve found an empty space if sucess is not 01.

   if(success == -1){ // if it failed, then there isn't enough space in the directory. Therefore,
      return 2;
   }
   printf("looking ofr psace\n");
   // now, we will look for a space in the file_data by going through the directory_table
   int exists[((Help*)helper)->file_size];
   size_t offset[4];
   size_t lengthA[4];
   for(int i = 0; i<((Help*)helper)->file_size; i++){
      exists[i]=false;
   }
   for(int i = 0; i< ((Help*)helper)->dir_size; i+=72){
      fseek(((Help*)helper)->directory_table,72*i+64,SEEK_SET);
      fread(offset,1,4,((Help*)helper)->directory_table);
      fseek(((Help*)helper)->directory_table,72*i+68,SEEK_SET);
      fread(lengthA,1,4,((Help*)helper)->directory_table);
      int trueOffset= (int)offset[0]+(int)(offset[1]<<8)+(int)(offset[2]<<16)+(int)(offset[3]<<24);
      int trueLength = (int)lengthA[0]+(int)(lengthA[1]<<8)+(int)(lengthA[2]<<16)+(int)(lengthA[3]<<24);
      while(trueOffset<trueOffset+trueLength){
         exists[trueOffset]=true;
         trueOffset++;
      }
   }
   printf("%u",exists[0]);
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);
   //find an area of exists that has enough room for length
   printf("looking for room");
   int position = -1;
   int counter = 0;
   for(int i = 0; i < sizeof(exists)/sizeof(exists[0]);i++){
      if(counter != length){
         if(exists[i]==false){
            if(position ==-1){
               position = i;
            }
            counter++;
         }else{
            position = -1;
         }
      }
   }
   printf("%d position is", position );
   // Assume that there is enough space for this test case.
   // position will now have the needed value



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
