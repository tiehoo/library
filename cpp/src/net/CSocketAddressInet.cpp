/* =========================================================================== /
* @category     SocketAddress
* @package      
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: mysql.class.php,v 1.121 2006/08/29 21:25:17 artur Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CException.h"
#include "CRegularExpression.h"
#include "CSocketAddressInet.h"

// gethostbyname2_r()
#include <netdb.h>

// inet_pton()
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>

/* --------------------------------------------------------------------------- /
//CCSocketAddressInet 1.0
/ --------------------------------------------------------------------------- */
void CSocketAddressInet::Clear()
{
    // Clear address structure: 
    bzero(&m_stAddress, sizeof(m_stAddress));
    // Set family: 
    m_stAddress.sin_family = AF_INET;
}

// --------------------------------------------------------------------------- /
CSocketAddressInet::CSocketAddressInet()
{
    // 
    m_pstAddress    = (sockaddr*)&m_stAddress;
    // 
    Clear();
}

// 
CSocketAddressInet::CSocketAddressInet(const CSocketAddressInet& objAddress)
{
    try
    {
        /* DO NOT call this function! Will call CSocketAddress() and cause the reset of pointer: m_pstAddress.
        CSocketAddressInet();
        */
        // 
        m_pstAddress    = (sockaddr*)&m_stAddress;
        Clear();
        // 
        operator = (objAddress);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::CSocketAddressInet(const CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSocketAddressInet::~CSocketAddressInet()
{
}

// --------------------------------------------------------------------------- /
bool CSocketAddressInet::IsFormatValid(std::string sIp)
{
    // 
    try
    {
        // 
        CRegularExpression objRegularExpression;
        return(objRegularExpression.IsMatch("^[1-9][0-9]{0,2}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}", sIp));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::IsFormatValid(std::string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
const CSocketAddressInet& CSocketAddressInet::operator = (const CSocketAddressInet& objAddress)
{
    try
    {
        // 
        memcpy(&m_stAddress, &objAddress.m_stAddress, sizeof(m_stAddress));
        return(*this);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::operator = (const CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// Note: pResultBuffer pints to memory allocated out side for the result. ppHostEntry is NULL if can not find.
bool CSocketAddressInet::GetHostByName(const std::string sHost, struct hostent *pResultBuffer, struct hostent **ppHostEntry)
{
    // 
    if(NULL == pResultBuffer)
    {
        throw CException(-1, "%s(%ld): CSocketAddressInet::GetHostByName(string, struct hostent *, struct hostent **): NULL pointer.", __FILE__, __LINE__);
    }
    
    // Auxiliary data buffer: 
    char    szBuffer[512];
    // Error Number: 
    int     nErrorNo = -1;
    
    // Auxiliary data is stored in the szBuffer, if the buffer is too small, these functions will return ERANGE.
    // while(gethostbyname_r() == ERANGE){try to allocate more buffer in szBuffer.}
    if(0 != gethostbyname2_r(sHost.c_str(), AF_INET, pResultBuffer, szBuffer, sizeof(szBuffer),
        ppHostEntry, &nErrorNo))
    {
        // Error: 
        switch(nErrorNo)
        {
        case HOST_NOT_FOUND:    // No such host is known in the database.
        case NO_ADDRESS:        // The host database contains an entry for the name, but it doesn't have an associated Internet address.
            return(false);
        case TRY_AGAIN:         // This condition happens when the name server could not be contacted. If you try again later, you may succeed then.
            return(false);
        case NO_RECOVERY:       // A non-recoverable error occurred.
            throw CException(-1, "%s(%ld): CSocketAddressInet::GetHostByName(): gethostbyname_r() failed permanently.", __FILE__, __LINE__);
        default: 
            throw CException(-1, "%s(%ld): CSocketAddressInet::GetHostByName(): gethostbyname2_r() unknown error number: %d.", __FILE__, __LINE__, nErrorNo);
        }
        // 
        return(false);
    }else
    {    
        // Check result again: 
        if(NULL == *ppHostEntry)
        {
            // gethostbyname() succeeded, but returned NULL pointer: Not found.
            return(false);
        }
        // Found result: 
        return(true);
    }
    // In case no return issued: 
    throw CException(-1, "%s(%ld): CSocketAddressInet::GetHostByName(): Internal error, no return value here.", __FILE__, __LINE__);
}

// --------------------------------------------------------------------------- /
void CSocketAddressInet::SetIp(const std::string sHostOrIp, bool bUseDns)
{
    try
    {
        // 
        struct hostent stHostEntry, *pHostEntry = NULL;
        // 
        if(bUseDns && GetHostByName(sHostOrIp, &stHostEntry, &pHostEntry))
        {
        // Try host name first: 
            // hstrerror(h_errno) to get error.
            if(pHostEntry->h_length > sizeof(m_stAddress.sin_addr))
            {
                throw CException(-1, "%s(%ld): CSocketAddressInet::SetIp(): Overflowed, host address too long: %d, no more than: %d.", __FILE__, __LINE__, 
                    pHostEntry->h_length, sizeof(m_stAddress.sin_addr));
            }
            // 
            memcpy(&m_stAddress.sin_addr, pHostEntry->h_addr, pHostEntry->h_length);
        }else
        {
        // Try IP: 
            if(!IsFormatValid(sHostOrIp))
            {
                if(bUseDns)
                {
                    throw CException(-1, "%s(%ld): CSocketAddressInet::SetIp(): Can not resolve domain: [%s].", __FILE__, __LINE__, sHostOrIp.c_str());
                }else
                {
                    throw CException(-1, "%s(%ld): CSocketAddressInet::SetIp(): Not a valid IP v4 format: [%s]. If you are using domain name, call SetIp(sHostOrIp, true) instead.", __FILE__, __LINE__, 
                        sHostOrIp.c_str());
                }
            }
            if(inet_pton(AF_INET, sHostOrIp.c_str(), &m_stAddress.sin_addr) <= 0)
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CSocket::SetIp(): inet_pton() failed, address: [%s], [%d]:[%s]", __FILE__, __LINE__, 
                    sHostOrIp.c_str(), errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::SetIp(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

std::string CSocketAddressInet::GetIp() const
{
    char szBuf[64];
	if(inet_ntop(AF_INET, &m_stAddress.sin_addr, szBuf, sizeof(szBuf)) == NULL)
	{
	    throw CException(-1, "%s(%ld): CSocketAddressInet::GetIp(): inet_ntop() failed.", __FILE__, __LINE__);
	}else return(std::string(szBuf));
}

// --------------------------------------------------------------------------- /
void CSocketAddressInet::SetPort(int nPort)
{
    if(nPort <= 0 || nPort > 65535)
    {
        throw CException(-1, "%s(%ld): CSocketAddressInet::SetPort(int): Invalid port: %d.", __FILE__, __LINE__, nPort);
    }
    m_stAddress.sin_port = htons((uint16_t) nPort);
}

// 
void CSocketAddressInet::SetPort(const std::string sServiceOrPort, const char* szProtocol)
{
    try
    {
        struct servent *pServiceEnt;
        int nPort;
        //Check whether I can find it from /etc/services, if szProtocol is NULL will match any protocol.
        if((pServiceEnt = getservbyname(sServiceOrPort.c_str(), szProtocol)) != NULL)
        {
        // Found port in the /etc/services: 
            // The port here is in network byte order: 
            m_stAddress.sin_port = pServiceEnt->s_port;
        }else
        {
            if((nPort = strtol(sServiceOrPort.c_str(), NULL, 10)) == 0)
            {
                throw CException(-1, "%s(%ld): CSocketAddressInet::SetPort(string, const char*): Invalid port:[%s].", __FILE__, __LINE__, 
                    sServiceOrPort.c_str());
            }
            // The port here is in host byte order: 
            SetPort(nPort);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::SetPort(string, const char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
int CSocketAddressInet::GetPort() const
{
	if(ntohs(m_stAddress.sin_port) > 0) return(ntohs(m_stAddress.sin_port));
	else throw CException(-1, "%s(%ld): CSocketAddressInet::GetPort(): No port has been set before.", __FILE__, __LINE__);
}

// --------------------------------------------------------------------------- /
std::string CSocketAddressInet::GetString() const
{
    try
    {
        char szBuf[64];
        // IP:Port
        snprintf(szBuf, sizeof(szBuf), "%s:%d", GetIp().c_str(), GetPort());
        return(std::string(szBuf));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressInet::GetString(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
