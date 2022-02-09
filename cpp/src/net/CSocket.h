/* =========================================================================== /
* @category     Socket
* @package      
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: mysql.class.php,v 1.121 2006/08/29 21:25:17 artur Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CSOCKET_H
#define _CSOCKET_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// AF_INET, SOCK_STREAM
#include <sys/types.h>
#include <sys/socket.h>
// 
#include "CSocketAddressInet.h"
#include "CSocketAddressUnix.h"

// --------------------------------------------------------------------------- /
//CSocket 1.0
// --------------------------------------------------------------------------- /
class CSocket
{
protected: 
// Unique ID: Used in pool.
    long                    m_lId;
// Socket: 
    // Socket device/file descriptor: 
    int                     m_fdSocket;
    // 
    int                     m_nDomain;
    int                     m_nType;
// Address string: IP:port. Used in socket pool.
    // For TCP/UDP listener, the address string is the address bound.
    // For TCP client, the address string is the peer's address.
    // For UDP client, it has no real meaning.
    std::string             m_sAddressString;
// 
    // 
    bool                    m_bAutoClose;
// 
    // 
    void                    SetDomain(int nDomain);
    void                    SetType(int nType);
public: 
    // bAutoClose: If need to auto close socket in ~CSocket()
    explicit CSocket(int nDomain = AF_INET, int nType = SOCK_STREAM, bool bOpenSocket = true, bool bAutoClose = true);
    ~CSocket();
// 
    // TCP/UDP/Unix domain: 
    void                    Bind(const CSocketAddressInet& objAddress);
    void                    Bind(const CSocketAddressUnix& objAddress);
//  
    void                    SetId(long lId);
    long                    GetId();
// 
    void                    SetAddressString(std::string sAddressString);
    std::string             GetAddressString();
// 
    bool                    IsAutoClose();
    bool                    IsOpen();
// 
    // Call socket() to initialize m_fdSocket.
    void                    Open();
    // Close: only reduce file descriptor access counter 1, 
    //  won't cause TCP to send a FIN, if it is still great than 0.
    void                    Close();
// 
    int                     GetDomain();
    int                     GetType();
// 
    int                     GetDescriptor();
};

#endif
