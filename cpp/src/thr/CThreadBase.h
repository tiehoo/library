/* =========================================================================== /
* @category     POSIX THREAD
* @package      THEAD
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CTHREADBASE_H
#define _CTHREADBASE_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CMutex.h"
#include "CCondition.h"

// --------------------------------------------------------------------------- /
// CThreadBase 1.0
// --------------------------------------------------------------------------- /
// Constants: 

class CThreadBase
{
private: 
// Suppress copying
    explicit CThreadBase(const CThreadBase& refThreadBase);
    const CThreadBase& operator = (const CThreadBase& refThreadBase);
protected: 
    pthread_t       m_nId;                  // Thread ID. unsigned int.
    bool            m_bShuttingDown;        // Is shutdown requested.
    CMutex          m_objMutexThreadBase;   // Used for shutting down exclusive process.
    CCondition      m_objCondition;
public: 
    CThreadBase();
    virtual ~CThreadBase();
// 
    // Overwrite this function to implement this thread.
    virtual void    Process();
    // Call when thread exist, send signal.
    virtual void    ThreadExit(bool bBroadCast = false);
// 
    virtual void    Run();
    void            Shutdown();
    void            Join();
    void            Detach();
    // Wait forever until get condition signal.
    void            WaitForShutdown();
    // Wait no more than lMillisecond.
    bool            WaitForShutdown(long lMillisecond);
// 
    bool            IsShuttingDown();       // 
    pthread_t       GetId();                // Get thread ID which has been created.
    pthread_t       GetSelfId();            // Get caller's thread ID.
};

#endif
