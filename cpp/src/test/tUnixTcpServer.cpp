#include <stdio.h>
#include "CException.h"
#include "CSocketStream.h"

// 
int main()
{
    // 
    try
    {
    char arrBuf[1024];
    int nSize;
        // 
        CSocketStream       objSocket(AF_LOCAL, true), objSocketClient(AF_LOCAL, false);
        CSocketAddressUnix  objAddressServer, objAddressClient;
        // 
        std::string sPath = "/tmp/socket_unix_domain_stream";
        objAddressServer.SetPath(sPath);
        objSocket.Bind(objAddressServer);
        objSocket.Listen();

        printf("Listening on device: %s.\n", objAddressServer.GetString().c_str());
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
