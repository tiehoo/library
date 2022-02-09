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
#ifndef _CMUTEXLOCK_H
#define _CMUTEXLOCK_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <pthread.h>

// --------------------------------------------------------------------------- /
// CMutex 1.0
// --------------------------------------------------------------------------- /
// Constants: 

// 
class CMutex 
{
private: 
    // 
    pthread_mutex_t m_nId;      // Lock ID. 
// 
    // Suppress copying
    explicit CMutex(const CMutex& refMutex);
    const CMutex& operator = (const CMutex& refMutex);
public: 
    explicit CMutex(const pthread_mutexattr_t *pAttribute = NULL);  // 
    virtual ~CMutex();                                              // 
// 
    bool    TryLock();  // Return true if locked, false if busy.
    void    Lock();     // Will wait untile the lock is obtained. 
    void    Unlock();   // 
// 
friend class CCondition;
};

#endif
