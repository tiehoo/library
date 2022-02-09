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
#include "CMutex.h"
#include <errno.h>

// --------------------------------------------------------------------------- /
// CMutex 1.0
// --------------------------------------------------------------------------- /
CMutex::CMutex(const pthread_mutexattr_t *pAttribute) 
{
    try
    {
        // int pthread_mutex_init(pthread_mutex_t *restrict mutex, 
        //          const pthread_mutexattr_t *restrict attr);
    
        // Upon successful initialization, the state of the mutex becomes 
        //   initialized and unlocked. 
        if(0 != pthread_mutex_init(&m_nId, pAttribute)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CMutex::CMutex(): pthread_mutex_init() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CMutex::CMutex(const pthread_mutexattr_t *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CMutex::~CMutex() 
{
    try
    {
        // Destroy: Attempting to destroy a locked mutex results in undefined behavior. 
        // If unlock failed, it will throw an expception, which causes it by passing this logic. 
        if(TryLock()) 
        {
            Unlock();
            // 
            if(0 != pthread_mutex_destroy(&m_nId)) 
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CMutex::~CMutex(): pthread_mutex_destroy() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CMutex::~CMutex(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
/*
       The  mutex  object referenced by mutex shall be locked by calling pthread_mutex_lock(). If the mutex is already
       locked, the calling thread shall block until the mutex becomes available.  This operation shall return with the
       mutex object referenced by mutex in the locked state with the calling thread as its owner.

       If  the  mutex type is PTHREAD_MUTEX_NORMAL, deadlock detection shall not be provided. Attempting to relock the
       mutex causes deadlock. If a thread attempts to unlock a mutex that it has  not  locked  or  a  mutex  which  is
       unlocked, undefined behavior results.

       If  the  mutex type is PTHREAD_MUTEX_ERRORCHECK, then error checking shall be provided. If a thread attempts to
       relock a mutex that it has already locked, an error shall be returned. If a thread attempts to unlock  a  mutex
       that it has not locked or a mutex which is unlocked, an error shall be returned.

       If the mutex type is PTHREAD_MUTEX_RECURSIVE, then the mutex shall maintain the concept of a lock count. When a
       thread successfully acquires a mutex for the first time, the lock count shall be  set  to  one.  Every  time  a
       thread  relocks this mutex, the lock count shall be incremented by one. Each time the thread unlocks the mutex,
       the lock count shall be decremented by one. When the lock count reaches zero, the mutex shall become  available
       for other threads to acquire. If a thread attempts to unlock a mutex that it has not locked or a mutex which is
       unlocked, an error shall be returned.

       If the mutex type is PTHREAD_MUTEX_DEFAULT, attempting to recursively  lock  the  mutex  results  in  undefined
       behavior. Attempting to unlock the mutex if it was not locked by the calling thread results in undefined behav-
       ior. Attempting to unlock the mutex if it is not locked results in undefined behavior.

       The pthread_mutex_trylock() function shall be equivalent to pthread_mutex_lock(),  except  that  if  the  mutex
       object  referenced  by  mutex is currently locked (by any thread, including the current thread), the call shall
       return immediately. If the mutex type is PTHREAD_MUTEX_RECURSIVE and the mutex is currently owned by the  call-
       ing  thread,  the  mutex  lock count shall be incremented by one and the pthread_mutex_trylock() function shall
       immediately return success.

       The pthread_mutex_unlock() function shall release the mutex object referenced by mutex.    The manner in  which
       a  mutex  is  released is dependent upon the mutex's type attribute.  If there are threads blocked on the mutex
       object referenced by mutex when pthread_mutex_unlock() is called, resulting in the  mutex  becoming  available,
       the scheduling policy shall determine which thread shall acquire the mutex.

       (In  the  case of PTHREAD_MUTEX_RECURSIVE mutexes, the mutex shall become available when the count reaches zero
       and the calling thread no longer has any locks on this mutex.)

       If a signal is delivered to a thread waiting for a mutex, upon return from the signal handler the thread  shall
       resume waiting for the mutex as if it was not interrupted.
*/
bool CMutex::TryLock() 
{
    try
    {
        // 
        if(0 != pthread_mutex_trylock(&m_nId)) 
        {
            switch(errno) 
            {
            case EBUSY: 
                return(false);  // Can not lock. 
            default: 
                char szErr[256];
                throw CException(-1, "%s(%ld): CMutex::TryLock(): pthread_mutex_trylock() failed: [%d]:[%s].", __FILE__, __LINE__, 
                    errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }else return(true);     // Locked. 
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CMutex::TryLock(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMutex::Lock() 
{
    try
    {
        //  Lock: 
        if(0 != pthread_mutex_lock(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CMutex::Lock(): pthread_mutex_lock() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMutex::Lock(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMutex::Unlock() 
{
    try
    {
        //  Unlock: 
        if(0 != pthread_mutex_unlock(&m_nId)) 
        {
            char szErr[256];
            throw CException(-1, "%s(%ld): CMutex::Unlock(): pthread_mutex_unlock() failed: [%d]:[%s].", __FILE__, __LINE__, 
                errno, strerror_r(errno, szErr, sizeof(szErr)));
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CMutex::Unlock(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
