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

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CException.h"
#include "CSocketDatagram.h"
// 
#include <errno.h>

// --------------------------------------------------------------------------- /
// CSocketDatagram 1.0
// --------------------------------------------------------------------------- /
CSocketDatagram::CSocketDatagram(int nDomain, bool bOpenSocket, bool bAutoClose):CSocket(nDomain, SOCK_DGRAM, bOpenSocket, bAutoClose)
{
}

// 
CSocketDatagram::~CSocketDatagram()
{
}

// --------------------------------------------------------------------------- /
// Here I assume that you allocated the buffer 1 byte more than the MAX length of data
// could be received, if I get the data length equal to max buffer length, I trited this as an error.
size_t CSocketDatagram::RecvFrom(void *pBuf, size_t nByte, CSocketAddressInet& objAddressFrom)
{
    try
    {
        // 
        if(pBuf == NULL)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): NULL pointer.", 
                __FILE__, __LINE__);
        }
        if(nByte <= 0)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): Invalid buffer size: %d.", 
                __FILE__, __LINE__, nByte);
        }
        if(!IsOpen())
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): Socket has not been opened.",
                __FILE__, __LINE__);
        }
        
        // Clean address: 
        objAddressFrom.Clear();
        socklen_t nAddressLen = sizeof(objAddressFrom.m_stAddress);

        // It is possible to receive a 0 length UDP: 
        ssize_t nSize;
        if((nSize = recvfrom(m_fdSocket, pBuf, nByte, 0, (sockaddr *)(&objAddressFrom.m_stAddress), &nAddressLen)) < 0)
        {
            throw CException(-1, "%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): recvfrom() failed: [%s]", 
                __FILE__, __LINE__, strerror(errno));
        }
        // 
        if(nSize >= nByte)
        {
            throw CException(-1, "%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): recvfrom(), real length read equal to buffer size.",
                __FILE__, __LINE__);
        }
        return(nSize);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
size_t CSocketDatagram::RecvFrom(void *pBuf, size_t nByte, CSocketAddressUnix& objAddressFrom)
{
    try
    {
        // 
        if(pBuf == NULL)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): NULL pointer.", 
                __FILE__, __LINE__);
        }
        if(nByte <= 0)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): Invalid buffer size: %d.", 
                __FILE__, __LINE__, nByte);
        }
        if(!IsOpen())
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::RecvFrom(void *, size_t, CSocketAddressInet&): Socket has not been opened.",
                __FILE__, __LINE__);
        }
        
        // Clean address: 
        objAddressFrom.Clear();
        socklen_t nAddressLen = sizeof(objAddressFrom.m_stAddress);

        // It is possible to receive a 0 length UDP: 
        ssize_t nSize;
        if((nSize = recvfrom(m_fdSocket, pBuf, nByte, 0, (sockaddr *)(&objAddressFrom.m_stAddress), &nAddressLen)) < 0)
        {
            throw CException(-1, "%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): recvfrom() failed: [%s]", 
                __FILE__, __LINE__, strerror(errno));
        }
        // 
        if(nSize >= nByte)
        {
            throw CException(-1, "%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): recvfrom(), real length read equal to buffer size.",
                __FILE__, __LINE__);
        }
        return(nSize);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocket::RecvFrom(void *, size_t, CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
size_t CSocketDatagram::SendTo(const void *pBuf, size_t nByte, const CSocketAddressInet& objAddressTo)
{
    try
    {
        if(pBuf == NULL) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): NULL pointer.", 
                __FILE__, __LINE__);
        }
        if(nByte <= 0) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Invalid buffer size: %d.", 
                __FILE__, __LINE__, nByte);
        }
        if(!IsOpen()) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Socket has not been opened.", 
                __FILE__, __LINE__);
        }
        // 
        if(sendto(m_fdSocket, pBuf, nByte, 0, objAddressTo.GetAddressPointer(), sizeof(objAddressTo.m_stAddress)) != nByte)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): sendto() failed: [%s]", 
                __FILE__, __LINE__, strerror(errno));
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
size_t CSocketDatagram::SendTo(const void *pBuf, size_t nByte, const CSocketAddressUnix& objAddressTo)
{
    try
    {
        if(pBuf == NULL) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): NULL pointer.", 
                __FILE__, __LINE__);
        }
        if(nByte <= 0) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Invalid buffer size: %d.", 
                __FILE__, __LINE__, nByte);
        }
        if(!IsOpen()) 
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Socket has not been opened.", 
                __FILE__, __LINE__);
        }
        //if(sendto(m_fdSocket, pBuf, nByte, 0, (sockaddr *)(&objAddressTo.m_stAddress), sizeof(objAddressTo.m_stAddress)) != nByte)
        if(sendto(m_fdSocket, pBuf, nByte, 0, objAddressTo.GetAddressPointer(), sizeof(objAddressTo.m_stAddress)) != nByte)
        {
            throw CException(-1, "%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): sendto() failed: [%s]", 
                __FILE__, __LINE__, strerror(errno));
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketDatagram::SendTo(const void *, size_t, CSocketAddressInet&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
