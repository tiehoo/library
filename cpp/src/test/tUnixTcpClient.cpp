#include <stdio.h>
#include "CException.h"
#include "CSocketStream.h"

int main()
{
    // 
    try
    {
    char arrBuf[1024*2];
    int nSize;
        // 
        CSocketStream       objSocket(AF_LOCAL, true);
        CSocketAddressUnix  objAddressServer;
        // 
        objAddressServer.SetPath("/tmp/socket_unix_domain_stream");
        // 
        objSocket.Connect(objAddressServer);
        printf("Connected to: %s\n", objAddressServer.GetString().c_str());

        // 
        strcpy(arrBuf, "Hello Server");
        // Send to server: 
        objSocket.Write(arrBuf, strlen(arrBuf));
        printf("Sent: To: [%s]\n", arrBuf);

        // 
        nSize = objSocket.Read(arrBuf, sizeof(arrBuf));
        arrBuf[nSize] = '\0';
        printf("Received: [%s]\n\n", arrBuf);

    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
