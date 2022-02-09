// 
/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
*/
#include <iostream>
#include "CException.h"
#include "CLog.h"
// 
using namespace std;
// 
int main()
{
    try
    {
        CLog objLog("tLog");
        objLog.WriteError("The error is: %s, %c, %d, %ld.", "Test error", 'd', 65535, 65539);
        objLog.WriteWarning("The warning is here.");
        objLog.SetTimeStampOn(true);
        objLog.WriteInfo("The information is here.");
        objLog.SetTimeStampOn(false);
        objLog.WriteDebug("The debug information is here.");
        objLog.SetLevel(LOG_LEVEL_NONE);
        objLog.WriteInfo("You should not see this.");
    }catch(CException &eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
