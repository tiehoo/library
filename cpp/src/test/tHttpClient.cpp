// 
#include <string>
#include <iostream>
// 
#include "CSocketStream.h"
#include "CSocketAddressInet.h"
#include "CException.h"

int main(int argc, char* argv[])
{
    // 
    try
    {
    char cBuf[1024*4];
    int nSize;
        // 
        if(argc < 4) 
        {
            printf("Usage: ClientIP ServerIP URL .\n");
            return(-1);
        }
        // 
        CSocketStream       objSocket;
        CSocketAddressInet  objAddressClient;
        CSocketAddressInet  objAddressServer;
        
        // Set server address: 
        objAddressClient.SetIp(argv[1]);
        objAddressServer.SetIp(argv[2]);
        objAddressServer.SetPort(80);

        // Connect: 
        objSocket.Connect(objAddressServer);
        printf("Connected to: %s\n", objAddressServer.GetString().c_str());
        // Sent Request: 
        char sRequest[512];
        sprintf(sRequest, "GET /%s HTTP/1.0\n\n", argv[3]);
        objSocket.Write(sRequest, strlen(sRequest));
        nSize = objSocket.Read(cBuf, sizeof(cBuf));
        cBuf[nSize] = '\0';
        printf("Received: [%s]\n", cBuf);
        // 
        objSocket.ShutDown();
        objSocket.Close();
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
