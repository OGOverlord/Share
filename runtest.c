#include <stdio.h>

#define TEST(x) test(x, #x)
#include "myfilesystem.h"
#include "myfilesystem.c"

/* You are free to modify any part of this file. The only requirement is that when it is run, all your tests are automatically executed */

/* Some example unit test functions */
int success() {
    return 0;
}

int failure() {
    return 1;
}

int no_operation() {
    void * helper = init_fs("file1.bin", "file2.bin", "file3.bin", 1); // Remember you need to provide your own test files and also check their contents as part of testing
    close_fs(helper);
    return 0;
}


int already(){
   void * helper = init_fs("before/03_file_data", "before/03_directory_table", "before/03_hash_data", 4);
   int ret = create_file("document.txt", 1, helper);
/*   if(ret==1){
      printf("Document case succeeded in what I wanted it to do.");
   }
   if(ret!=1){
      close_fs(helper);
      return 1; //fail
   }else{
      close_fs(helper);
      return 0;
   }
*/
   ret = create_file("config", 3, helper);
   close_fs(helper);
   return ret;
}

/****************************/

/* Helper function */
void test(int (*test_function) (), char * function_name) {
    int ret = test_function();
    if (ret == 0) {
        printf("Passed %s\n", function_name);
    } else {
        printf("Failed %s returned %d\n", function_name, ret);
    }
}
/************************/







int main(int argc, char * argv[]) {

    // You can use the TEST macro as TEST(x) to run a test function named "x"
    TEST(success);
    TEST(failure);
    TEST(no_operation);
    TEST(already);

    // Add more tests here

    return 0;
}
