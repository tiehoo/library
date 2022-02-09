/* =========================================================================== /
* @category     RADIUS
* @package      Buffer
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CRADIUSCLIENT_H
#define _CRADIUSCLIENT_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdint.h>         // Network byte types
#include <string>
#include <utility>
#include <vector>

// 
#include "CTimeInterval.h"
#include "CSocketDatagram.h"
#include "CSocketAddressInet.h"
#include "CLog.h"
#include "CRadiusPacket.h"

//------------------------------------------------------------------------------
// CRadiusClient 1.0
//------------------------------------------------------------------------------
// Constants: 
// 
const int   RAD_AUTHENTICATION_PORT                 = 1812;     // Default RADIUS authentication port.
const int   RAD_ACCOUNTING_PORT                     = 1813;     // Default RADIUS accounting port.
// 
const int   RAD_CLIENT_TIMEOUT                      = 500*1;    // 1 second. 
const int   RAD_CLIENT_TRANSMIT_COUNT               = 2;        // Retransmit x times totally. 
const float RAD_CLIENT_RETRANSMIT_TIMEOUT_INC       = 1.00;     // Incease x% time out length every retransmit. 
// 
const int   RAD_CLIENT_TRANSMIT_PACKET_COPY_COUNT   = 1;        // Send x duplicated packets in one transmit at same time. 

// 
const int   RAD_MAX_SERVER_NUMBER                   = 10;

// 
const char  RAD_SERVER_TYPE_AUTH                    = 'U';
const char  RAD_SERVER_TYPE_ACCT                    = 'C';

// Server address list: 
typedef std::pair<std::string, int>         tServerAddress;
typedef std::vector<tServerAddress>         tServerList;

//------------------------------------------------------------------------------
/*
// Constants: 
// Error: 3xxxx;
// Warnning: 1xxxx;

// RADIUS client: 
//  Warnning: 
//  Get duplicated packet: 
const int   WAR_CODE_RAD_DUPLICATE_PACKET       = 11001;
const char  WAR_MSG_RAD_DUPLICATE_PACKET[]      = "# Radius client got an duplicated/old packet, this means radius server is responding slowly or network condition between client and server is poor.";
//  Get invalid response: Check sum not right.
const int   WAR_CODE_RAD_INVALID_SUM            = 11011;
const char  WAR_MSG_RAD_MSG_INVALID_SUM[]       = "# Get an invalid response, check sum verification failed. Discarded silently.";
//  Get invalid response: Code is wrong.
const int   WAR_CODE_RAD_INVALID_CODE           = 11012;
const char  WAR_MSG_RAD_MSG_INVALID_CODE[]      = "# Get an invalid response, code is wrong. Discarded silently.";

*/

//------------------------------------------------------------------------------
// 
class CRadiusClient 
{
private: 
    // Log: 
    CLog&               m_objLog;
    // Server list: 
    tServerList         m_vecServerAuth;                // Server list. 
    int                 m_nServerIndexAuth;             // Current server index. 
    tServerList         m_vecServerAcct;                // Server list. 
    int                 m_nServerIndexAcct;             // Current server index. 
    // Identifier: 
    uint8_t             m_nIdentifier;                  // Current Identifier, it will increase sequentially.
    // 
    std::string         m_sSecret;                      // Secret shared between server and client. 
    int                 m_nTransmitTimeOut;             // How many times should it try totally. 
    
    // Time out parameters: 
    CTimeInterval       m_objTimeOut;                   // First transmit time out. 
    float               m_fRetransmitTimeOutIncrease;   // How much percentage of time out shoud increase when retrying. 
    int                 m_nTransmitCount;               // Transmit x time before return failure. 
    int                 m_nTransmitPacketCopyCount;         // Transmit x redunant packets in same transmit. 
/*
    // Socket address: 
    CSocketDatagram     m_objSocket;                    // Client socket.
    CSocketAddressInet  m_objAddressServer;             // Address of server
    CSocketAddressInet  m_objAddressParty;              // Address of other party
    // RADIUS packets: 
    uint8_t             m_nLastIdentifier;              // Current Identifier, it will increase sequentially.
    CRadiusPacket       m_objPacketSend;                // Radius packet used for sending.
    CRadiusPacket       m_objPacketReceive;             // Radius packet used for receiving.
*/
// 
/*
    // Return next identifier in sequence.
    uint8_t         GenerateIdentifier();
    // 
    void            SetServerAddress(std::string sIp, int nPort);
    // Will allocate an address from list randomly: 
    // 
*/
    // 
    uint8_t         GetNextIdentifier();
    void            AssignServerAddress(char cServerType, CSocketAddressInet& objAddressServer);
    bool            SendRequest(char cServerType, const CRadiusPacket& objPacketSend, CRadiusPacket& objPacketReceived);
    // 
public: 
// 
    // lTimeOut: is in milliseconds.
    CRadiusClient(CLog& objLog, const std::string sSecret = "", long lTimeOutMillisecond = RAD_CLIENT_TIMEOUT, int nTransmitCount = RAD_CLIENT_TRANSMIT_COUNT, 
        float fRetransmitTimeOutIncrease = RAD_CLIENT_RETRANSMIT_TIMEOUT_INC, int nTransmitPacketCopyCount = RAD_CLIENT_TRANSMIT_PACKET_COPY_COUNT);
    ~CRadiusClient();
// 
    bool            IsSecretSet();
    void            SetSecret(const std::string sSecret);
    void            SetTimeOut(long lTimeOutMillisecond);
    void            SetTransmitCount(int nTransmitCount);
    void            SetRetransmitTimeOutIncrease(float fPercentage);
    void            SetTransmitPacketCount(int nTransmitPacketCopyCount);
// 
    void            AddServerAddress(const char cServerType, const std::string sIp, int nPort = RAD_AUTHENTICATION_PORT);
// 
    bool            AccessRequest(tRadiusAccessRecord& stAccessRecord);
    // Inbound IP is used to settle with DID provider; Outbound IP is used to settle with long distance provider. 
    bool            AccountingStart(const tRadiusAccountingRecord stAccountingRecord);
    bool            AccountingStop(const tRadiusAccountingRecord stAccountingRecord);
};

/* 
// Accounting: 
1. Outgoing: stephen@vbuzzer.com -> 17187773456.
(INVITE+200 OK): 
O: (B) s1, stephen@vbuzzer.com, 17187773456@vbuzzer.com, ip1, ip2, 209.47.41.24, mci
(BYE1): 
O: (E) s1
(BYE2): 
O: (E) s1

2. Incoming: 14165551234 -> stephen@vbuzzer.com
I: (B) s1, 14165551234@vbuzzer.com, stephen@vbuzzer.com, ip1, ip2, telus, 209.47.41.24

3. Forward: Outgoing + Incoming: 14165551234 -> stephen@vbuzzer.com -> 14166661234
I: (B) s1, 14165551234@vbuzzer.com, stephen@vbuzzer.com, ip1, ip2, telus, 209.47.41.24
O: (B) s1, stephen@vbuzzer.com, 14166661234@vbuzzer.com, ip1, ip2, 209.47.41.24, telus
Note: Above 2 calls has the same acct_session_id(s1), then I have to identify a call by: acct_session_id, caller, callee.
*/

#endif
