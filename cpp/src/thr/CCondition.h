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
#ifndef _CCONDITION_H
#define _CCONDITION_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <bitset>
#include <pthread.h>
#include "CMutex.h"

// --------------------------------------------------------------------------- /
// CCondition 1.0
// --------------------------------------------------------------------------- /
// Constants: 

// 
class CCondition
{
private: 
// 
    static const size_t         SIZE_BITSET = 512;
// 
    pthread_cond_t              m_nId;              // Condition ID. 
    CMutex                      &m_refMutexWait;    // Use for pthread_cond_wait and pthread_cond_timedwait
    // 
    std::bitset<SIZE_BITSET>    m_bitEventFlag;     // 
// 
    // Suppress copying
    explicit CCondition(const CCondition& refCondition);
    const CCondition& operator = (const CCondition& refCondition);
public: 
    explicit CCondition(CMutex &refMutexWait, const pthread_condattr_t *pAttribute = NULL);   // 
    virtual ~CCondition();                          // 

// 
    // Wait for the condition: The calling thread need to lock the mutex. 
    void            Wait();
    // Return true if condition was woken up, false if timed out. 
    bool            Wait(const struct timespec *pAbsTime);
    // 
    void            Signal();               // Signal one waitting thread. 
    void            Broadcast();            // Broadcast all waitting thread. 

//  Wait for the condition: Will lock mutex internally. 
    void            WaitSimple();
    bool            WaitSimple(long lMillisecond);
    // 
    void            SignalSimple();
    void            BroadcastSimple();

//  Wait for the condition and event flag: 
    void            SetEventBit(const size_t nEventBit);
    void            ResetEventBit(const size_t nEventBit);
    // 
    void            WaitSimple(const size_t nEventBit);
    bool            WaitSimple(const size_t nEventBit, long lMillisecond);
    // 
    void            SignalSimple(const size_t nEventBit);
    void            BroadcastSimple(const size_t nEventBit);
};

#endif
