#include <iostream>

#include "CException.h"
#include "CSocketStream.h"
#include "CSocketPool.h"

using namespace std;
// 
int main()
{
    // 
    try
    {
    char arrBuf[1024];
    int nSize;
        // Pool: 
        CSocketPool     objSocketPool;

        // Listen address: 
        CSocketAddressInet  objAddressServer;
        objAddressServer.SetPort(5010);

        // Allocate listener socket from pool: 
        CSocketStream*  pSocketListener = (CSocketStream*)  objSocketPool.AllocateSocket(AF_INET, SOCK_STREAM, true);

        // Bind, Listen: 
        pSocketListener->Bind(objAddressServer);
        pSocketListener->Listen();
        objSocketPool.SetSocketAddress(pSocketListener->GetId(), objAddressServer.GetString());

        // Poll or Select the pool: 
        std::list<CSocket*>::iterator iSocket;
        std::list<CSocket*> listSocket;
        int nReadCount;
        while(1)
        {
            nReadCount = objSocketPool.Select(listSocket, NULL);
            if(0 == nReadCount) 
            {
                // Time out: 
            }else if(nReadCount > 0)
            {
                // Read ready: 
                for(iSocket = listSocket.begin(); listSocket != listSocket.end(); ++iSocket)
                {
                    if(NULL == *iSocket)
                    {
                        continue;
                    }
printf("ID: %ld\n", *iSocket->GetId());
                }
            }else
            {
                // Internal erro: 
                throw CException(-1, "%s(%ld): main: Internal error, Select() returned minus value: %d.", 
                    __FILE__, __LINE__, nReadyCount);
            }
        }
/*
        // Network events arrived: 
            // If on a listener socket: 
                // Allocate a client socket from pool: 
                // Accept(): 
            // If on a client socket: Data arrived 
                // Read(): 
                // Write(): 
        
        CSocketStream       objSocket(true, true), objSocketClient(false, true);
        CSocketAddressInet  objAddressServer, objAddressClient;
        // Bind server address: 
        objAddressServer.SetPort(5010);
        objSocket.Bind(objAddressServer);
        // Listen: 
        objSocket.Listen();
        cout << "Listening on port: " << objAddressServer.GetPort() << endl;
        // Receive data: 
        while(1)
        {
            objSocket.Accept(objSocketClient, objAddressClient);
            cout << "Connected from: " << objAddressClient.GetString() << endl;
            // Read: 
            nSize = objSocketClient.Read(arrBuf, sizeof(arrBuf));
            nSize = objSocketClient.Write(arrBuf, nSize);
            cout << "Sent back." << endl;
            // 
            objSocketClient.Close();
            objSocketClient.ShutDown();
        }
*/
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
