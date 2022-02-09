#include <stdio.h>
#include "CException.h"
#include "CSocketDatagram.h"

int main()
{
    // 
    try
    {
    char arrBuf[1024], szTempFile[64], *pTempFile;
    int nSize;
        // 
        CSocketDatagram     objSocket(AF_LOCAL, true);
        CSocketAddressUnix  objAddressServer, objAddressClient;
        // 
        objAddressServer.SetPath("/tmp/socket_unix_domain_datagram");
        // Bind client address: 
        if(NULL == (pTempFile = tmpnam(szTempFile)))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::SetPath(string): Failed to make an temporary file.", __FILE__, __LINE__);
        }
        objAddressClient.SetPath(pTempFile);
        objSocket.Bind(objAddressClient);           // This is the diffrence from TCP!
        
        // 
        strcpy(arrBuf, "Hi, this is a test of UDP");
        // Send to server: 
        objSocket.SendTo(arrBuf, strlen(arrBuf), objAddressServer);
        printf("Sent to: %s\n[%s]\n", objAddressServer.GetString().c_str(), arrBuf);
        nSize = objSocket.RecvFrom(arrBuf, sizeof(arrBuf), objAddressClient);
        arrBuf[nSize] = '\0';
        printf("Received from: %s\n[%s]\n\n", objAddressClient.GetString().c_str(), arrBuf);
        // 
        objSocket.Close();
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
