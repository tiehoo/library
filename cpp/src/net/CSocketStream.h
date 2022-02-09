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
#ifndef _CSOCKETSTREAM_H
#define _CSOCKETSTREAM_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CSocket.h"

// --------------------------------------------------------------------------- /
//CSocketStream 1.0
// --------------------------------------------------------------------------- /
class CSocketStream : public CSocket
{
private: 
    bool        m_bConnected;
public: 
    /* 
    // bOpenSocket: If need to call socket() to initialize m_fdSocket.
    // bAutoClose: If need to auto close socket in ~CSocketStream().
    */
    CSocketStream(int nDomain = AF_INET, bool bOpenSocket = true, bool bAutoClose = true);
    ~CSocketStream();

// 
    // TCP/Unix domain(Stream) Server: 
    void                    Listen(int nQueueLen = 5);
    // TCP/Unix domain(Stream) Server: 
    void                    Accept(CSocketStream& objSocketClient, CSocketAddressInet& objAddressClient);
    void                    Accept(CSocketStream& objSocketClient, CSocketAddressUnix& objAddressClient);
// 
    // TCP/Unix domain(Stream) Client: 
    void                    Connect(const CSocketAddressInet& objAddress);
    void                    Connect(const CSocketAddressUnix& objAddress);
// Raw device interface: 
    // TCP/UnixDomain read and write
    // This is allso the buffer interface: 
    size_t                  Read(void *pBuf, size_t nByte);
    size_t                  Write(const void *pBuf, size_t nByte);
// 
    // TCP/Unix domain(Stream) Server and Client: 
    // ShutDown: Cause a TCP FIN.
    void                    ShutDown(int nHow = SHUT_RDWR);     // SHUT_RD, SHUT_WR, SHUT_RDWR
// 
    bool                    IsConnected();
};

#endif
