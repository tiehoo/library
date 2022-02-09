// 
#include <stdio.h>
#include <stdlib.h>
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
    char arrBuf[1024*3];
    int nSize;
        // 
        if(argc < 4) 
        {
            printf("Usage: tTcpClient IP port ClientIP.\n");
            return(-1);
        }
        // 
        CSocketStream       objSocket;
        CSocketAddressInet  objAddressClient;
        CSocketAddressInet  objAddressServer;
        
        // Set server address: 
        objAddressServer.SetIp(argv[1]);
        objAddressServer.SetPort(atoi(argv[2]));

        // Connect: 
        objSocket.Connect(objAddressServer);
        printf("Connected to: %s\n", objAddressServer.GetString().c_str());
        // Get user input: 
        std::string sInput;
        std::cout << "Please enter word to send to the server, Q to quit." << std::endl;
        while(std::cin >> sInput) 
        {
            std::cout << sInput << std::endl;
            
            // Send to server: 
            objSocket.Write(sInput.c_str(), sInput.length());
            printf("\tSent: [%s]\n", sInput.c_str());
            // 
            if("Q" == sInput) 
            {
                break;
            }else 
            {
                // 
                nSize = objSocket.Read(arrBuf, sizeof(arrBuf));
                arrBuf[nSize] = '\0';
                printf("\tReceived: [%s]\n\n", arrBuf);
            }
        }
        // 
        objSocket.ShutDown();
        objSocket.Close();
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
