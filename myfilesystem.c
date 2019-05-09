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

   printf("Is the error here?\n");
   struct HelpStruct * helper = (struct HelpStruct*) malloc(sizeof(struct HelpStruct));
   printf("here?\n");
   printf("%s\n",f1);
   printf("%s\n",f2);
   printf("%s\n",f3);
   helper->file_data = fopen(f1,"rb+");
   helper->directory_table = fopen(f2,"rb+");
   //FILE* file1 = fopen(f1,"rb+");
   //FILE* file2 = fopen(f2,"rb+");
   if (helper->file_data==NULL){
      printf("Unable to open file1!\n");
   }
   if (helper->directory_table==NULL){
      printf("Unable to open file2!\n");
   }
   helper->hash_data = fopen(f3, "rb+");
   printf(",here?\n");
   char abc[1]= {'1'};
   printf("testing fread\n");
   fread(abc,1,1,((Help*)helper)->directory_table);
   printf("did fread cause the issue? NO?\n");
   printf("the value I got from fread is %x\n",abc[0]);
   fseek(helper->directory_table,0,SEEK_END);
   printf("after fseek?\n");
   int len = ftell(helper->directory_table);
   printf("after ftell?\n");
   helper->dir_size = len;
   fseek(helper->file_data,0,SEEK_END);
   len = ftell(helper->file_data);
   helper->file_size = len;
   fseek(helper->directory_table,0,SEEK_SET);
   helper->threads_max = n_processors;
   helper->threads = 0;
   printf("or here?\n");
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
   printf("NEW RUN\n");
   FILE* dir = ((Help*)helper)->directory_table;
   unsigned char buf[((Help*)helper)->dir_size];
   unsigned char buf2[((Help*)helper)->file_size];
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);
   fseek(((Help*)helper)->file_data,0,SEEK_SET);
   fread(buf,sizeof(char),((Help*)helper)->dir_size,((Help*)helper)->directory_table);
   fread(buf2,sizeof(char),((Help*)helper)->file_size,((Help*)helper)->file_data);
   char truncated[64];
   truncate(filename,truncated);
   //create a char buf of that one.
   //check if the filename already exists
   char compared[((Help*)helper)->dir_size];
   for(int i = 0; i< ((Help*)helper)->dir_size; i++){
      compared[i]=buf[i];
   }
   for(int i = 0; i< ((Help*)helper)->dir_size;i+=72){
      if(strcmp(&(compared[i]),truncated)==0){
         printf("EXACTLY ThE SMAE\n");
         printf("%s",filename);
         printf("\n");
         printf("original value is %s\n",filename);
         printf("filename is %s\n",truncated);
         printf("found values are ");
         printf("%s",truncated);
         printf("\n");
         printf("This has been determiened to be already there\n");
         printf("%c\n",buf[0]);
         if((int)buf[0]==99){
            printf("big error\n");
         }
         fseek(((Help*)helper)->directory_table,0,SEEK_SET);
         unsigned char test[0];
         fread(&test,sizeof(char),1,((Help*)helper)->directory_table);
         printf("%u\n",(unsigned int) test[0]);
         printf("%X\n",test[0]);
         printf("ITSOVER\n");
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
      unsigned char test[1];
      //test = fgets(test, 1,((Help*)helper)->directory_table);
      //fgtes causes a fatal error
      fread(test, 1, 1,((Help*)helper)->directory_table );
      if(test[0]=='\0'&&success==-1){
         success = i;
      }
   }
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);
   printf("%d\n",success);
   //right now, we will ahve found an empty space if sucess is not -1.

   if(success == -1){ // if it failed, then there isn't enough space in the directory. Therefore,
      return 2;
   }
   printf("looking ofr space\n");
   printf("%x \n",buf[64]);
   printf("%x \n",buf[65]);
   printf("%x \n",buf[66]);
   printf("%x \n",buf[67]);
   printf("%u \n",(int)buf[64]+((int)buf[65]<<8)+((int)buf[66]<<16)+((int)buf[67]<<24));


   int exists[((Help*)helper)->file_size];
   //unsigned char offsetA[4];
   //unsigned char lengthA[4];
   printf("first for loop to fill exists with false about to start \n");
   for(int i = 0; i<((Help*)helper)->file_size; i++){
      exists[i]=false;
   }

   printf("fin\n");
   printf("about to use loop to set the values of exists as true or false\n");
   for(int i = 0; i< (((Help*)helper)->dir_size); i+=72){
      //check if this code is an actual file or an empty/deleted space.
      printf("i is %d\n",i);
      fseek(((Help*)helper)->directory_table,i,SEEK_SET);//start of each block
      unsigned char value[1];
      fread(&value[0],1,1,((Help*)helper)->directory_table);
      printf("value of value[0] is %X \n",value[0]);
      if(value[0]!='\0'){
         // get offset bytes into offsetA
         /*fseek(((Help*)helper)->directory_table,72*i+64,SEEK_SET);
         fread(&offsetA[0],1,1,((Help*)helper)->directory_table);
         fseek(((Help*)helper)->directory_table,72*i+65,SEEK_SET);
         fread(&offsetA[1],1,1,((Help*)helper)->directory_table);
         fseek(((Help*)helper)->directory_table,72*i+66,SEEK_SET);
         fread(&offsetA[2],1,1,((Help*)helper)->directory_table);
         fseek(((Help*)helper)->directory_table,72*i+67,SEEK_SET);
         fread(&offsetA[3],1,1,((Help*)helper)->directory_table);
         */
         fseek(((Help*)helper)->directory_table,i+64,SEEK_SET);
         int trueOffset = -1;
         fread(&trueOffset,4,1,((Help*)helper)->directory_table);
         printf("%d\n",trueOffset);
         // printf("1st is %X\n",offsetA[0]);
         // printf("2nd is %X\n",offsetA[1]);
         // printf("3rd is %X\n",offsetA[2]);
         // printf("4th is %X\n",offsetA[3]);
         int trueLength = -1;
         fseek(((Help*)helper)->directory_table,i+68,SEEK_SET);
         fread(&trueLength,4,1,((Help*)helper)->directory_table);
         printf("%d\n",trueLength);
         //fread(&lengthA,1,4,((Help*)helper)->directory_table);
         // printf("1st, 2nd, 3rd, and 4th lengthA are %X,%X,%X,%X \n",lengthA[0],lengthA[1],lengthA[2],lengthA[3]);
         // printf("about to use \n");
         // int trueOffset= (int)offsetA[0]+((int)offsetA[1]<<8)+((int)offsetA[2]<<16)+((int)offsetA[3]<<24);
         // int trueLength = (int)lengthA[0]+((int)lengthA[1]<<8)+((int)lengthA[2]<<16)+((int)lengthA[3]<<24);
         printf("Trueoffset and truelength acheived\n");
         int counter = trueOffset;
         printf("error?\n");

         if((trueOffset+trueLength)>((Help*)helper)->dir_size){
            printf("BIG ERROR\n");
         }
         printf("%d\n",trueOffset);
         printf("%d\n",trueLength);
         printf("%d\n", ((Help*)helper)->dir_size);
         //fseek(((Help*)helper)->directory_table,64,SEEK_SET);
         //int o = -1;
         //fread(&o,4,1,((Help*)helper)->directory_table);
         //printf("%d\n",o);
         while(counter<trueOffset+trueLength){
            exists[counter]=true;
            counter++;
         }
         printf("trueOffset success\n");
      }

   }
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);

   printf("SO far so good\nThe value of exists[0] is now %u\n",exists[0]);


   //NOW, it can be assumed that we found a spot in the directory file.
   // Now, it is time to find an area inside of it that has enough space for lengthA
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
            counter = 0;
         }
      }
   }
   //now, if counter == length, the value of position will be
   printf("position is %d, success is %d\n",position, success);
   if(counter!=length){
      // repack
      printf("test\n");
      // recalculate exists

      // recalculate position and counter.

   }

   if(counter == length){
      if(position == -1){
         printf("\n\nFor some reason, position is -1 while counter is length\n");
      }
      // place this inside of the directory_table
      fseek(dir,success,SEEK_SET);
      fwrite(&truncated,64,1,dir);
      fseek(((Help*)helper)->directory_table,success+64,SEEK_SET);
      fwrite(&position,4,1,dir);
      fseek(((Help*)helper)->directory_table,success+68,SEEK_SET);
      fwrite(&length,4,1,dir);
      fseek(((Help*)helper)->directory_table,0,SEEK_SET);

      // zero out the stuff inside of the file
      fseek(((Help*)helper)->file_data,position,SEEK_SET);
      unsigned char zero[length];
      for(int i = 0; i< length; i++){
         zero[i]='\0';
      }
      fwrite(&zero,length,1,((Help*)helper)->file_data);
      fseek(((Help*)helper)->file_data,0,SEEK_SET);
   }else{
      return 2;
   }
   fseek(((Help*)helper)->directory_table,0,SEEK_SET);
   unsigned char test[0];
   fread(&test,sizeof(char),1,((Help*)helper)->directory_table);
   printf("%u\n",(unsigned int) test[0]);
   printf("%X\n",test[0]);
   printf("test 2 should end now\n");
   return 0;
}

int resize_file(char * filename, size_t length, void * helper) {
   return 0;
};



////
/*
while i from 0 to sizeof dir. i+=72

print






*/
////


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
