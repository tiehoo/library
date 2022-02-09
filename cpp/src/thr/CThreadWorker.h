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
#ifndef _CTHREADWORKER_H
#define _CTHREADWORKER_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */

// --------------------------------------------------------------------------- /
// CThreadWorker 1.0
// --------------------------------------------------------------------------- /
// Constants: 
// 
class CThreadWorker
{
private: 
    // Supress copy: 
    explicit CThreadWorker(const CThreadWorker& refThreadWorker);
    const CThreadWorker& operator = (const CThreadWorker& refThreadWorker);
protected: 
    unsigned long       m_lId;                      // 
    pthread_t           m_threadId;                 // Thread ID. unsigned int. 
    // 
    bool                m_bInitialized;             // 
    bool                m_bShuttingDown;            // Is shutdown requested. 
public: 
    // 
    void*               m_pArgument;                // Used to pass the parameter to the wrapper. 
    // 
    CThreadWorker();
    virtual ~CThreadWorker();
// 
    void                Run(void *pArgument = NULL, const pthread_attr_t *pAttribute = NULL);
// 
    const pthread_t     GetId() const;              // Get thread ID which has been created. 
                                                    // We can not return a copy, so we returned a reference. 
    const pthread_t     GetSelfId() const;          // Get caller's thread ID. 
// 
    void                Join(void **pExitStatus = NULL);
    void                Detach();
// 
    // Manager thread can call this function tell the thread to exit. 
    void                Shutdown();                 // 
    bool                IsShuttingDown() const;     // 
// 
    // Overwrite this function to implement this thread. 
    virtual void*       Process(void *pArgument);
};

#endif
