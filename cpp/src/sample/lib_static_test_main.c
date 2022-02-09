/*
cc -o lib_static_test lib_static_test_main.c libstatictest.a
cc -o lib_static_test lib_static_test_main.c -L`pwd` -lstatictest
./lib_static_test
*/

#include <stdio.h>
void func1();
void func2();

// 
int main(int argc, char* argv[]) 
{
    func1();
    func2();
    func3();
    return(0);
}
