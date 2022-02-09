// 
#include <stdio.h>
#include <stdlib.h>
#include "CException.h"
#include "CSocketAddressInet.h"
#include "CSocketStream.h"
#include "CThreadWorkerTcp.h"

int main() 
{
    // 
    try 
    {
        // 
        CSocketStream       objSocket(AF_INET, true);
        CSocketAddressInet  objAddressServer;
        // Bind server address: 
        objAddressServer.SetPort(1234);
        objSocket.Bind(objAddressServer);
        // Listen: 
        objSocket.Listen();
        printf("Listening on: %s.\n", objAddressServer.GetString().c_str());
        // Mutex for accept: 
        CMutex objSocketListenMutex;
        
        // Workers: 
        CThreadWorkerTcp arrWorker[2];

        // 
        for(int i = 0; i < 2; ++i) 
        {
            arrWorker[i].SetSocketListen(&objSocket, &objSocketListenMutex);
            arrWorker[i].Run();
            printf("Created: %lu\n", arrWorker[i].GetId());
        }

        // 
        for(int i = 0; i < 2; ++i) 
        {
            arrWorker[i].Join();
            printf("Joined one.\n");
        }
        
        // 
        objSocket.ShutDown();
        objSocket.Close();
        
        // 
        exit(0);
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
