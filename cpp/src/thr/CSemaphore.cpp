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
#include "CException.h"
#include "CDateTime.h"
#include "CSemaphore.h"
#include <errno.h>

// --------------------------------------------------------------------------- /
// CSemaphore 1.0
// --------------------------------------------------------------------------- /
CSemaphore::CSemaphore(unsigned int nValue, int bShared) 
{
    try
    {
        // int sem_init(sem_t *sem, int pshared, unsigned int value);
        // Initialising a semaphore that has already been initialised results in undefined behaviour.
        if(0 != sem_init(&m_semId, bShared, nValue)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CSemaphore::CSemaphore(): sem_init() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::CSemaphore(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSemaphore::~CSemaphore() 
{
    try
    {
        // Destroying a semaphore that other processes or threads are currently blocked on (in sem_wait(3)) produces undefined behaviour.
        if(0 != sem_destroy(&m_semId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CSemaphore::CSemaphore(): sem_destroy() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::~CSemaphore(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
bool CSemaphore::TryWait() 
{
    try
    {
        // 
        if(0 != sem_trywait(&m_semId)) 
        {
            switch(errno) 
            {
            case EAGAIN: 
                return(false);  // Resource is occupied. Value is 0. 
            default: 
                char szErr[256];
                throw CException(-1, "%s(%ld): CSemaphore::TryWait(): sem_trywait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }else return(true);     // Decreased. 
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::TryWait(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSemaphore::Wait() 
{
    try
    {
        //  Lock: 
        if(0 != sem_wait(&m_semId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CSemaphore::Wait(): sem_wait() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::Wait(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSemaphore::Wait(long lMillisecond) 
{
    try
    {
        // Get current time and add lMillisecond: 
        CDateTime objDateTime;
        objDateTime.SetToNow();
        objDateTime = objDateTime + (lMillisecond*1000);    // Convert to Microsecond(1/1000). 
        
        // Get timespec
        struct timespec stTimeSpec;
        objDateTime.GetTimeSpec(stTimeSpec);
        
        // 
        if(0 != sem_timedwait(&m_semId, &stTimeSpec)) 
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
        return(true);           // Decreased. 
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::Wait(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSemaphore::Post() 
{
    try
    {
        //  Decrease: 
        if(0 != sem_post(&m_semId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CSemaphore::Post(): sem_post() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSemaphore::Post(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
