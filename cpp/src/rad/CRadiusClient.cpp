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

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// 
#include "CException.h"
#include "CDateTime.h"
#include "CMd5.h"
#include "CRadiusPacket.h"
#include "CRadiusClient.h"

//------------------------------------------------------------------------------
// CRadiusClient 1.0
//------------------------------------------------------------------------------
void CRadiusClient::SetSecret(std::string sSecret)
{
    try
    {
        m_sSecret = sSecret;
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::SetSecret(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusClient::IsSecretSet()
{
    return(!m_sSecret.empty());
}

void CRadiusClient::SetTimeOut(long lTimeOutMillisecond)
{
    m_objTimeOut.Clear();
    m_objTimeOut.SetMicroSecond(lTimeOutMillisecond*1000);
}

void CRadiusClient::SetTransmitCount(int nTransmitCount) 
{
    if(nTransmitCount < 1 || nTransmitCount > 3) 
    {
        throw CException(-1, "%s(%ld): CRadiusClient::SetTransmitTime(): Unreasonable transmit count: %d, should between 1 and 3.", __FILE__, __LINE__, 
            nTransmitCount);
    }
    m_nTransmitCount = nTransmitCount;
}

void CRadiusClient::SetRetransmitTimeOutIncrease(float fPercentage)
{
    if(fPercentage < 0 || fPercentage > 9)
    {
        throw CException(-1, "%s(%ld): CRadiusClient::SetTransmitTime(): Unreasonable transmit time: %f, should between 0 and 9.", __FILE__, __LINE__, 
            fPercentage);
    }
    m_fRetransmitTimeOutIncrease = fPercentage;
}

void CRadiusClient::SetTransmitPacketCount(int nTransmitPacketCopyCount) 
{
    if(nTransmitPacketCopyCount < 1 || nTransmitPacketCopyCount > 2) 
    {
        throw CException(-1, "%s(%ld): CRadiusClient::SetTransmitTime(): Unreasonable transmit time: %d, should between 1 and 2.", __FILE__, __LINE__, 
            nTransmitPacketCopyCount);
    }
    m_nTransmitPacketCopyCount = nTransmitPacketCopyCount;
}

//------------------------------------------------------------------------------
CRadiusClient::CRadiusClient(CLog& objLog, const std::string sSecret, long lTimeOutMillisecond, int nTransmitCount, 
        float fRetransmitTimeOutIncrease, int nTransmitPacketCopyCount):m_objLog(objLog)
{
    try
    {
        // Set a random identifier: 
        m_nIdentifier = random()%255;
        // 
        if(!sSecret.empty()) 
        {
            SetSecret(sSecret);
        }
        // 
        SetTimeOut(lTimeOutMillisecond);
        SetTransmitCount(nTransmitCount);
        SetRetransmitTimeOutIncrease(fRetransmitTimeOutIncrease);
        SetTransmitPacketCount(nTransmitPacketCopyCount);
        // 
        m_vecServerAuth.clear();
        m_nServerIndexAuth = -1;
        // 
        m_vecServerAcct.clear();
        m_nServerIndexAcct = -1;
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::CRadiusClient(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CRadiusClient::~CRadiusClient()
{
}

//------------------------------------------------------------------------------
uint8_t CRadiusClient::GetNextIdentifier() 
{
    // uint8_t will not increase more than 255.
    m_nIdentifier++;
    return(m_nIdentifier);
}

/*
uint8_t CRadiusClient::GenerateIdentifier()
{
    // uint8_t will not increase more than 255.
    m_nLastIdentifier++;
    return(m_nLastIdentifier);
}
*/

//------------------------------------------------------------------------------
// 
void CRadiusClient::AssignServerAddress(char cServerType, CSocketAddressInet& objAddressServer)
{
    try
    {
        switch(cServerType) 
        {
        case RAD_SERVER_TYPE_AUTH: 
            if(m_vecServerAuth.size() <= 0)
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AssignServerAddress(): No AUTH address available.", __FILE__, __LINE__);
            }
            if(1 == m_vecServerAuth.size() && m_nServerIndexAuth >= 0) 
            {
                // Assigned and only one server: 
                return;
            }
            // 
            if(m_nServerIndexAuth >= m_vecServerAuth.size() - 1 || m_nServerIndexAuth < 0)
            {
                m_nServerIndexAuth = 0;
            }else
            {
                m_nServerIndexAuth++;
            }
            // 
            objAddressServer.SetIp(m_vecServerAuth.at(m_nServerIndexAuth).first, false);
            objAddressServer.SetPort(m_vecServerAuth.at(m_nServerIndexAuth).second);
            break;
        case RAD_SERVER_TYPE_ACCT: 
            if(m_vecServerAcct.size() <= 0)
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AssignServerAddress(): No ACCT address available.", __FILE__, __LINE__);
            }
            if(1 == m_vecServerAcct.size() && m_nServerIndexAcct >= 0)
            {
                // Assigned and only one server: 
                return;
            }
            // 
            if(m_nServerIndexAcct >= m_vecServerAcct.size() - 1 || m_nServerIndexAcct < 0)
            {
                m_nServerIndexAcct = 0;
            }else
            {
                m_nServerIndexAcct++;
            }
            // 
            objAddressServer.SetIp(m_vecServerAcct.at(m_nServerIndexAcct).first, false);
            objAddressServer.SetPort(m_vecServerAcct.at(m_nServerIndexAcct).second);
            break;
        default: 
            throw CException(-1, "%s(%ld): CRadiusClient::AssignServerAddress(): Invalid server type: %c.", 
                __FILE__, __LINE__, cServerType);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AssignServerAuthAddress(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CRadiusClient::AddServerAddress(const char cServerType, const std::string sIp, int nPort) 
{
    try
    {
        switch(cServerType) 
        {
        case RAD_SERVER_TYPE_AUTH: 
            // 
            if(m_vecServerAuth.size() >= RAD_MAX_SERVER_NUMBER) 
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AddServerAddress(): Too many address added, no more than: %d.", __FILE__, __LINE__, 
                    RAD_MAX_SERVER_NUMBER);
            }
            if(nPort <= 0)
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AddServerAddress(): Invalid port: %d.", __FILE__, __LINE__, nPort);
            }
            // 
            m_vecServerAuth.push_back(tServerAddress(sIp, nPort));
            break;
        case RAD_SERVER_TYPE_ACCT: 
            // 
            if(m_vecServerAcct.size() >= RAD_MAX_SERVER_NUMBER) 
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AddServerAddress(): Too many address added, no more than: %d.", __FILE__, __LINE__, 
                    RAD_MAX_SERVER_NUMBER);
            }
            if(nPort <= 0)
            {
                throw CException(-1, "%s(%ld): CRadiusClient::AddServerAddress(): Invalid port: %d.", __FILE__, __LINE__, nPort);
            }
            // 
            m_vecServerAcct.push_back(tServerAddress(sIp, nPort));
            break;
        default: 
            throw CException(-1, "%s(%ld): CRadiusClient::AddServerAddress(): Invalid server type: %c.", 
                __FILE__, __LINE__, cServerType);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AddServerAddress(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//------------------------------------------------------------------------------
bool CRadiusClient::SendRequest(char cServerType, 
    const CRadiusPacket& objPacketSend, CRadiusPacket& objPacketReceived)
{
    try
    {
        // 1. Senity check: 
        if(objPacketSend.GetLength() <= 0)
        {
            throw CException(-1, "%s(%ld): CRadiusClient::SendRequest(): Packet to be sent is empty.", __FILE__, __LINE__);
        }

        // 2. Data structures: 
        // Data buffer: 
        uint8_t arrBuffer[2*1024];

        // Socket descriptor: 
        CSocketDatagram objSocket(AF_INET, SOCK_DGRAM);
        CSocketAddressInet objAddressServer, objAddressPeer;
        // 
        fd_set arrDescriptorSet;
        struct timeval stTimeVal;
        int nMaxFd1, nReadyCnt, nLen;
        
        /*
        FD_ZERO(&arrDescriptorSet);
        FD_SET(m_objSocket.GetDescriptor(), &arrDescriptorSet);
        nMaxFd1 = m_objSocket.GetDescriptor();
        nMaxFd1++;
        */

        // Prepare error message: 
        /*
        nErrorCode = 0;
        sErrorMessage.clear();
        */
        /*
        char szTimeOut[32];
        snprintf(szTimeOut, sizeof(szTimeOut), "(%ld,%ld)", m_objTimeOut.GetSecond(), m_objTimeOut.GetMicroSecond());
        */

/*
        // Save current identifier for further checking when get the response: 
        int nRequestIdentifer = objPacketSend.GetIdentifier();
        // Save request authenticator for validation purpose: 
        unsigned char arrRequestAuthenticator[RAD_AUTHENTICATOR_LEN];
        objPacketSend.GetAuthenticator(arrRequestAuthenticator, sizeof(arrRequestAuthenticator));
*/
        // 3. Process request: 
        // Select time out: 
        CDateTime       objTimeStart, objTimeStop;
        // objTimeOutTransmit: total timeout for current transmit. 
        CTimeInterval   objTimeOut, objTimeOutTransmit;
        // 
        bool bIsResponseValid = false;
        
        // Send and wait: Set to default time out: 
        objTimeOutTransmit = m_objTimeOut;
        // Assign a server address: 
        AssignServerAddress(cServerType, objAddressServer);
        
        // 
        for(int i = 0; i < m_nTransmitCount; ++i) 
        {
            // 3.1. Send packet: 
            m_objLog.WriteDebug("Sending packet(%d)(%d copies) to RADIUS server(%s).", objPacketSend.GetIdentifier(), m_nTransmitPacketCopyCount, 
                objAddressServer.GetString().c_str());
            for(int j = 0; j < m_nTransmitPacketCopyCount; ++j) 
            {
                objSocket.SendTo(objPacketSend.GetPacket(), objPacketSend.GetLength(), objAddressServer);
            }
            
            // 3.2. Wait for response until time out: During the period we may get delayed packet.
            while(1)
            {
                // 
                objTimeOut = objTimeOutTransmit;
                // Set start time: 
                objTimeStart.SetToNow();
                // Wait here: 
                objTimeOut.GetTimeVal(stTimeVal);
                // 
                FD_ZERO(&arrDescriptorSet);
                FD_SET(objSocket.GetDescriptor(), &arrDescriptorSet);
                nMaxFd1 = objSocket.GetDescriptor();
                nMaxFd1++;
                // 
                m_objLog.WriteDebug("\tWaiting for response: Packet: (%d), RADIUS server: (%s), time out: (%ld, %ld).", objPacketSend.GetIdentifier(), objAddressServer.GetString().c_str(), 
                    objTimeOut.GetSecond(), objTimeOut.GetMicroSecond());
                if((nReadyCnt = select(nMaxFd1, &arrDescriptorSet, NULL, NULL, &stTimeVal)) < 0)
                {
                    throw CException(-1, "%s(%ld): CRadiusClient::SendRequest(): select() failed: [%s]", __FILE__, __LINE__, strerror(errno));
                }else if(nReadyCnt == 0) 
                {
                    m_objLog.WriteWarning("\tTransmit to RADIUS server(%s) timed out(%ld,%ld).", objAddressServer.GetString().c_str(), objTimeOut.GetSecond(), objTimeOut.GetMicroSecond());
                    // Switch to another server: 
                    AssignServerAddress(cServerType, objAddressServer);
                    m_objLog.WriteInfo("\tSwitched to RADIUS server(%s) trying to retransmit.", objAddressServer.GetString().c_str());
                    // Try to retransmit: 
                    break;
                }else 
                {
                    // Socket ready: 
                    nLen = objSocket.RecvFrom(arrBuffer, sizeof(arrBuffer), objAddressPeer);
                    m_objLog.WriteDebug("\tGot response from: (%s), size: (%d).", objAddressPeer.GetString().c_str(), nLen);
                    
                    // Response packet: 
                    objPacketReceived.SetPacket(arrBuffer, nLen);
                    bIsResponseValid = objPacketReceived.IsPacketValid(objPacketSend.GetAuthenticator(), RAD_AUTHENTICATOR_LEN);

                    // Check is it the response I want: 
                    if(objPacketSend.GetIdentifier() != objPacketReceived.GetIdentifier() || !bIsResponseValid) 
                    {
                        // It's an old or duplicated packet, discard silently, deduct waitting time, and wait again: 
                        // Get stop time: 
                        objTimeStop.SetToNow();
                        // Calculate new waitting time: 
                        objTimeOut = objTimeOut - (objTimeStop - objTimeStart);
                        // Check time out: 
                        if(objTimeOut.Sign() <= 0) 
                        {
                        // Finally time out: 
                            m_objLog.WriteError("Transmit to (%s) timed out(%ld,%ld)!", objAddressServer.GetString().c_str(), m_objTimeOut.GetSecond(), m_objTimeOut.GetMicroSecond());
                            // Switch address: 
                            AssignServerAddress(cServerType, objAddressServer);
                            // Try to retransmit: 
                            break;
                        }
                        // Check packet: 
                        if(objPacketSend.GetIdentifier() != objPacketReceived.GetIdentifier()) 
                        {
                        // Got a duplicated/old packet: 
                            m_objLog.WriteDebug("\tGot a packet with different id(%d), disgarded!", objPacketReceived.GetIdentifier());
                        }
                        if(!bIsResponseValid) 
                        {
                        // Get an response encrypted with wrong secret, somebody trying to attack us?
                            m_objLog.WriteWarning("Got inavlid packet from: %s!", objAddressPeer.GetString().c_str());
                            // Discard and wait again: Wait for the true one.
                        }
                        // Wrong packet interrupted, time out reduced, wait again: 
                        continue;
                    }
                    // Got my response now: 
                    return(true);
                }
            } // while(1)

            // Need to retransmit: Increase retransmit time out: 
            objTimeOutTransmit = objTimeOutTransmit * (1 + m_fRetransmitTimeOutIncrease);
        } // for(int i = 0; i < m_nTransmitCount; ++i)
        // Time out after retransmit: 
        return(false);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::SendRequest(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusClient::AccessRequest(tRadiusAccessRecord& stAccessRecord) 
{
    try
    {
        if(!IsSecretSet()) 
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): No secret set yet.", __FILE__, __LINE__);
        }

        // 
        CRadiusPacket objPacketSend(m_sSecret), objPacketReceived(m_sSecret);
        
        // Generate authentication packet: Here I assume you have already set the secret.
        objPacketSend.ClearPacket();
        objPacketSend.SetCode(RAD_ACCESS_REQUEST);
        // Get a new identifier: Set identifier before generate random authenticator!
        objPacketSend.SetIdentifier(GetNextIdentifier());
        // Authentication: Set a random authenticator before set password attribute: 
        objPacketSend.GenerateRandomAuthenticator();
        
        // Set all attributes: 
        if(!stAccessRecord.sUserName.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_USERNAME, 
                stAccessRecord.sUserName.length(), (uint8_t*)stAccessRecord.sUserName.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sPassword.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_USERPASSWORD, 
                stAccessRecord.sPassword.length(), (uint8_t*)stAccessRecord.sPassword.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sSipCallId.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPCALLID, 
                stAccessRecord.sSipCallId.length(), (uint8_t*)stAccessRecord.sSipCallId.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sSipFromTag.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPFROMTAG, 
                stAccessRecord.sSipFromTag.length(), (uint8_t*)stAccessRecord.sSipFromTag.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sSipToTag.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPTOTAG, 
                stAccessRecord.sSipToTag.length(), (uint8_t*)stAccessRecord.sSipToTag.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sSipUserAgent.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPUSERAGENT, 
                stAccessRecord.sSipUserAgent.length(), (uint8_t*)stAccessRecord.sSipUserAgent.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sCallingStationId.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONID, 
                stAccessRecord.sCallingStationId.length(), (uint8_t*)stAccessRecord.sCallingStationId.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sCalledStationId.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONID, 
                stAccessRecord.sCalledStationId.length(), (uint8_t*)stAccessRecord.sCalledStationId.c_str(), RAD_ATTR_KIND_STRING);
        }
        if(!stAccessRecord.sSipMediaIp.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPMEDIAIPADDRESS, 
                stAccessRecord.sSipMediaIp.length(), (uint8_t*)stAccessRecord.sSipMediaIp.c_str(), RAD_ATTR_KIND_IP);
        }
        // 
        if(!stAccessRecord.sSipInboundIp.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPINBOUNDIPADDRESS, 
                stAccessRecord.sSipInboundIp.length(), (uint8_t*)stAccessRecord.sSipInboundIp.c_str(), RAD_ATTR_KIND_IP);
        }
        if(stAccessRecord.nSipInboundPort > 0) 
        {
            uint32_t lPort = stAccessRecord.nSipInboundPort;
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPINBOUNDPORT, 
                sizeof(uint32_t), (uint8_t*)(&lPort), RAD_ATTR_KIND_ULONG);
        }
        if(!stAccessRecord.sSipOutboundIp.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPOUTBOUNDIPADDRESS, 
                stAccessRecord.sSipOutboundIp.length(), (uint8_t*)stAccessRecord.sSipOutboundIp.c_str(), RAD_ATTR_KIND_IP);
        }
        if(stAccessRecord.nSipOutboundPort > 0) 
        {
            uint32_t lPort = stAccessRecord.nSipOutboundPort;
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPOUTBOUNDPORT, 
                sizeof(uint32_t), (uint8_t*)(&lPort), RAD_ATTR_KIND_ULONG);
        }
        // 
        if(!stAccessRecord.sNasIp.empty()) 
        {
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_NASIPADDRESS, 
                stAccessRecord.sNasIp.length(), (uint8_t*)stAccessRecord.sNasIp.c_str(), RAD_ATTR_KIND_IP);
        }
        if(stAccessRecord.nNasPort > 0) 
        {
            uint32_t lPort = stAccessRecord.nNasPort;
            objPacketSend.AddAttribute(RAD_ATTR_TYPE_NASPORT, 
                sizeof(uint32_t), (uint8_t*)(&lPort), RAD_ATTR_KIND_ULONG);
        }
        
        // Send request: 
        if(!SendRequest(RAD_SERVER_TYPE_AUTH, objPacketSend, objPacketReceived)) 
        {
            m_objLog.WriteDebug("Timed out waitting for response.");
            return(false);   // Timed out. 
        }

        // 
        if(RAD_ACCESS_ACCEPT == objPacketReceived.GetCode()) 
        {
            // Accepted: 
            if(!objPacketReceived.GetAttributeLong(RAD_ATTR_TYPE_SESSIONTIMEOUT, stAccessRecord.lSessionTimeout)) 
            {
                // Can not find Session-Timeout attribute: 
                throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): Can not find Session-Timeout attribute in response packet.", __FILE__, __LINE__);
            }
            if(!objPacketReceived.GetAttributeString(RAD_ATTR_TYPE_ACCESSTOKEN, stAccessRecord.sAccessToken)) 
            {
                // Can not find Session-Timeout attribute: 
                throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): Can not find Access-Token attribute in response packet.", __FILE__, __LINE__);
            }
            if(!objPacketReceived.GetAttributeLong(RAD_ATTR_TYPE_CHANNELID, stAccessRecord.lChannelId))
            {
                stAccessRecord.lChannelId = 0;
            }
            return(true);
        }else if(RAD_ACCESS_REJECT == objPacketReceived.GetCode()) 
        {
            // Rejected: 
            m_objLog.WriteDebug("Got reject packet.");
            return(false);
        }else 
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): Invalid RADIUS access request result code: %d.", __FILE__, __LINE__, objPacketReceived.GetCode());
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccessRequest(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusClient::AccountingStart(const tRadiusAccountingRecord stAccountingRecord) 
{
    try
    {
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccountingStart(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusClient::AccountingStop(const tRadiusAccountingRecord stAccountingRecord) 
{
    try
    {
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccountingStop(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

/*
// 
bool CRadiusClient::AccessRequest(const char* szUserName, const char* szDomain, const char* szPass, const char* szCallee, long& lSessionTimeOut, 
        int& nErrorCode, std::string& sErrorMessage, const char* szCallee2)
{
    try
    {
        if(NULL == szUserName || NULL == szDomain || NULL == szCallee)
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): NULL pointer.", __FILE__, __LINE__);
        }
        if(!m_objPacketSend.IsSecretSet() || !m_objPacketReceive.IsSecretSet())
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): No secret set yet.", __FILE__, __LINE__);
        }
        // Generate authentication packet: Here I assume you have already set the secret.
        m_objPacketSend.ClearPacket();
        m_objPacketSend.SetCode(RAD_ACCESS_REQUEST);
        // Get a new identifier: Set identifier before generate random authenticator!
        m_objPacketSend.SetIdentifier(GenerateIdentifier());
        // Authentication: Set a random authenticator before set password attribute: 
        m_objPacketSend.SetRandomAuthenticator();
        // Set all attributes: 
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_USERNAME, strlen(szUserName), (uint8_t*)szUserName, RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_USERDOMAIN, strlen(szDomain), (uint8_t*)szDomain, RAD_ATTR_KIND_STRING);
        if(NULL != szPass)
        {
            m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_USERPASSWORD, strlen(szPass), (uint8_t*)szPass, RAD_ATTR_KIND_STRING);
        }
        // Trick the free radius server: RAD_ATTR_TYPE_NASIPADDRESS
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_NASIPADDRESS, strlen("127.0.0.1"), (uint8_t*)"127.0.0.1", RAD_ATTR_KIND_IP);
        // 
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONID, strlen(szCallee), (uint8_t*)szCallee, RAD_ATTR_KIND_STRING);
        // Callback number: 
        if(NULL != szCallee2)
        {
            m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLBACKNUMBER, strlen(szCallee2), (uint8_t*)szCallee2, RAD_ATTR_KIND_STRING);
        }
        
        // Send request and get response: 
        // 
        if(!SendRequest(m_objPacketSend, m_objPacketReceive, nErrorCode, sErrorMessage)) return(false);   // Timed out.

        // 
        if(m_objPacketReceive.GetCode() == RAD_ACCESS_ACCEPT)
        {
            // Accepted: 
            if(!m_objPacketReceive.GetAttributeLong(RAD_ATTR_TYPE_SESSIONTIMEOUT, lSessionTimeOut))
            {
                // Can not find Session-Timeout attribute: 
                throw CException(-1, "%s(%ld): CRadiusClient::AccessRequest(): Can not find Session-Timeout attribute in response packet.", __FILE__, __LINE__);
                //AssignServerAddress();
                //return(false);
            }
            return(true);
        }else if(m_objPacketReceive.GetCode() == RAD_ACCESS_REJECT)
        {
            // Rejected: 
            return(false);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccessRequest(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/

//------------------------------------------------------------------------------
/*
rlm_acct_unique: WARNING: Attribute NAS-Port was not found in request, unique ID MAY be inconsistent
rlm_acct_unique: WARNING: Attribute User-Name was not found in request, unique ID MAY be inconsistent
rlm_acct_unique: Hashing ',Client-IP-Address = 74.13.233.144,NAS-IP-Address = 74.13.233.144,Acct-Session-Id = "0123456789",'
rlm_acct_unique: Acct-Unique-Session-ID = "34f12a612e4e0b8a".
  modcall[preacct]: module "acct_unique" returns ok for request 2
    rlm_realm: Proxy reply, or no User-Name.  Ignoring.
*/
/*
bool CRadiusClient::AccountingStart(const char* szAcctSessionId, const char* szCaller, const char* szCallee, 
    const char* szCallerIp, const char*szCalleeIp, const char* szInboundRouteIp, const char* szOutboundRouteIp, 
    int& nErrorCode, std::string& sErrorMessage)
{
    try
    {
        // Routine check: 
        if(NULL == szAcctSessionId || NULL == szCaller || NULL == szCallee || NULL == szCallerIp || NULL == szCalleeIp || 
            NULL == szInboundRouteIp, NULL == szOutboundRouteIp) 
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccountingStart(): NULL pointer.", __FILE__, __LINE__);
        }
        // 
        if(!m_objPacketSend.IsSecretSet() || !m_objPacketReceive.IsSecretSet())
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccountingStart(): No secret set yet.", __FILE__, __LINE__);
        }
        
        // Generate accounting packet: 
        m_objPacketSend.ClearPacket();
        m_objPacketSend.SetCode(RAD_ACCOUNTING_REQUEST);
        // Get a new identifier: 
        m_objPacketSend.SetIdentifier(GenerateIdentifier());
        // Set 0 authenticator: 
        m_objPacketSend.SetAuthenticator(RAD_ACCT_REQUEST_AUTHENTICATOR);
        // 
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_ACCTSTATUSTYPE, sizeof(RAD_ATTR_VALUE_ACCTSTATUSTYPE_START), (uint8_t*)&RAD_ATTR_VALUE_ACCTSTATUSTYPE_START, RAD_ATTR_KIND_ULONG);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_ACCTSESSIONID, strlen(szAcctSessionId), (uint8_t*)szAcctSessionId, RAD_ATTR_KIND_STRING);
        // 
        std::string sCaller, sCallee;
        sCaller =   "sip:";
        sCaller +=  szCaller;
        if(szCallee[0] >= '2' && szCallee[0] <= '9') sCallee = "sip:011";
        else sCallee = "sip:";
        sCallee += szCallee;
        // 
*
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONID, strlen(szCaller), (uint8_t*)szCaller, RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONID, strlen(szCallee), (uint8_t*)szCallee, RAD_ATTR_KIND_STRING);
*
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONID, sCaller.length(), (uint8_t*)sCaller.c_str(), RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONID, sCallee.length(), (uint8_t*)sCallee.c_str(), RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONIP, strlen(szCallerIp), (uint8_t*)szCallerIp, RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONIP, strlen(szCalleeIp), (uint8_t*)szCalleeIp, RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPINBOUNDIP, strlen(szInboundRouteIp), (uint8_t*)szInboundRouteIp, RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPOUTBOUNDIP, strlen(szOutboundRouteIp), (uint8_t*)szOutboundRouteIp, RAD_ATTR_KIND_IP);
        // Calculate and update authenticator: 
        m_objPacketSend.CalculateAuthenticator();
        
        // Send request and get response: 
        if(!SendRequest(m_objPacketSend, m_objPacketReceive, nErrorCode, sErrorMessage)) return(false);   // Timed out.

        // 
        if(m_objPacketReceive.GetCode() == RAD_ACCOUNTING_RESPONSE) return(true);  // Get an acknowledgement.
        else
        {
            // Got an unknown code.
            char szCode[16];
            snprintf(szCode, sizeof(szCode), "%d", m_objPacketReceive.GetCode());
            // Get an unknown response code: 
            nErrorCode = WAR_CODE_RAD_INVALID_CODE;
            sErrorMessage += m_objErrorMap.GetErrorMessage(WAR_CODE_RAD_INVALID_CODE);
            sErrorMessage += " P1: ";
            sErrorMessage += szCode;
            sErrorMessage += ", P2: " + m_objAddressServer.GetString();
            return(false);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccountingStart(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/

//------------------------------------------------------------------------------
/*
bool CRadiusClient::AccountingStop(const char* szAcctSessionId, int& nErrorCode, std::string& sErrorMessage)
{
    try
    {
        // Routine check: 
        if(NULL == szAcctSessionId) 
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccountingStop(): NULL pointer.", __FILE__, __LINE__);
        }
        // 
        if(!m_objPacketSend.IsSecretSet() || !m_objPacketReceive.IsSecretSet())
        {
            throw CException(-1, "%s(%ld): CRadiusClient::AccountingStop(): No secret set yet.", __FILE__, __LINE__);
        }
        
        // Generate accounting packet: 
        m_objPacketSend.ClearPacket();
        m_objPacketSend.SetCode(RAD_ACCOUNTING_REQUEST);
        // Get a new identifier: 
        m_objPacketSend.SetIdentifier(GenerateIdentifier());
        // Set 0 authenticator: 
        m_objPacketSend.SetAuthenticator(RAD_ACCT_REQUEST_AUTHENTICATOR);
        // 
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_ACCTSTATUSTYPE, sizeof(RAD_ATTR_VALUE_ACCTSTATUSTYPE_STOP), (uint8_t*)&RAD_ATTR_VALUE_ACCTSTATUSTYPE_STOP, RAD_ATTR_KIND_ULONG);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_ACCTSESSIONID, strlen(szAcctSessionId), (uint8_t*)szAcctSessionId, RAD_ATTR_KIND_STRING);
        // Just add redundant fields: 
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONID, strlen("?"), (uint8_t*)"?", RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONID, strlen("?"), (uint8_t*)"?", RAD_ATTR_KIND_STRING);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLINGSTATIONIP, strlen("192.168.0.1"), (uint8_t*)"192.168.0.1", RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_CALLEDSTATIONIP, strlen("192.168.0.1"), (uint8_t*)"192.168.0.1", RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPINBOUNDIP, strlen("192.168.0.1"), (uint8_t*)"192.168.0.1", RAD_ATTR_KIND_IP);
        m_objPacketSend.AddAttribute(RAD_ATTR_TYPE_SIPOUTBOUNDIP, strlen("192.168.0.1"), (uint8_t*)"192.168.0.1", RAD_ATTR_KIND_IP);
        // Calculate and update authenticator: 
        m_objPacketSend.CalculateAuthenticator();
        
        // Send request and get response: 
        if(!SendRequest(m_objPacketSend, m_objPacketReceive, nErrorCode, sErrorMessage)) return(false);   // Timed out.

        // 
        if(m_objPacketReceive.GetCode() == RAD_ACCOUNTING_RESPONSE) return(true);  // Get an acknowledgement.
        else
        {
            // Got an unknown code.
            char szCode[16];
            snprintf(szCode, sizeof(szCode), "%d", m_objPacketReceive.GetCode());
            // Get an unknown response code: 
            nErrorCode = WAR_CODE_RAD_INVALID_CODE;
            sErrorMessage += m_objErrorMap.GetErrorMessage(WAR_CODE_RAD_INVALID_CODE);
            sErrorMessage += " P1: ";
            sErrorMessage += szCode;
            sErrorMessage += ", P2: " + m_objAddressServer.GetString();
            return(false);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusClient::AccountingStop(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/
