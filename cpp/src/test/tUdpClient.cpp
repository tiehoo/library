#include <stdio.h>
#include <stdlib.h>
#include "CException.h"
#include "CSocketDatagram.h"

int main(int argc, char* argv[])
{
    // 
    try
    {
        char arrBuf[1024];
        int nSize;
        // 
        if(argc < 3)
        {
            printf("Usage: tUdpClient IP port.\n");
            return(-1);
        }
        // 
        CSocketAddressInet  objAddressServer, objAddressPeer;
        CSocketDatagram     objSocket(AF_INET, true);
        // Bind server address: 
        objAddressServer.SetIp(argv[1], false);
        objAddressServer.SetPort(atoi(argv[2]));
    
        // 
        strcpy(arrBuf, "Hello Server");
        // Send to server: 
        objSocket.SendTo(arrBuf, strlen(arrBuf), objAddressServer);
        printf("Sent to: %s\n[%s]\n", objAddressServer.GetString().c_str(), arrBuf);
        nSize = objSocket.RecvFrom(arrBuf, sizeof(arrBuf), objAddressPeer);
        arrBuf[nSize] = '\0';
        printf("Received from: %s\n[%s]\n\n", objAddressPeer.GetString().c_str(), arrBuf);
        // 
        objSocket.Close();
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
