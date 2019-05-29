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
   return ret;
}

int reading(){
   void * helper = init_fs("before/10_file_data", "before/10_directory_table", "before/10_hash_data", 4);
   char buf[11];
   int ret = read_file("file1.txt", 5, 50, buf, helper);
   ret = read_file("file1.txt", 5, 10, buf, helper);
   close_fs(helper);
   return ret;
}


int writing(){
   void * helper = init_fs("before/12_file_data", "before/12_directory_table", "before/12_hash_data", 4);
   char buf[] = "tests";
   int ret = write_file("file1.txt", 160, 50, buf, helper);
   ret = write_file("file1.txt", 16, 5, buf, helper);
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
    // TEST(already);
    TEST(writing);

    // Add more tests here

    return 0;
}
