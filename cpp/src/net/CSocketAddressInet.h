/* =========================================================================== /
* @category     CSocketAddress
* @package      
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: mysql.class.php,v 1.121 2006/08/29 21:25:17 artur Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CSOCKETADDRESSINET_H
#define _CSOCKETADDRESSINET_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>

// struct sockaddr_in
#include <netinet/in.h>
#include "CSocketAddress.h"

// --------------------------------------------------------------------------- /
// CSocketAddressInet 1.0 (INET4)
// --------------------------------------------------------------------------- /
// Consts: 
// 
class CSocketAddressInet : public CSocketAddress
{
private: 
// 
    struct sockaddr_in      m_stAddress;
// 
    // Initialize all data: bzero()
    void                    Clear();
public: 
    CSocketAddressInet();
    CSocketAddressInet(const CSocketAddressInet& objAddress);
    ~CSocketAddressInet();
// 
    bool            IsFormatValid(std::string sIp);   // [0-9]{2,3}.[0-9]{2,3}.[0-9]{2,3}.[0-9]{2,3}
// 
    const CSocketAddressInet& 
                    operator = (const CSocketAddressInet& objAddress);
// 
    // Changes the hostname of this address to the given one.
    bool            GetHostByName(const std::string sHost, struct hostent *pResultBuffer, struct hostent **ppHostEntry);
// 
    void            SetIp(const std::string sHostOrIp, bool bUseDns = false);
    std::string     GetIp() const;
// 
    // 
    void            SetPort(int nPort);
    // szProtocol can be: "tcp", ..., if szProtocol is NULL, any protocol will be matched.
    void            SetPort(const std::string sServiceOrPort, const char* szProtocol = NULL);
    int             GetPort() const;
// 
    // Return: 209.47.41.255:80
    std::string     GetString() const;
// 
    friend class CSocket;
    friend class CSocketStream;
    friend class CSocketDatagram;
};

#endif
