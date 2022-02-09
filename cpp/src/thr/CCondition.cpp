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
// 
#include <string.h>
#include <errno.h>
#include "CException.h"
#include "CMutexLock.h"
#include "CDateTime.h"
#include "CCondition.h"

// --------------------------------------------------------------------------- /
// CCondition 1.0
// --------------------------------------------------------------------------- /
CCondition::CCondition(CMutex &refMutexWait, const pthread_condattr_t *pAttribute) : m_refMutexWait(refMutexWait) 
{
    // Attempting to initialize an already initialized condition variable results in undefined behavior. 
    if(0 != pthread_cond_init(&m_nId, pAttribute)) 
    {
        char szErr[256];
        throw CException(-1, "%s(%ld): CCondition::CCondition(): pthread_cond_init() failed: [%d]:[%s].", __FILE__, __LINE__, 
            errno, strerror_r(errno, szErr, sizeof(szErr)));
    }
}

CCondition::~CCondition() 
{
    // Attempting to destroy a condition variable upon which other threads are
    //   currently blocked results in undefined behavior.
    if(0 != pthread_cond_destroy(&m_nId)) 
    {
        char szErr[256];
        throw CException(-1, "%s(%ld): CCondition::~CCondition(): pthread_cond_destroy() failed: [%d]:[%s].", __FILE__, __LINE__, 
            errno, strerror_r(errno, szErr, sizeof(szErr)));
    }
}

/*
The pthread_cond_timedwait() and pthread_cond_wait() functions shall block on a condition variable. 
They shall be called with mutex locked by the calling thread or undefined behavior results. 
After pthread_cond_wait() or pthread_cond_timedwait() is called the system will let thread sleep and RELEASE the mutex lock. 
Upon successful return, the mutex shall have been locked and shall be owned by the calling thread. 
The effect of using more than one mutex for concurrent pthread_cond_timedwait() or pthread_cond_wait() operations on the same condition variable is undefined; 
*/
// --------------------------------------------------------------------------- /
// This version will let the caller lock the mutex. 
void CCondition::Wait() 
{
    try
    {
        // Will rely on caller to lock the mutex. 
        // Wait for the condition: 
        if(0 != pthread_cond_wait(&m_nId, &(m_refMutexWait.m_nId))) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Wait(): pthread_cond_wait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CCondition::Wait(const struct timespec *pAbsTime) 
{
    try
    {
        if(pAbsTime == NULL) 
        {
            throw CException(-1, "%s(%ld): CCondition::Wait(const struct timespec *): NULL pointer.", __FILE__, __LINE__);
        }
        
        // Will rely on caller to lock the mutex. 
        // Wait for the condition: 
        if(0 != pthread_cond_timedwait(&m_nId, &(m_refMutexWait.m_nId), pAbsTime)) 
        {
            switch(errno) 
            {
            case ETIMEDOUT: 
                return(false);  // Timed out. 
            default: 
                char szErr[256];
                throw CException(-1, "%s(%ld): CCondition::Wait(const struct timespec *): pthread_cond_timedwait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
        return(true);           // Condition arrived.
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(const struct timespec *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::Signal() 
{
    try
    {
        // Wake up one waitting thread: 
        if(0 != pthread_cond_signal(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Signal(): pthread_cond_signal() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CCondition::Signal(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::Broadcast() 
{
    try
    {
        // Wake up all waitting thread: 
        if(0 != pthread_cond_broadcast(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Broadcast(): pthread_cond_broadcast() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Broadcast(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}


// --------------------------------------------------------------------------- /
// This version will lock the mutex internally. 
void CCondition::WaitSimple() 
{
    try
    {
        // Lock mutext: 
        CMutexLock objLock(m_refMutexWait);
        // Wait for the condition: 
        if(0 != pthread_cond_wait(&m_nId, &(m_refMutexWait.m_nId))) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Wait(): pthread_cond_wait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CCondition::WaitSimple(long lMillisecond) 
{
    try
    {
        if(lMillisecond <= 0) 
        {
            throw CException(-1, "%s(%ld): CCondition::Wait(long): Invalid time out: %ld.", __FILE__, __LINE__, lMillisecond);
        }
        // Get current time and add lMillisecond: 
        CDateTime objDateTime;
        objDateTime.SetToNow();
        objDateTime = objDateTime + (lMillisecond*1000);    // Convert to Microsecond(1/1000). 
        
        // Get timespec
        struct timespec stTimeSpec;
        objDateTime.GetTimeSpec(stTimeSpec);
        
        // Wait for the condition: 
        CMutexLock objLock(m_refMutexWait);
        // 
        if(0 != pthread_cond_timedwait(&m_nId, &(m_refMutexWait.m_nId), &stTimeSpec)) 
        {
            switch(errno) 
            {
            case ETIMEDOUT: 
                return(false);  // Timed out. 
            default: 
                char szErr[256];
                throw CException(-1, "%s(%ld): CCondition::Wait(long): pthread_cond_timedwait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
        return(true);           // Condition arrived.
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::SignalSimple() 
{
    try
    {
        // 
        CMutexLock objLock(m_refMutexWait);
        // Wake up one waitting thread: 
        if(0 != pthread_cond_signal(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Signal(): pthread_cond_signal() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Signal(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::BroadcastSimple()
{
    try
    {
        // 
        CMutexLock objLock(m_refMutexWait);
        // Wake up all waitting thread: 
        if(0 != pthread_cond_broadcast(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Broadcast(): pthread_cond_broadcast() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CCondition::Broadcast(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// This version will lock the mutex internally and test event bit. 
void CCondition::SetEventBit(const size_t nEventBit) 
{
    m_bitEventFlag.set(nEventBit);
}

void CCondition::ResetEventBit(const size_t nEventBit) 
{
    m_bitEventFlag.reset(nEventBit);
}

void CCondition::WaitSimple(const size_t nEventBit) 
{
    try
    {
        // Lock mutext: 
        while(1) 
        {
            CMutexLock objLock(m_refMutexWait);
            // Wait for the condition: 
            if(0 != pthread_cond_wait(&m_nId, &(m_refMutexWait.m_nId))) 
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CCondition::Wait(): pthread_cond_wait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
            if(m_bitEventFlag.test(nEventBit)) 
            {
                break;
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CCondition::WaitSimple(const size_t nEventBit, long lMillisecond) 
{
    try
    {
        if(lMillisecond <= 0) 
        {
            throw CException(-1, "%s(%ld): CCondition::Wait(long): Invalid time out: %ld.", __FILE__, __LINE__, lMillisecond);
        }
        // Get current time and add lMillisecond: 
        CDateTime objDateTime;
        objDateTime.SetToNow();
        objDateTime = objDateTime + (lMillisecond*1000);    // Convert to Microsecond(1/1000). 
        
        // Get timespec
        struct timespec stTimeSpec;
        objDateTime.GetTimeSpec(stTimeSpec);
        
        // Wait for the condition: 
        while(1) 
        {
            CMutexLock objLock(m_refMutexWait);
            // 
            if(0 != pthread_cond_timedwait(&m_nId, &(m_refMutexWait.m_nId), &stTimeSpec)) 
            {
                switch(errno) 
                {
                case ETIMEDOUT: 
                    return(false);  // Timed out. 
                default: 
                    char szErr[256];
                    throw CException(-1, "%s(%ld): CCondition::Wait(long): pthread_cond_timedwait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                        errno, strerror_r(errno, szErr, sizeof(szErr)));
                }
            }
            if(m_bitEventFlag.test(nEventBit)) 
            {
                return(true);           // Condition arrived. 
            }
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Wait(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::SignalSimple(const size_t nEventBit) 
{
    try
    {
        // 
        CMutexLock objLock(m_refMutexWait);
        // 
        SetEventBit(nEventBit);
        // Wake up one waitting thread: 
        if(0 != pthread_cond_signal(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Signal(): pthread_cond_signal() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CCondition::Signal(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCondition::BroadcastSimple(const size_t nEventBit)
{
    try
    {
        // 
        CMutexLock objLock(m_refMutexWait);
        // 
        SetEventBit(nEventBit);
        // Wake up all waitting thread: 
        if(0 != pthread_cond_broadcast(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CCondition::Broadcast(): pthread_cond_broadcast() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CCondition::Broadcast(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
