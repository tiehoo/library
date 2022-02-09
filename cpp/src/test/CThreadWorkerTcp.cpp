// 
#include <stdio.h>
#include "CException.h"
#include "CSocketAddressInet.h"
#include "CMutex.h"
#include "CMutexLock.h"
#include "CThreadWorkerTcp.h"

//------------------------------------------------------------------------------
// CThreadWorker 1.0
//------------------------------------------------------------------------------
CThreadWorkerTcp::CThreadWorkerTcp() 
{
    m_pSocketListen         = NULL;
    m_pSocketListenMutex    = NULL;
}

// 
void CThreadWorkerTcp::SetSocketListen(CSocketStream *pSocketListen, CMutex *pSocketListenMutex) 
{
    try
    {
        if(NULL == pSocketListen) 
        {
            throw CException(-1, "%s(%ld): CThreadWorkerTcp::SetSocketListen(): pSocketListen NULL pointer.", __FILE__, __LINE__);
        }
        m_pSocketListen = pSocketListen;
        // 
        if(NULL == pSocketListenMutex) 
        {
            throw CException(-1, "%s(%ld): CThreadWorkerTcp::SetSocketListen(): pSocketListenMutex NULL pointer.", __FILE__, __LINE__);
        }
        m_pSocketListenMutex = pSocketListenMutex;
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorkerTcp::SetSocketListen(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void* CThreadWorkerTcp::Process(void *pArgument) 
{
    try
    {
        if(NULL == m_pSocketListen || NULL == m_pSocketListenMutex) 
        {
            throw CException(-1, "%s(%ld): CThreadWorkerTcp::Process(): Please call CThreadWorkerTcp::SetSocketListen() first to set listening socket and mutex.", __FILE__, __LINE__);
        }
        // Receive data: 
        CSocketStream       objSocketClient(AF_INET, false);
        CSocketAddressInet  objAddressClient;
        // 
        while(1) 
        {
            {
                CMutexLock objMutexLock(*m_pSocketListenMutex);
                printf("\t%lu is accepting request now...\n", GetSelfId());
                m_pSocketListen->Accept(objSocketClient, objAddressClient);
                printf("(%lu) Connected from: %s.\n", GetSelfId(), objAddressClient.GetString().c_str());
            }
            // Read: 
            char arrBuf[1024];
            int nSize;
            // 
            while(1) 
            {
                if((nSize = objSocketClient.Read(arrBuf, sizeof(arrBuf))) > 0) 
                {
                    arrBuf[nSize] = '\0';
                    if(0 == strcmp("Q", arrBuf)) 
                    {
                        // 
                        objSocketClient.Close();
                        break;
                    }
                    printf("\t%ul Received: [%s]\n", GetId(), arrBuf);
                    nSize = objSocketClient.Write(arrBuf, nSize);
                }
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CThreadWorkerTcp::Process(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
