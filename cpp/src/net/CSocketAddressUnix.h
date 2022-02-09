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
#ifndef _CSOCKETADDRESSUNIX_H
#define _CSOCKETADDRESSUNIX_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>

// struct sockaddr_un
#include <sys/un.h>
#include "CSocketAddress.h"

// --------------------------------------------------------------------------- /
// CSocketAddressUnix 1.0 (INET4)
// --------------------------------------------------------------------------- /
// Consts: 
// 
class CSocketAddressUnix : public CSocketAddress
{
private: 
// 
    struct sockaddr_un      m_stAddress;
// 
    // Initialize all data: bzero()
    void                    Clear();
public: 
    CSocketAddressUnix();
    CSocketAddressUnix(const CSocketAddressUnix& objAddress);
    ~CSocketAddressUnix();
// 
    const CSocketAddressUnix& 
                    operator = (const CSocketAddressUnix& objAddress);
// 
    // Set unix domain device name, like: /tmp/unix_domain_sock
    void            SetPath(std::string sPath);
    std::string     GetPath() const;
// 
    // 1. google.com:80, 209.47.41.255:80
    std::string     GetString() const;
// 
    friend class CSocket;
    friend class CSocketStream;
    friend class CSocketDatagram;
};

#endif
