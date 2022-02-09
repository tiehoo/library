/*
Compile: 
cc -Wall -c lib_static_test1.c lib_static_test2.c
# Make a static library: 
Create library "libctest.a": 
rm libstatictest.a
ar -cvq libstatictest.a lib_static_test1.o lib_static_test2.o
nm -l libstatictest.a
*/
#include <stdio.h>

void func1() 
{
    printf("func1()...\n");
}

void func2() 
{
    printf("func2.1()...\n");
}
