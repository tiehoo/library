#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "CException.h"
#include "CSocketAddressInet.h"
#include "CSocketAddressUnix.h"

using namespace std;
int main()
{
    try
    {
    // Unix domain socket: 
        CSocketAddressUnix objAddressInetUnix;
        objAddressInetUnix.SetPath("/tmp/unix_socket");
        cout << "Address: " << objAddressInetUnix.GetPath() << endl;

    // INET socket: 
        // Copy and construction: 
        CSocketAddressInet objAddressInet01;
        objAddressInet01.SetIp("192.168.1.1");
        objAddressInet01.SetPort(80);
        printf("Address: %02x, %s.\n\n", objAddressInet01.GetAddressPointer(), objAddressInet01.GetString().c_str());
        // 
        CSocketAddressInet objAddressInet02(objAddressInet01);
        printf("Address: %02x, %s.\n\n", objAddressInet02.GetAddressPointer(), objAddressInet02.GetString().c_str());
exit(1);
        // Others: 
        cout << endl;
        CSocketAddressInet objAddressInet;
        // 
        printf("IsFormatValid: %d, %d, %d.\n", objAddressInet.IsFormatValid("192.168.1.1"), objAddressInet.IsFormatValid("0.168.1.1"), objAddressInet.IsFormatValid("what do you mean?"));
        // 
        objAddressInet.SetIp("192.168.1.1");
        objAddressInet.SetPort(80);
        cout << "Address string: " << objAddressInet.GetString() << endl;
        // 
        CSocketAddressInet objAddressInet1(objAddressInet);
        printf("IP: %s, Port: %d\n", objAddressInet1.GetIp().c_str(), objAddressInet1.GetPort());
        // /etc/hosts: 
        objAddressInet.SetIp("svn", true);
        objAddressInet.SetPort("ssh", "tcp");
        cout << "Address string: " << objAddressInet.GetString() << endl;
        // DNS: 
        objAddressInet.SetIp("www.google.com", true);
        cout << "Address string: " << objAddressInet.GetString() << endl;
        // Can not find: 
        objAddressInet.SetIp("x1x2.google.com", true);
        cout << "Address string: " << objAddressInet.GetString() << endl;
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
