#include <stdio.h>
#include "CException.h"
#include "CBuffer.h"

int main()
{
    try
    {
//        CBuffer (arrBuffer("abc"))[10];
        CBuffer objBuffer1("1234567890");
        printf("Buffer: (%ld)[%s].\n", objBuffer1.GetSize(), objBuffer1.c_str());
        // 
        CBuffer objBuffer2(objBuffer1);
        printf("Buffer: [%s].\n", objBuffer2.c_str());
        // 
        CBuffer objBuffer3(8);
        /* Exception: 
        objBuffer3.SetBuffer("1234567890", 9);
        */
        objBuffer3.SetBuffer("1234567890", 8);
        printf("Buffer: [%s].\n", objBuffer3.c_str());
        // 
        objBuffer3.SetBuffer("123456");
        printf("Buffer: [%s].\n", objBuffer3.c_str());
        // 
        objBuffer1 = objBuffer3;
        printf("Buffer: [%s].\n", objBuffer1.c_str());
        // 
        /*
        objBuffer2 = "12345678901";
        */
        objBuffer2 = "123456789";
        printf("Buffer: [%s].\n", objBuffer2.c_str());
        // 
        
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
