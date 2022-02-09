// 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "CException.h"
#include "CMutex.h"
#include "CCondition.h"
#include "CSemaphore.h"
#include "CThreadWorkerTest.h"

// 
unsigned long lThreadId;
// 
CMutex      objMutex;
CCondition  objCondition(objMutex);
CSemaphore  objSemaphore(2);

// 
int main() 
{
    // 
    try 
    {
        // Workers: 
        char szBuffer[64] = "Hello data.";
	int nThreadCount=3, nTermCount=0;
        CThreadWorkerTest arrWorker[nThreadCount];

        // 
        for(int i = 0; i < nThreadCount; ++i) 
        {
            arrWorker[i].Set(&objMutex, &objCondition, &objSemaphore);
            arrWorker[i].Run(szBuffer);
            printf("Created: %lu\n", arrWorker[i].GetId());
        }
        
        // Wait and see who is terminated: 
        while(nTermCount<nThreadCount) 
        {
            objMutex.Lock();
            printf("waiting for condition now.\n", lThreadId);
            objCondition.Wait();
            nTermCount++;
            printf("%lu termiated. Total:%d\n", lThreadId, nTermCount);
            objMutex.Unlock();
        }
        
        // 
        exit(0);
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
