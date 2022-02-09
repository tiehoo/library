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
#include "CSocketAddressInet.h"
#include "CSocketAddressUnix.h"
#include "CSocket.h"

// --------------------------------------------------------------------------- /
//CSocketDatagram 1.0
// --------------------------------------------------------------------------- /
class CSocketDatagram : public CSocket
{
public: 
    /* 
    // bOpenSocket: If need to call socket() to initialize m_fdSocket.
    // bAutoClose: If need to auto close socket in ~CSocketDatagram().
    */
    CSocketDatagram(int nDomain = AF_INET, bool bOpenSocket = true, bool bAutoClose = true);
    ~CSocketDatagram();

    // UDP/Unix doman(Diagram) Server and Client: 
    size_t      RecvFrom(void *pBuf, size_t nByte, CSocketAddressInet& objAddressFrom);
    size_t      RecvFrom(void *pBuf, size_t nByte, CSocketAddressUnix& objAddressFrom);
    size_t      SendTo(const void *pBuf, size_t nByte, const CSocketAddressInet& objAddressTo);
    size_t      SendTo(const void *pBuf, size_t nByte, const CSocketAddressUnix& objAddressTo);
};

#endif
