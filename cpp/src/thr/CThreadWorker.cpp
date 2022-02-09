/* =========================================================================== /
* @category     POSIX THREAD
* @package      THREAD
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines here. 
/============================================================================ */
#include <errno.h>
#include <string.h>
// 
#include "CException.h" 
#include "CThreadWorker.h"

// --------------------------------------------------------------------------- /
// CThreadWorker 1.0
// --------------------------------------------------------------------------- /
extern "C"
{
	void* ThreadWrapper(void* pThreadObject) 
	{
	    if(NULL == pThreadObject) 
	    {
	        throw CException(-1, "%s(%ld): void* ThreadWrapper(void*): NULL pointer pThreadObject.", __FILE__, __LINE__);
	    }
	    CThreadWorker* pThreadWorker = (CThreadWorker*)(pThreadObject);
	    if(NULL == pThreadWorker) 
	    {
	        throw CException(-1, "%s(%ld): void* ThreadWrapper(): Internal error, can not convert void* to CThreadWorker*, get NULL pointer.", __FILE__, __LINE__);
	    }
	    return(pThreadWorker->Process(pThreadWorker->m_pArgument));
	}
}

// 
CThreadWorker::CThreadWorker():m_lId(0), m_bShuttingDown(false), m_bInitialized(false) 
{
}

CThreadWorker::~CThreadWorker() 
{
}

// --------------------------------------------------------------------------- /
void CThreadWorker::Run(void *pArgument, const pthread_attr_t *pAttribute) 
{
    try
    {
        // Thead ID which has been created: 
        if(m_bInitialized) 
        {
            throw CException(-1, "%s(%ld): CThreadWorker::Join(): Thread is runing, can not run again.", 
                __FILE__, __LINE__);
        }
        // Spawn thread, no attribute, pass this as wrapper's parameter: 
        /*
        int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void*), void *restrict arg);
        */
        m_pArgument = pArgument;
        // 
        if(0 != pthread_create(&m_threadId, pAttribute, ThreadWrapper, this)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CThreadWorker::Run(): pthread_create() failed, [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
        m_bInitialized = true;
        // 
        m_lId = (unsigned long)m_threadId;
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorker::CThreadWorker(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
const unsigned long CThreadWorker::GetId() const 
{
    try
    {
        // Thead ID which has been created: 
        if(!m_bInitialized) 
        {
            throw CException(-1, "%s(%ld): CThreadWorker::Join(): Thread is not initialized or has terminated.", 
                __FILE__, __LINE__);
        }
        return(m_threadId);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorker::GetId(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
const pthread_t CThreadWorker::GetSelfId() const 
{
    // Caller's ID, caller could be the main thread. 
    return(pthread_self());
}

// --------------------------------------------------------------------------- /
/*
The results of multiple simultaneous calls to pthread_join() specifying the same target 
       thread are undefined. 
*/
void CThreadWorker::Join(void **pExitStatus) 
{
    try
    {
        if(!m_bInitialized) 
        {
            throw CException(-1, "%s(%ld): CThreadWorker::Join(): Thread is not initialized or has terminated, can not join.", __FILE__, __LINE__);
        }
        // Join the thread: 
        // Don't join itself: Only be joined by the manager thread. 
        if(GetId() != GetSelfId()) 
        {
            if(0 != pthread_join(m_threadId, pExitStatus)) 
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CThreadWorker::Join(): pthread_join() failed, [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorker::Join(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadWorker::Detach() 
{
    try
    {
        if(!m_bInitialized) 
        {
            throw CException(-1, "%s(%ld): CThreadWorker::Detach(): Thread is not initialized or has terminated, can not join.", __FILE__, __LINE__);
        }
        // Join the thread: 
        // Don't detach others: Only detach itself. 
        if(GetId() == GetSelfId())
        {
            if(0 != pthread_detach(m_threadId)) 
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CThreadWorker::Detach(): pthread_detach() failed, [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorker::Detach(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CThreadWorker::IsShuttingDown() const
{
    return(m_bShuttingDown);
}

void CThreadWorker::Shutdown() 
{
    m_bShuttingDown = true;
}

// --------------------------------------------------------------------------- /
// Overwrite this function to implement this thread. 
void* CThreadWorker::Process(void *pArgument) 
{
    try
    {
        // Here is only an example of threaded process structure: 
        
        // Thread loop: 
        while(!IsShuttingDown()) 
        {
            // Do stuff here: 
            throw CException(-1, "%s(%ld): CThreadWorker::Process(): Called virtual base, consider overwrite this function.", __FILE__, __LINE__);
            sleep(1);
        }
        
        // Call ThreadExit(), notify waitting threads. 
        m_bInitialized = false;
        return(pArgument);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorker::Process(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
