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

typedef struct LinkedList{
    int data;
    int offset;
    int length;
    struct LinkedList *next;
}Node;



//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
      repack(helper);

      // recalculate exists

      for(int i = 0; i<((Help*)helper)->file_size; i++){
         exists[i]=false;
      }
      for(int i = 0; i< (((Help*)helper)->dir_size); i+=72){
         fseek(((Help*)helper)->directory_table,i,SEEK_SET);//start of each block
         unsigned char value[1];
         fread(&value[0],1,1,((Help*)helper)->directory_table);
         printf("value of value[0] is %X \n",value[0]);
         if(value[0]!='\0'){
            fseek(((Help*)helper)->directory_table,i+64,SEEK_SET);
            int trueOffset = -1;
            fread(&trueOffset,4,1,((Help*)helper)->directory_table);
            int trueLength = -1;
            fseek(((Help*)helper)->directory_table,i+68,SEEK_SET);
            fread(&trueLength,4,1,((Help*)helper)->directory_table);
            int counter = trueOffset;
            if((trueOffset+trueLength)>((Help*)helper)->dir_size){
               printf("BIG ERROR\n");
            }
            while(counter<trueOffset+trueLength){
               exists[counter]=true;
               counter++;
            }
         }
      }

      // recalculate position and counter.

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
   }

   if(counter == length){
      if(position == -1){
         printf("\n\nFor some reason, position is -1 while counter is length\n");
         return 2;
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

/*
dir
files
Loop through to the correct filename.
store the position in int position.
if resizing it to be greater :
   loop though dir. If any have a offset that is between the current value's offset and offset+length,
   remove the original file.
   repack();
   create_file();
if it is resizing to be smaller :
   read the file into a unsigned char array. Turn the back into '\0'.
   loop though files
   to the correct position
   fwrite it in.
*/

int bothCharSame(char a[],char b[]){
   int same = true;
   for(int i = 0; i< 64; i++){
      if(a[i]!=b[i]){
         same = false;
      }
   }
   return same;
}


int resize_file(char * filename, size_t length, void * helper) {
   printf("length is %ld, and file size is %d\n",length,((Help*)helper)->file_size);
   if(length>((Help*)helper)->file_size){
      return 2;
   }

   printf("\n\n\nSTART OF RESIZE\n\n\n");
   printf("length is %ld\n",length);
   FILE * dir = ((Help*)helper)->directory_table;
   FILE * files = ((Help*)helper)->file_data;
   int size = ((Help*)helper)->dir_size/72;
   char truncated[64];
   truncate(filename,truncated);
   printf("truncated is : %s\n", truncated);
   int position = -1;
   int valueLength = -1;
   int valueOffset = -1;
   // unsigned char values[length];
   for(int i = 0; i< size; i++){
      printf("length is %ld\n",length);
      int buf[1];
      char name[64];
      fseek(dir,72*i,SEEK_SET);
      fread(&name,64,1,dir);
      printf("name is %s \n", name);
      if(bothCharSame(name, truncated)==true){
         position = i;
         printf("position is %d\n",position);
         fseek(dir,72*i+64,SEEK_SET);
         fread(&buf,4,1,dir);
         valueOffset = buf[0];
         fseek(dir,72*i+68,SEEK_SET);
         fread(&buf,4,1,dir);
         valueLength = buf[0];
         printf("length is %ld\n",length);
      }
   }
   printf("length is %ld\n",length);
   unsigned char values[valueLength];
   printf("Before fseeking files, length is %ld\n",length);
   fseek(files,valueOffset,SEEK_SET);
   fread(&values,valueLength,1,files);
   printf("is position -1 right now? %d\n",position);
   printf("length is %ld\n",length);
   if(position == -1){
      return 1;
   }
   printf("length is %ld\n",length);
   printf("testing postiion fialed somhow\n");
   // now, position will be the number of 72-byte blocks in this went.
   // values, valueOffset, and valueLength will have the correct value.
   int tes[4];
   fseek(dir,72*position+64,SEEK_SET);
   fread(&tes,4,1,dir);
   printf("length is %ld\n",length);
   printf("offset at position is %d\n",tes[0]);
   fseek(dir,72*position+68,SEEK_SET);
   fread(&tes,4,1,dir);
   printf("length is %ld\n",length);
   printf("length at position is %d\n",tes[0]);
   printf("length is %ld, valueLength is %d\n",length,valueLength);


   if(length > valueLength){ // increasing the length
      printf("length is increasing. the new length is %ld, while the old is %d\n",length,valueLength);
      int repackbool = false;
      for(int i = 0; i<size; i++){ // check for VALUE
         int o;
         int l;
         fseek(dir,72*i+64,SEEK_SET);
         fread(&o,4,1,dir);
         fseek(dir,72*i+68,SEEK_SET);
         fread(&l,4,1,dir);
         if(o>=valueOffset&&o<valueOffset+length){ // between the offset and the new length
            repackbool=true;
         }
         printf("o is %d, valueOffset is %d, length is %ld, repackbool is %d\n",o,valueOffset,length,repackbool);
      }

      if(repackbool==true){
         printf("repack is happening\n");
         //delete the current file
         fseek(dir,position*72,SEEK_SET);
         unsigned char n = '\0';
         fwrite(&n,1,1,dir);
         //REPACK
         repack(helper); //position variable is unchanged
         //find the position in file_data
         printf("repack success\n");
         int currentMax = 0;
         for(int j =0; j<size;j++){
            int buf1[1];
            int buf2[1];
            int bufv[1];
            fseek(dir,72*j,SEEK_SET);
            fread(&bufv,1,1,dir);
            fseek(dir,72*j+64,SEEK_SET);
            fread(&buf1,4,1,dir);
            fseek(dir,72*j+68,SEEK_SET);
            fread(&buf2,4,1,dir);
            if(buf1[0]+buf2[0] > currentMax && bufv[0]!='\0'){
               currentMax = buf1[0]+buf2[0];
            }
            printf("in the %d th run, currentMax becomes %d\n",j,currentMax);
         }
         //currentMax will thus be the position that one can add in. // check to make sure it is within bound
         printf("currentmax is %d, length is %ld or %d, filesize is %d\n",currentMax,length,(int)length,((Help*)helper)->file_size);
         if(currentMax+length>((Help*)helper)->file_size){
            return 2;
         }
         // go to the position, place all the values in.
         fseek(dir,72*position,SEEK_SET);
         fwrite(&truncated,64,1,dir);
         fseek(dir,72*position+64,SEEK_SET);
         fwrite(&currentMax,4,1,dir);
         fseek(dir,72*position+68,SEEK_SET);
         fwrite(&length,4,1,dir);
         // place the values into file_data
         fseek(files,currentMax,SEEK_SET);
         unsigned char buf[length];
         for(int i=0;i<length;i++){
            if(i<valueLength){
               buf[i]=values[i];
            }else{
               buf[i]='\0';
            }
         }
         fwrite(&buf,length,1,files);
      }else{ //if repack was not called
         // go to the position, place all the values in.
         fseek(dir,72*position,SEEK_SET);
         fwrite(&truncated,64,1,dir);
         fseek(dir,72*position+64,SEEK_SET);
         fwrite(&valueOffset,4,1,dir);
         fseek(dir,72*position+68,SEEK_SET);
         fwrite(&length,4,1,dir);
         // place the values into file_data
         unsigned char buf[length];
         for(int i=0;i<length;i++){
            if(i<valueLength){
               buf[i]=values[i];
            }else{
               buf[i]='\0';
            }
         }
         fwrite(&buf,length,1,files);
      }
   }else if(length < valueLength){
      printf("length is decreasing. the new length is %ld, while the old is %d\n",length,valueLength);
      // go to the position, place all the values in.
      fseek(dir,72*position,SEEK_SET);
      fwrite(&truncated,64,1,dir);
      fseek(dir,72*position+64,SEEK_SET);
      fwrite(&valueOffset,4,1,dir);
      fseek(dir,72*position+68,SEEK_SET);
      fwrite(&length,4,1,dir);
      // place the values into file_data
      unsigned char buf[length];
      for(int i=0;i<length;i++){
         buf[i]=values[i];
      }
      fseek(files,valueOffset,SEEK_SET);
      fwrite(&buf,length,1,files);

   }
   fseek(files,54,SEEK_SET);
   unsigned char test[1];
   fread(&test,1,1,files);
   printf("The 54th bytes is %X\n",test[0]);
   return 0;
}



////
/*
Read file_data.
loop through directory file to determine the order of the filenames (disregarding those that have been deleted)
store that in ordered1[]. fill up the unnecessary with -1.
create ordered[] made from
set up a counter
set up minpos=0
loop through ordered[] :
   shift the values in file_data described by ordered[1] back by (offset - minpos)
   write minpos into the offset.
   make minpos = minpos+length
end of loop
*/
////


void repack(void * helper) {

   printf("\n\n\nSTART OF REPACK\n");
   FILE* dir = ((Help*)helper)->directory_table;
   FILE* files = ((Help*)helper)->file_data;
   unsigned char test[1];
   fseek(dir,64,SEEK_SET);
   fread(&test,1,1,dir);
   printf("offset 64 has %u (decimal)\n",test[0]);
   fseek(dir,68,SEEK_SET);
   fread(&test,1,1,dir);
   printf("offset 68 has %u (decimal)\n",test[0]);
   fseek(dir,0,SEEK_SET);
   fseek(files,0,SEEK_SET);
   int size = (((Help*)helper)->dir_size)/72;
   if(size==0){
      printf("An empty file was given in the directory table.\n");
   }
   // REMEMBER TO FREE NODES
   Node *head = (Node*)malloc(sizeof(Node));
   Node *curr = head;
   head->data = -1;
   head->offset = -1;
   head->length = -1;
   head->next = NULL;
   for(int i = 0; i< size; i++){
      unsigned char doesFileExist[1];
      fseek(dir, 72*i,SEEK_SET);
      fread(&doesFileExist,1,1,dir);
      if(doesFileExist[0]!='\0'){
         Node * new = (Node*)malloc(sizeof(Node));
         // fseek(dir,72*i,SEEK_SET);
         // fread(&(new->data),4,1,dir);
         // the above two lines make no sense
         new->data = 72*i;
         fseek(dir,72*i+64,SEEK_SET);
         fread(&(new->offset),4,1,dir);
         fseek(dir,72*i+68,SEEK_SET);
         fread(&(new->length),4,1,dir);
         curr = head;
         int j = 0; // j will be the position of curr, with 0 as the head.
         while(curr!=NULL&&(new->offset > curr-> offset)){
            curr = curr->next;
            j++;
         }
         // right now, j will be the value that new will replace, shifting the original j to new->next
         //0 : head, 1: head->next ,...... j-1: something, j: new, j+1 : original j
         Node*curr = head;
         for(int k = 1; k<j; k++){ // won't run if j is 1. Thus, curr is still head in that case.
            curr = curr->next;
         }
         new->next = curr->next;
         curr->next = new;
      }

   }
   // Now, I have an ordered list of nodes.
   curr = head->next; // cos head isn't actuall one of the files
   int minpos = 0;
   //int end = 0;
   while(curr!=NULL){
      //fseek(dir,68,SEEK_SET);
      //fread(&test,1,1,dir);
      //printf("offset 68 has %u (decimal)\n",test[0]);
      //printf("offset is %d. Minimum position thus far is %d \n", curr->offset, minpos);
      if(minpos > curr->offset && curr!=head){
         printf("for some reason, the values of the offset and length was completely wrong\n");
         // it seems that this happens when curr->offset is -1. This only occurs when it is talking about head
      }else if(minpos < curr->offset){ // i.e. no need to do anything when the minimum and the offset are the same
         printf("The minpos is lower than offset; as expected\n");
         unsigned char values[curr->length];
         fseek(files,(curr->offset),SEEK_SET);
         fread(&values,curr->length,1,files);
         fseek(files,minpos,SEEK_SET);
         fwrite(&values,curr->length,1,files);
         fseek(dir,(curr->data)+64,SEEK_SET); // this somehow edits 68
         fwrite(&minpos,4,1,dir); // places the minimum value into the directory_table's offset
         minpos = minpos+(curr->length);
         printf("end of testrun\n");
      }else if(minpos == curr->offset){
         minpos = minpos+(curr->length);
      }
      curr = curr->next;
      printf("end of loop\n");
   }
   // free Nodes
   curr = head;
   Node* after = head->next;
   while(after!=NULL){
      free(curr);
      curr = after;
      after = after->next;
   }
   free(curr);
   // fseek(dir,64,SEEK_SET);
   // fread(&test,1,1,dir);
   // printf("offset 64 has %u (decimal)\n",test[0]);
   // fseek(dir,68,SEEK_SET);
   // fread(&test,1,1,dir);
   // printf("offset 68 has %u (decimal)\n",test[0]);
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
