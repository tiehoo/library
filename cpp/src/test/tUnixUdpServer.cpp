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
        CSocketDatagram     objSocket(AF_LOCAL, true);
        CSocketAddressUnix  objAddressServer, objAddressClient;
        // 
        objAddressServer.SetPath("/tmp/socket_unix_domain_datagram");
        objSocket.Bind(objAddressServer);

        printf("Listening on device: %s\n", objAddressServer.GetString().c_str());
        // Receive data: 
        while(1)
        {
            nSize = objSocket.RecvFrom(arrBuf, sizeof(arrBuf), objAddressClient);
            arrBuf[nSize] = '\0';
            printf("Received from: %s\n[%s]\n", objAddressClient.GetString().c_str(), arrBuf);
            objSocket.SendTo(arrBuf, nSize, objAddressClient);
            printf("Sent back.\n\n");
        }
        // 
        objSocket.Close();
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
