#include <stdlib.h>
#include <stdio.h>
#include <string.h>

   // char * test = "123";
   // char a = 'b';
   // printf("a value in int is %d\n", (int) a);


   //
   // printf("%c,%c,%c,%c,%c\n",test[0],test[1],test[2],test[3],test[4]);
   // if(test[3]=='\0'){
   //    printf("test\n");
   // }
   // if(test[4]=='\0'){
   //    printf("test2\n");
   // }
   // char * abc = "123\0";
   // if(abc[3]=='\0'){
   //    printf("|||||prev\n");
   // }
   // if(abc[4]=='\0'){
   //    printf("|||||\n");
   // }




   #include<stdio.h>
   /* Our structure */
   struct record{
      int a,b,c;
   };

   int main(){
      // for(int k = 1; k<1; k++){
      //    printf("Shouldn't work\n");
      // }
      uint8_t * buf = 2^60;


      printf("finished checkign out the values here\n");
      uint64_t a = 0; //It needs to be 64 bytes so that a+data[i] doesn't go all the way around. before the mod is applied
      uint64_t b = 0;
      uint64_t c = 0;
      uint64_t d = 0;
      for(int i = 0; i<length; i++){
         a = (a + buf[i]) %((2^32)-1);
         b = (b + a) % ((2^32)-1);
         c = (c + b) %((2^32)-1);
         d = (d + c) % ((2^32)-1);
      }
      //uint8_t hash_value[16] = concatenate a, b, c, d //4x4 = 16 bytes in total
      uint8_t hash_value[16];
      hash_value[0]=(uint32_t)a;
      hash_value[4]=(uint32_t)b;
      hash_value[8]=(uint32_t)c;
      hash_value[12]=(uint32_t)d;
      //Note: when you concatenate a, b, c, d you must treat them as uint32_t
      //That is, treat them as 4 byte little endian integers, even though
      //we suggest you use uint64_t for the calculations
      memcpy(buf,hash_value,16);
   }
