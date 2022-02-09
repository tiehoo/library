// 
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "CException.h"
#include "CMutex.h"
#include "CMutexLock.h"
#include "CThreadWorkerTest.h"

//------------------------------------------------------------------------------
// CThreadWorker 1.0
//------------------------------------------------------------------------------
extern unsigned long lThreadId;
// 
CThreadWorkerTest::CThreadWorkerTest() 
{
    m_pMutex        = NULL;
    m_pCondition    = NULL;
    m_pSemaphore    = NULL;
}

// 
void CThreadWorkerTest::Set(CMutex *pMutex, CCondition *pCondition, CSemaphore *pSemaphore) 
{
    try
    {
        assert(pMutex);
        assert(pCondition);
        assert(pSemaphore);
        // 
        m_pMutex        = pMutex;
        m_pCondition    = pCondition;
        m_pSemaphore    = pSemaphore;
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorkerTest::SetSocketListen(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void* CThreadWorkerTest::Process(void *pArgument) 
{
    try
    {
        assert(m_pMutex);
        assert(m_pCondition);
        assert(m_pSemaphore);
        // Sleep a while: 
        int sec = rand() % 10;
        m_pSemaphore->Wait();
        printf("%lu Sleeping: %d, data: %s\n", GetSelfId(), sec, (char*)pArgument);
        sleep(sec);
        m_pSemaphore->Post();
        
        // Test condition: 
        {
            CMutexLock objLock(*m_pMutex);
            // 
            lThreadId = (unsigned long)GetSelfId();
            m_pCondition->Signal();
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorkerTest::Process(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
