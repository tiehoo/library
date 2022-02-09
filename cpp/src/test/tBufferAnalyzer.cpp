#include <stdio.h>
#include "CException.h"
#include "CBuffer.h"
#include "CBufferAnalyzer.h"

int main()
{
    try
    {
        // 
        CBuffer objBuffer(128);
        CBufferAnalyzer objAnalyer(objBuffer);
        
        // 
        long lLocation;
        char szString[128];
        /*
        // ToLower, ToUpper: 
        printf("%c, %c, %c, %c\n", objAnalyer.ToLower('a'), objAnalyer.ToLower('B'), objAnalyer.ToUpper('C'), objAnalyer.ToUpper('d'));
        
        // Find char: 
        strcpy(szString, "stephen@gmail.com");
        if(objAnalyer.IsCharInString('@', szString, strlen(szString), lLocation))
        {
            printf("Found char: %ld.\n", lLocation);
        }else
        {
            printf("Can not find char.\n");
        }
        // 
        if(objAnalyer.IsCharInString('x', szString, lLocation))
        {
            printf("Found char: %ld.\n", lLocation);
        }else
        {
            printf("Can not find char.\n");
        }
        
        // Find string: 
        if(objAnalyer.IsStringInString(szString, strlen(szString), szString, strlen(szString), lLocation))
        {
            printf("Found string: %ld.\n", lLocation);
        }else
        {
            printf("Can not find string.\n");
        }
        // 
        if(objAnalyer.IsStringInString("en@gm", szString, lLocation))
        {
            printf("Found string: %ld.\n", lLocation);
        }else
        {
            printf("Can not find string.\n");
        }
        // 
        if(objAnalyer.IsStringInString("en@gml", szString, lLocation))
        {
            printf("Found string: %ld.\n", lLocation);
        }else
        {
            printf("Can not find string.\n");
        }
        */
        // Main: 
        objBuffer = "0123456789abcdefg;";
        long lLenSkip;
        char cToken;
        /*
        if(objAnalyer.FindChar(";8", cToken, lLenSkip))
        {
            printf("Find: %c, %ld.\n", cToken, lLenSkip);
        }else printf("Can not find char.\n");
        // 
        if(objAnalyer.FindChar(";8", cToken, lLenSkip))
        {
            printf("Find: %c, %ld.\n", cToken, lLenSkip);
        }else printf("Can not find char.\n");
        // 
        if(objAnalyer.FindChar(";8", cToken, lLenSkip))
        {
            printf("Find: %c, %ld.\n", cToken, lLenSkip);
        }else printf("Can not find char.\n");
        */
        /*
        // 
        long i, j, k;
        objAnalyer.SetCaseInsensitive();
        //for(i = 0; i < 1024*1024*1024; ++i)
        {
            for(j = 0; j < 1024*1024*128; ++j)
            {
                objAnalyer.Rewind();
                objAnalyer.FindChar("8", cToken, lLenSkip);
            }
        }
        */
        // 
        if(objAnalyer.FindSequence("01", lLenSkip))
        {
            printf("Found: %ld.\n", lLenSkip);
        }else printf("Can not find sequence.\n");
/*
        if(objAnalyer.FindSequence("9a", lLenSkip))
        {
            printf("Found: %ld.\n", lLenSkip);
        }else printf("Can not find sequence.\n");
        if(objAnalyer.FindSequence("g;", lLenSkip))
        {
            printf("Found: %ld.\n", lLenSkip);
        }else printf("Can not find sequence.\n");
*/
        if(objAnalyer.FindSequence("01", lLenSkip))
        {
            printf("Found: %ld.\n", lLenSkip);
        }else printf("Can not find sequence. Skiped: %d.\n", lLenSkip);
        /*
        long i, j, k;
        objAnalyer.SetCaseInsensitive();
        //for(i = 0; i < 1024*1024*1024; ++i)
        {
            for(j = 0; j < 1024*1024*128; ++j)
            {
                objAnalyer.Rewind();
                objAnalyer.FindSequence("9a", lLenSkip);
            }
        }
        */
        /*
        // 
        lLenSkip = objAnalyer.Skip("1");
        printf("Skiped: %ld.\n", lLenSkip);
        lLenSkip = objAnalyer.Skip("0");
        printf("Skiped: %ld.\n", lLenSkip);
        lLenSkip = objAnalyer.Skip("12");
        printf("Skiped: %ld.\n", lLenSkip);
        */
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
