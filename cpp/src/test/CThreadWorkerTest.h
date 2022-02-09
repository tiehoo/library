// 
#ifndef _CTHREADWORKERTEST_H
#define _CTHREADWORKERTEST_H

#include "CSocketStream.h"
#include "CMutex.h"
#include "CCondition.h"
#include "CSemaphore.h"
#include "CThreadWorker.h"

class CThreadWorkerTest : public CThreadWorker 
{
private: 
    CMutex          *m_pMutex;
    CCondition      *m_pCondition;
    CSemaphore      *m_pSemaphore;
public: 
    CThreadWorkerTest();
// 
    void Set(CMutex *pMutex, CCondition *pCondition, CSemaphore *pSemaphore);
// Override Process() 
    void*   Process(void *pArgument);
};

#endif
