#include <stdio.h>
#include "CException.h"
#include "CSipStartLine.h"

int main()
{
    try
    {
        char szBuffer[512];
        bzero(szBuffer, sizeof(szBuffer));
        /*
        strcpy(szBuffer, "INVITE sip:14168780263@vbuzzer.com SIP/2.0\r\nVia: SIP/2.0/UDP 192.168.1.102:5060;branch=z9hG4bK-ca5addb1\r\nFrom: Mini Dawar <sip:v9059440262@vbuzzer.com>;tag=1ffe87b1cb7ddc73o0\r\nTo: <sip:14168780263@vbuzzer.com>\r\nCall-ID: b7897f09-dc138cfb@192.168.1.102\r\nCSeq: 101 INVITE\r\nMax-Forwards: 70\r\nContact: Mini Dawar <sip:v9059440262@192.168.1.102:5060>\r\nExpires: 240\r\nUser-Agent: Linksys/PAP2T-3.1.15(LS)\r\nContent-Length: 448\r\nAllow: ACK, BYE, CANCEL, INFO, INVITE, NOTIFY, OPTIONS, REFER\r\nSupported: x-sipura\r\nContent-Type: application/sdp\r\n\r\nv=0\r\no=- 18445809 18445809 IN IP4 192.168.1.102\r\ns=-\r\nc=IN IP4 192.168.1.102\r\nt=0 0\r\nm=audio 16420 RTP/AVP 0 2 4 8 18 96 97 98 100 101\r\na=rtpmap:0 PCMU/8000\r\na=rtpmap:2 G726-32/8000\r\na=rtpmap:4 G723/8000\r\na=rtpmap:8 PCMA/8000\r\na=rtpmap:18 G729a/8000\r\na=rtpmap:96 G726-40/8000\r\na=rtpmap:97 G726-24/8000\r\na=rtpmap:98 G726-16/8000\r\na=rtpmap:100 NSE/8000\r\na=fmtp:100 192-193\r\na=rtpmap:101 telephone-event/8000\r\na=fmtp:101 0-15\r\na=ptime:30\r\na=sendrecv\r\n");
        */
        CSipStartLine objSipStartLine(512);
        // 
        strcpy(szBuffer, "SIP/2.0  100  Stop retransmission  \r\n");
        objSipStartLine.Clear();
        objSipStartLine.SetBuffer(szBuffer);
        objSipStartLine.Parse();
        objSipStartLine.Print();
        printf("\n");
        objSipStartLine.PrintStructure();
        // 
        strcpy(szBuffer, "SIP/2.0 100   ");
        objSipStartLine.Clear();
        objSipStartLine.SetBuffer(szBuffer);
        objSipStartLine.Parse();
        objSipStartLine.PrintStructure();
        // 
        strcpy(szBuffer, "INVITE   sip:14168780263@vbuzzer.com   SIP/2.0  \r\n");
        objSipStartLine.Clear();
        objSipStartLine.SetBuffer(szBuffer);
        objSipStartLine.Parse();
        objSipStartLine.Print();
        printf("\n");
        objSipStartLine.PrintStructure();
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
