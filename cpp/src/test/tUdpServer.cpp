#include <stdio.h>
#include "CException.h"
#include "CSocketDatagram.h"

// 
int main()
{
    // 
    try
    {
    char arrBuf[1024];
    int nSize;
        // 
        CSocketAddressInet  objAddressServer, objAddressClient;
        CSocketDatagram     objSocket(AF_INET, true), objSocketPeer(AF_INET, true);
        
        // Bind server port: 
        objAddressServer.SetPort(5089);
        objSocket.Bind(objAddressServer);
        // 
        
        printf("Listening on address: %s\n", objAddressServer.GetString().c_str());
        // Receive data: 
        while(1)
        {
            nSize = objSocket.RecvFrom(arrBuf, sizeof(arrBuf), objAddressClient);
            arrBuf[nSize] = '\0';
            printf("Received from address: %s\n[%s]\n", objAddressClient.GetString().c_str(), arrBuf);
            objSocket.SendTo(arrBuf, nSize, objAddressClient);
            printf("Sent back.\n\n");
        }
        objSocket.Close();
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
