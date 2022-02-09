#include <stdio.h>
#include <iostream>
#include "CException.h"
#include "CErrorMap.h"

// 
using namespace std;
int main()
{
    try
    {
        // 
        CErrorMap objErrorMap;
        // You can also load it from file or database.
        objErrorMap.AppendError(11001, "Buffer overflowed.", "This is caused by a big network package.", "Either increase buffer size, or trncate the input before put into buffer.");
        // 
        printf("Error: [%ld][Description: %s Cause: %s Action: %s]\n", 11001, objErrorMap.GetErrorMessage(11001).c_str(), 
            objErrorMap.GetErrorCause(11001).c_str(), objErrorMap.GetErrorAction(11001).c_str());
        // 
        tErrorEntry     stErrorEntry;
        stErrorEntry    = objErrorMap.GetErrorEntry(11001);
        printf("Error: [%ld][Description: %s Cause: %s Action: %s]\n", 11001, stErrorEntry.sMessage.c_str(), 
            stErrorEntry.sCause.c_str(), stErrorEntry.sAction.c_str());
    }catch(CException& eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
