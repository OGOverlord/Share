#include<stdio.h>
#include<unistd.h>


int main(){
   pid_t pid = 0;
   int df[2];
   pipe(df);
   int sum = 12323123;
   // char * test = malloc();
   // test[0] = (char) sum;
   char* test;
   printf("%d\n",sum);
   // sprintf(test,"%d",sum);
   printf("where is the segmentation fault?\n");
   write(df[1],&sum,sizeof(int));
   // write(df[1],test,sizeof(char*));
   // char* string;
   // read(df[0],string,sizeof(char*));
   // printf("stringg is %s\n",string);
   int a;
   read(df[0],&a,sizeof(a));

   printf("a is %d\n",a);
   return 0;
}
