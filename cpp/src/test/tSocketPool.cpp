#include <stdio.h>
#include "CException.h"
#include "CSocketStream.h"
#include "CSocketPool.h"

// 
int main()
{
    try
    {
        // 
        CSocketPool objSocketPool;
        // 
        CSocketStream *pSocket, *pSocket1;
        pSocket = (CSocketStream*)objSocketPool.AllocateSocket(AF_INET, SOCK_STREAM, true, true);
        if(NULL == pSocket)
        {
            // Internal erro: 
            throw CException(-1, "%s(%ld): main: Internal error, NULL pointer.", 
                __FILE__, __LINE__);
        }
        // Bind address: 
        CSocketAddressInet  objAddressServer;
        objAddressServer.SetPort(5010);
        pSocket->Bind(objAddressServer);
        pSocket->Listen();
        // 
        objSocketPool.SetSocketAddress(pSocket->GetId(), objAddressServer.GetString());
        // 
        printf("Size: %d, ID: %ld, Address: %s\n", objSocketPool.GetSize(), pSocket->GetId(), pSocket->GetAddressString().c_str());
        // 
        long lSocketId;
        if(objSocketPool.GetSocketId(objAddressServer.GetString(), lSocketId))
        {
            printf("Found socket with address: %s, ID: %ld\n", objAddressServer.GetString().c_str(), lSocketId);
        }else
        {
            printf("Can not find socket with address: %s\n", objAddressServer.GetString().c_str());
        }
        if(objSocketPool.DoesSocketExist(pSocket->GetId()))
        {
            pSocket1 = (CSocketStream*)objSocketPool.GetSocket(pSocket->GetId());
            printf("Found socket with ID: %ld, address: %s\n", pSocket->GetId(), pSocket1->GetAddressString().c_str());
        }else
        {
            printf("Can not find socket with ID: %ld\n", pSocket->GetId());
        }
        if(objSocketPool.DoesSocketExist(objAddressServer.GetString()))
        {
            pSocket1 = (CSocketStream*)objSocketPool.GetSocket(objAddressServer.GetString());
            printf("Found socket with address: %s, ID: %ld\n", objAddressServer.GetString().c_str(), pSocket->GetId());
        }else
        {
            printf("Can not find socket with ID: %s\n", objAddressServer.GetString().c_str());
        }
        // Select(): 
        int nReadyCount;
        std::list<CSocket*>::iterator iSocket;
        std::list<CSocket*> listSocket;
        CTimeInterval objTimeInterval(10, 0);
        // 
        nReadyCount = objSocketPool.Select(listSocket, &objTimeInterval);
        printf("Ready count: %d, %d\n", nReadyCount, listSocket.size());
        if(0 == nReadyCount)
        {
            // Time out: 
            printf("Select(): Time out.\n");
        }else if(nReadyCount > 0)
        {
            // Read ready: 
            for(iSocket = listSocket.begin(); iSocket != listSocket.end(); ++iSocket)
            {
                if(NULL == (*iSocket))
                {
                    throw CException(-1, "%s(%ld): main: Internal error, NULL pointer.", 
                        __FILE__, __LINE__);
                }
                printf("Ready ID: %ld\n", (*iSocket)->GetId());
            }
        }else
        {
            // Internal erro: 
            throw CException(-1, "%s(%ld): main: Internal error, Select() returned minus value: %d.", 
                __FILE__, __LINE__, nReadyCount);
        }
        // 
        objSocketPool.FreeSocket(pSocket->GetId());
        // 
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
