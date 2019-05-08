

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
      int count;
      FILE *ptr;
      struct record myRecord;
      ptr=fopen("test.bin","rb");
      if (!ptr){
         printf("Unable to open file!");
         return 1;
      }
      for ( count=1; count <= 10; count++){
         fread(&myRecord,sizeof(struct record),1,ptr);
         printf("%d\n",myRecord.a);
      }
      fclose(ptr);
      return 0;
   }
