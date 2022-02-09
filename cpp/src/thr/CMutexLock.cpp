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
#include "CException.h"
#include "CMutexLock.h"

// --------------------------------------------------------------------------- /
// CMutexLock 1.0
// --------------------------------------------------------------------------- /
CMutexLock::CMutexLock(CMutex& refMutex):m_refMutex(refMutex) 
{
    try
    {
        m_refMutex.Lock();
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMutexLock::CMutexLock(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CMutexLock::~CMutexLock()
{
    try
    {
        m_refMutex.Unlock();
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CMutexLock::~CMutexLock(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
