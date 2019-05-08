#include<stdio.h>


int main(int argc, char** argv){
   char * test = "123";
   char a = 'b';
   printf("a value in int is %d\n", (int) a);



   printf("%c,%c,%c,%c,%c\n",test[0],test[1],test[2],test[3],test[4]);
   if(test[3]=='\0'){
      printf("test\n");
   }
   if(test[4]=='\0'){
      printf("test2\n");
   }
   char * abc = "123\0";
   if(abc[3]=='\0'){
      printf("|||||prev\n");
   }
   if(abc[4]=='\0'){
      printf("|||||\n");
   }


   return 0;


}
