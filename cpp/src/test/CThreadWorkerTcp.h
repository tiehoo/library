// TCP server, each thread accept. 
#ifndef _CTHREADWORKERTCP_H
#define _CTHREADWORKERTCP_H

#include "CSocketStream.h"
#include "CMutex.h"
#include "CThreadWorker.h"

class CThreadWorkerTcp : public CThreadWorker 
{
private: 
    CSocketStream   *m_pSocketListen;
    CMutex          *m_pSocketListenMutex;
public: 
    CThreadWorkerTcp();
// 
    void SetSocketListen(CSocketStream *pSocketListen, CMutex *pSocketListenMutex);
// Override Process() 
    virtual void*   Process(void *pArgument);
};

#endif
