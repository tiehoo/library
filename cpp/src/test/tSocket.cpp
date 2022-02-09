#include <stdio.h>
#include "CException.h"
#include "CSocket.h"
#include "CSocketStream.h"

// 
int main()
{
    try
    {
        // 
        CSocket objSocket(AF_INET, SOCK_STREAM, true);
        objSocket.SetId(191);
        objSocket.SetAddressString("127.0.0.1:1935");
        printf("ID: %ld, Address: %s\n", objSocket.GetId(), objSocket.GetAddressString().c_str());
        printf("Domain: %d, Type: %d.\n", objSocket.GetDomain(), objSocket.GetType());
        // bOpenSocket, AutoClose
        CSocketStream objSocketStream(AF_INET, true);
        printf("Socket: %d\n", objSocketStream.GetDescriptor());
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
