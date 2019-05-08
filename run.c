#include <stdio.h>

#define TEST(x) test(x, #x)
#include "myfilesystem.h"
#include "myfilesystemFail.c"




int main(int argc, char * argv[]) {
   printf("test1\n");
   void * helper = init_fs("file1.bin","file2.bin","file3.bin",1);
   printf("test\n");
   create_file("test",100,helper);
   close_fs(helper);
   return 0;
}
