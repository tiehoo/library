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
#ifndef _CMUTEXAUTOLOCK_H
#define _CMUTEXAUTOLOCK_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CMutex.h"

// --------------------------------------------------------------------------- /
// CMutexLock 1.0
// --------------------------------------------------------------------------- /
// Constants: 

// 
class CMutexLock
{
private: 
    CMutex&     m_refMutex;
public: 
    explicit CMutexLock(CMutex& refMutex);      // Constructor will auto lock.
    ~CMutexLock();                              // Destructor will auto unlock.
};

#endif
