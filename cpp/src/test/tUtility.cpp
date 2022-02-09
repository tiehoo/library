#include <stdio.h>
#include <string.h>
#include "CUtility.h"

using namespace std;
int main()
{
    // 
    int nRandom;
    for(int i = 0; i < 10; ++i)
    {
        nRandom = CUtility::GetRandom(10);
        printf("0-%d: %d.\n", 10, nRandom);
    }
    // 
    char szString[17];
    char szSecret[17];
    char szResult[17];
    char szResult1[17];
    strcpy(szString, "1234567890abcde");
    strcpy(szSecret, "ABCDEFGHIJ01234");
    //
    CUtility::Xor((uint8_t*)szString, (uint8_t*)szSecret, (uint8_t*)szResult, 16);
    szResult[16] = '\0';
    printf("R: %s\n", szResult);
    // 
    CUtility::Xor((uint8_t*)szResult, (uint8_t*)szSecret, (uint8_t*)szResult1, 16);
    szResult1[16] = '\0';
    printf("R: %s\n", szResult1);
}
