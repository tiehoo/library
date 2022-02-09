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
#ifndef _CSEMAPHORE_H
#define _CSEMAPHORE_H

/* =========================================================================== /
* Place includes, constant defines here. 
/============================================================================ */
#include <semaphore.h>

// --------------------------------------------------------------------------- /
// CSemaphore 1.0
// --------------------------------------------------------------------------- /
// Constants: 

// 
class CSemaphore 
{
private: 
    // 
    sem_t m_semId;      // Lock ID. 
// 
    // Suppress copying
    explicit CSemaphore(const CSemaphore& refSemaphore);
    const CSemaphore& operator = (const CSemaphore& refSemaphore);
public: 
    explicit CSemaphore(unsigned int nValue = 1, int bShared = 0);       // 
    virtual ~CSemaphore();                                              // 
// 
    bool    TryWait();                  // 
    void    Wait();                     // Decrease. 
    bool    Wait(long lMillisecond);    // 
    // 
    void    Post();                     // Increase. 
};

#endif
