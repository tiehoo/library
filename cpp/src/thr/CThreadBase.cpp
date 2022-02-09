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
#include <string.h>
#include <errno.h>
// 
#include "CException.h"
#include "CMutexLock.h"
#include "CThreadBase.h"

// --------------------------------------------------------------------------- /
// CThreadBase 1.0
// --------------------------------------------------------------------------- /
extern "C"
{
	void* ThreadWrapper(void* pObject)
	{
	    if(NULL == pObject)
	    {
	        throw CException(-1, "%s(%ld): void* ThreadWrapper(): NULL pointer.", __FILE__, __LINE__);
	    }
	    CThreadBase* pThreadBase = static_cast<CThreadBase*> (pObject);
	    if(NULL == pThreadBase)
	    {
	        throw CException(-1, "%s(%ld): void* ThreadWrapper(): Internal error, can not convert void* to CThreadBase*, get NULL pointer.", __FILE__, __LINE__);
	    }
	    pThreadBase->Process();
	    return(NULL);
	}
}

// 
CThreadBase::CThreadBase():m_nId(0), m_bShuttingDown(false), m_objCondition(m_objMutexThreadBase)
{
    try
    {
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::CThreadBase(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CThreadBase::~CThreadBase()
{
    try
    {
        /*
        Shutdown();
        Join();
        */
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::~CThreadBase(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
pthread_t CThreadBase::GetId()
{
    // Thead ID which has been created: 
    return(m_nId);
}

pthread_t CThreadBase::GetSelfId()
{
    // Caller's ID, caller could be the main thread.
    return(pthread_self());
}

void CThreadBase::Run()
{
    try
    {
        if(m_nId != 0) 
        {
            throw CException(-1, "%s(%ld): CThreadBase::Run(): Thread already running, ID: [%d].", __FILE__, __LINE__, m_nId);
        }
        // Spawn thread, no attribute, pass this as wrapper's parameter: 
        /*
        int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void*), void *restrict arg);
        */
        if(0 != pthread_create(&m_nId, NULL, ThreadWrapper, this)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CThreadBase::Run(): pthread_create() failed, [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadBase::Run(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadBase::Join() 
{
    try
    {
        if(m_nId == 0)
        {
            throw CException(-1, "%s(%ld): CThreadBase::Join(): Thread not runing, call Run() first.", __FILE__, __LINE__);
        }
        // Join the thread: 
        // Don't join itself: Only be joined by others.
        if(GetId() != GetSelfId())
        {
            void* pExitStatus;
            if(0 != pthread_join(m_nId, &pExitStatus))
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CThreadBase::Join(): pthread_join() failed, [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
            // Thread exited: 
            m_nId = 0;
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::Join(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadBase::Detach()
{
    try
    {
        if(m_nId == 0)
        {
            throw CException(-1, "%s(%ld): CThreadBase::Detach(): Thread not runing, call Run() first.", __FILE__, __LINE__);
        }
        // Join the thread: 
        // Don't detach others: Only detach itself.
        if(GetId() == GetSelfId())
        {
            if(0 != pthread_detach(m_nId))
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CThreadBase::Detach(): pthread_detach() failed, [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::Detach(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CThreadBase::IsShuttingDown()
{
    try
    {
        CMutexLock objLock(m_objMutexThreadBase);
        return(m_bShuttingDown);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::IsShuttingDown(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadBase::Shutdown() 
{
    try
    {
        if(!IsShuttingDown())
        {
            // Get the lock: 
            CMutexLock objLock(m_objMutexThreadBase);
            m_bShuttingDown = true;
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::Shutdown(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadBase::ThreadExit(bool bBroadCast)
{
    try
    {
        // Always wait 1 second: 
        sleep(1);
        // Send signal to wake up waitting threads: 
        if(bBroadCast) m_objCondition.Broadcast();
        else m_objCondition.Signal();
        // Shutdown finished: 
        m_bShuttingDown = false;
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::ThreadExit(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CThreadBase::WaitForShutdown()
{
    try
    {
        m_objCondition.Wait();
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::WaitForShutdown(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CThreadBase::WaitForShutdown(long lMillisecond)
{
    try
    {
        return(m_objCondition.WaitSimple(lMillisecond));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::WaitForShutdown(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// Overwrite this function to implement this thread.
void CThreadBase::Process()
{
    try
    {
        // Here is only an example of threaded process structure: 
        
        // Thread loop: 
        while(!IsShuttingDown())
        {
            // Do stuff here: 
            sleep(1);
        }
        
        // Call ThreadExit(), notify waitting threads.
        ThreadExit();
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CThreadBase::Process(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
