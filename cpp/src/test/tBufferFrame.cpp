#include <stdio.h>
#include <string.h>
#include "CException.h"
#include "CBufferFrame.h"

int main()
{
    try
    {
        char pBuffer[8];
        CBufferFrame objBufferFrame;
        // 0 size buffer: 
        objBufferFrame.BindBuffer(pBuffer, 0, 0);
        // 
        /*
        objBufferFrame.Append("A", 1);
        objBufferFrame.Write(0, "A", -1);
        printf("Insert: %ld.\n", objBufferFrame.Insert(0, "C", 1));
        objBufferFrame.Print();
        printf("Insert: %ld.\n", objBufferFrame.Replace(0, 1, "D", 1));
        objBufferFrame.Print();
        printf("Trim: %ld.\n", objBufferFrame.Trim(1));
        objBufferFrame.Print();
        printf("Erase: %ld.\n", objBufferFrame.Erase(0, 1));
        objBufferFrame.Print();
        */
        /*
        // 1 byte buffer: 
        objBufferFrame.BindBuffer(pBuffer, 2, 0);
        // 
        printf("Append: %ld.\n", objBufferFrame.Append("A", 1));
        objBufferFrame.Print();
        printf("Write: %ld.\n", objBufferFrame.Write(0, "B", -1));
        objBufferFrame.Print();
        printf("Insert: %ld.\n", objBufferFrame.Insert(0, "C", -1));
        objBufferFrame.Print();
        printf("Insert: %ld.\n", objBufferFrame.Replace(0, 1, "D", 1));
        objBufferFrame.Print();
        printf("Erase: %ld.\n", objBufferFrame.Erase(0, 1));
        objBufferFrame.Print();
        printf("Trim: %ld.\n", objBufferFrame.Trim(1));
        objBufferFrame.Print();
        */
        /*
        // Read: 
        char cChar;
        strcpy(pBuffer, "abc");
        objBufferFrame.BindBuffer(pBuffer, 8, 2);
        // 
        if(objBufferFrame.GetChar(0, cChar)) printf("0: [%c]\n", cChar);
        else printf("0: Not found.\n");
        if(objBufferFrame.GetChar(1, cChar)) printf("1: [%c]\n", cChar);
        else printf("1: Not found.\n");
        if(objBufferFrame.GetChar(2, cChar)) printf("2: [%c]\n", cChar);
        else printf("2: Not found.\n");
        */
        /*
        // Test GetChar speed: 
        char cCharTest;
        char pBufferTest[1024*1024];
        objBufferFrame.BindBuffer(pBufferTest, sizeof(pBufferTest), sizeof(pBufferTest));
        long i, j, k, l;
        for(l = 0; l < 1024 * 1024 * 1024; l++)
        {
            for(k = 0; k < 1024 * 1024 * 1024; k++)
            {
                for(j = 0; j < 1024 * 1024 * 1024; j++)
                {
                    for(i = 0; i < sizeof(pBufferTest); i++) 
                    {
                        //objBufferFrame.GetChar(i, cCharTest);
                        cCharTest = pBufferTest[i];
                    }
                }
            }
        }
        printf("l: %ld.\n", l);
        */
        strcpy(pBuffer, "12345678");
        objBufferFrame.BindBuffer(pBuffer, 8, 7);
        objBufferFrame.Print();
        objBufferFrame.Print(1, 2);
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
