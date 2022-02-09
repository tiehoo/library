#include <stdio.h>
#include "CException.h"
#include "CSipMessage.h"

int main()
{
    try
    {
        char szBuffer[4096];
        // Simple structure: 
        strcpy(szBuffer, "AB\r\nCD\r\n\r\nEF");
        // No startline: 
        strcpy(szBuffer, "AB");
        // No Header: 
        strcpy(szBuffer, "AB\r\n");
        // Normal: 
        strcpy(szBuffer, "INVITE sip:14168780263@vbuzzer.com SIP/2.0\r\nVia: SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\nFrom: Mini Dawar <sip:v9059440262@vbuzzer.com>;tag=1ffe87b1cb7ddc73o0\r\nTo: <sip:14168780263@vbuzzer.com>\r\nCall-ID: b7897f09-dc138cfb@192.168.1.102\r\nCSeq: 101 INVITE\r\nMax-Forwards: 70\r\nContact: Mini Dawar <sip:v9059440262@192.168.1.102:5060>\r\nExpires: 240\r\nUser-Agent: Linksys/PAP2T-3.1.15(LS)\r\nContent-Length: 448\r\nAllow: ACK, BYE, CANCEL, INFO, INVITE, NOTIFY, OPTIONS, REFER\r\nSupported: x-sipura\r\nContent-Type: application/sdp\r\n\r\nv=0\r\no=- 18445809 18445809 IN IP4 192.168.1.102\r\ns=-\r\nc=IN IP4 192.168.1.102\r\nt=0 0\r\nm=audio 16420 RTP/AVP 0 2 4 8 18 96 97 98 100 101\r\na=rtpmap:0 PCMU/8000\r\na=rtpmap:2 G726-32/8000\r\na=rtpmap:4 G723/8000\r\na=rtpmap:8 PCMA/8000\r\na=rtpmap:18 G729a/8000\r\na=rtpmap:96 G726-40/8000\r\na=rtpmap:97 G726-24/8000\r\na=rtpmap:98 G726-16/8000\r\na=rtpmap:100 NSE/8000\r\na=fmtp:100 192-193\r\na=rtpmap:101 telephone-event/8000\r\na=fmtp:101 0-15\r\na=ptime:30\r\na=sendrecv\r\n");
        // Multiline header: From
        strcpy(szBuffer, "INVITE sip:14168780263@vbuzzer.com SIP/2.0\r\nVia: SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\nFrom: 1 Mini Dawar <sip:v9059440262@vbuzzer.com>;tag=1ffe87b1cb7ddc73o0\r\nFrom: 2 Mini Dawar <sip:v9059440262@vbuzzer.com>;tag=1ffe87b1cb7ddc73o0\r\nTo: <sip:14168780263@vbuzzer.com>\r\nCall-ID: b7897f09-dc138cfb@192.168.1.102\r\nCSeq: 101 INVITE\r\nMax-Forwards: 70\r\nContact: Mini Dawar <sip:v9059440262@192.168.1.102:5060>\r\nExpires: 240\r\nUser-Agent: Linksys/PAP2T-3.1.15(LS)\r\nContent-Length: 448\r\nAllow: ACK, BYE, CANCEL, INFO, INVITE, NOTIFY, OPTIONS, REFER\r\nSupported: x-sipura\r\nContent-Type: application/sdp\r\n\r\nv=0\r\no=- 18445809 18445809 IN IP4 192.168.1.102\r\ns=-\r\nc=IN IP4 192.168.1.102\r\nt=0 0\r\nm=audio 16420 RTP/AVP 0 2 4 8 18 96 97 98 100 101\r\na=rtpmap:0 PCMU/8000\r\na=rtpmap:2 G726-32/8000\r\na=rtpmap:4 G723/8000\r\na=rtpmap:8 PCMA/8000\r\na=rtpmap:18 G729a/8000\r\na=rtpmap:96 G726-40/8000\r\na=rtpmap:97 G726-24/8000\r\na=rtpmap:98 G726-16/8000\r\na=rtpmap:100 NSE/8000\r\na=fmtp:100 192-193\r\na=rtpmap:101 telephone-event/8000\r\na=fmtp:101 0-15\r\na=ptime:30\r\na=sendrecv\r\n");
        // 
        CSipMessage objSipMessage(4096);
        
        objSipMessage.SetBuffer(szBuffer);
        //
        objSipMessage.Print();
        printf("\n\n");
        //
        if(objSipMessage.Parse())
        {
            // 
            objSipMessage.PrintStructure();
        }else 
        {
            // 
            printf("%s.\n", objSipMessage.GetError());
        }
        
        // 
        printf("\nGeting Start Line: \n");
        CSipStartLine objSipStartLine;
        if(objSipMessage.GetStartLine(objSipStartLine)) 
        {
            if(objSipStartLine.Parse()) 
            {
                objSipStartLine.PrintStructure();
            }else 
            {
                printf("Invalid startline.\n");
            }
        }else 
        {
            printf("Can not find start line.\n");
        }
        
        // Header: 
        CSipHeader objSipHeader;
        printf("\nGeting Header: CONTENT-LENGTH\n");
        if(objSipMessage.GetHeader("CONTENT-LENGTH", objSipHeader)) 
        {
            if(objSipHeader.Parse()) 
            {
                objSipHeader.PrintStructure();
            }else 
            {
                printf("Invalid header.\n");
            }
        }else 
        {
            printf("Can not find header.\n");
        }
        // Multi header: 
        printf("\nGeting Header: FROM\n");
        if(objSipMessage.GetHeader("FROM", objSipHeader)) 
        {
            if(objSipHeader.Parse()) 
            {
                objSipHeader.PrintStructure();
            }else 
            {
                printf("Invalid header.\n");
            }
        }else 
        {
            printf("Can not find header.\n");
        }
        // Non-exist header: 
        printf("\nGeting Header: FROM1\n");
        if(objSipMessage.GetHeader("FROM1", objSipHeader)) 
        {
            if(objSipHeader.Parse()) 
            {
                objSipHeader.PrintStructure();
            }else 
            {
                printf("Invalid header.\n");
            }
        }else 
        {
            printf("Can not find header.\n");
        }
        
        // Insert: 
        printf("\n\n-------------------------------------------------------------------------------------\n");
        printf("Inserting: \n");
        objSipHeader.Clear();
        //  First line: 
        objSipHeader.SetBuffer("Via1: 1 SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\n");
        objSipHeader.Parse();
        objSipMessage.InsertHeader(objSipHeader, 1);
        objSipMessage.PrintStructure();
        printf("\n");
        //  Middle line: 
        objSipHeader.SetBuffer("Via1: 2 SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\n");
        objSipHeader.Parse();
        objSipMessage.InsertHeader(objSipHeader, 3);
        objSipMessage.PrintStructure();
        printf("\n");
        //  Last line: 
        objSipHeader.SetBuffer("Via1: 3 SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\n");
        objSipHeader.Parse();
        objSipMessage.InsertHeader(objSipHeader, -1);
        objSipMessage.PrintStructure();
        printf("\n");
        /*
        // Invalid line: 
        objSipMessage.InsertHeader(objSipHeader, 0);
        objSipMessage.PrintStructure();
        printf("\n");
        // Invalid line: 
        objSipMessage.InsertHeader(objSipHeader, 19);
        objSipMessage.PrintStructure();
        printf("\n");
        */
        
        // Push header: 
        printf("\n\n-------------------------------------------------------------------------------------\n");
        printf("Pushing: \n");
        objSipHeader.Clear();
        objSipHeader.SetBuffer("FROM: SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\n");
        objSipHeader.Parse();
        // ON TOP: 
        objSipMessage.PushHeader(objSipHeader);
        objSipMessage.PrintStructure();
        printf("\n");
        // Append: 
        objSipHeader.Clear();
        objSipHeader.SetBuffer("FROM1: SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\n");
        objSipHeader.Parse();
        objSipMessage.PushHeader(objSipHeader);
        objSipMessage.PrintStructure();
        printf("\n");
        
        // Pop header: 
        printf("\n\n-------------------------------------------------------------------------------------\n");
        printf("Poping: \n");
        objSipHeader.Clear();
        if(objSipMessage.PopHeader("FROM", objSipHeader))
        {
            objSipHeader.Print();
            printf("\nAfter: ");
            objSipMessage.PrintStructure();
        }else 
        {
            printf("Can not find.\n");
        }
        printf("\n\n");

        objSipHeader.Clear();
        if(objSipMessage.PopHeader("FROM", objSipHeader))
        {
            objSipHeader.Print();
            printf("\nAfter: ");
            objSipMessage.PrintStructure();
        }else 
        {
            printf("Can not find.\n");
        }
        printf("\n\n");

        objSipHeader.Clear();
        if(objSipMessage.PopHeader("FROM", objSipHeader))
        {
            objSipHeader.Print();
            printf("\nAfter: ");
            objSipMessage.PrintStructure();
        }else 
        {
            printf("Can not find.\n");
        }
        printf("\n\n");

        objSipHeader.Clear();
        if(objSipMessage.PopHeader("FROM", objSipHeader))
        {
            objSipHeader.Print();
            printf("\nAfter: ");
            objSipMessage.PrintStructure();
        }else 
        {
            printf("Can not find.\n");
        }
        printf("\n\n");
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
