#include <stdio.h>
#include "CException.h"
#include "CSocketAddressInet.h"
#include "CSocketStream.h"

// 
int main(int argc, char* argv[]) 
{
    // 
    try
    {
    char arrBuf[1024];
    int nSize;
        // 
        CSocketStream       objSocket(AF_INET, true), objSocketClient(AF_INET, false);
        CSocketAddressInet  objAddressServer, objAddressClient;
        // Bind server address: 
        if(argc > 1) 
        {
            objAddressServer.SetIp(argv[1]);
        }
        objAddressServer.SetPort(5010);
        objSocket.Bind(objAddressServer);
        // Listen: 
        objSocket.Listen();
        printf("Listening on: %s.\n", objAddressServer.GetString().c_str());
        
        // Receive data: 
        while(1) 
        {
            objSocket.Accept(objSocketClient, objAddressClient);
            printf("Connected from: %s.\n", objAddressClient.GetString().c_str());
            // Read: 
            nSize = objSocketClient.Read(arrBuf, sizeof(arrBuf));
            printf("Received: [%s]\n", arrBuf);
            nSize = objSocketClient.Write(arrBuf, nSize);
            printf("Sent back.\n\n");
            // 
            objSocketClient.ShutDown();
            objSocketClient.Close();
        }
        objSocket.Close();
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
