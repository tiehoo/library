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
#include "CSocketStream.h"
// 
#include <errno.h>

// --------------------------------------------------------------------------- /
// CSocketStream 1.0
// --------------------------------------------------------------------------- /
CSocketStream::CSocketStream(int nDomain, bool bOpenSocket, bool bAutoClose):CSocket(nDomain, SOCK_STREAM, bOpenSocket, bAutoClose)
{
    m_bConnected = false;
}

// 
CSocketStream::~CSocketStream()
{
    try
    {
        ShutDown();
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): ~CSocketStream(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
void CSocketStream::Listen(int nQueueLen)
{
    try
    {
        if(!IsOpen()) throw CException(-1, "%s(%ld): CSocketStream::Listen(): Socket has not been opened.", __FILE__, __LINE__);
        //Listen:
    	if(listen(m_fdSocket, nQueueLen) != 0)
    	{
    	    throw CException(-1, "%s(%ld): CSocketStream::Listen(): listen() failed: [%s]", __FILE__, __LINE__, strerror(errno));
    	}
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketStream::Listen(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
void CSocketStream::Accept(CSocketStream& objSocketClient, CSocketAddressInet& objAddressClient)
{
    try
    {
        // Routine check: 
        if(AF_INET != objSocketClient.GetDomain())
        {
            throw CException(-1, "%s(%ld): CSocketStream::Accept(CSocketStream&, CSocketAddressInet&): objSocketClient is not in AF_INET domain.", 
                __FILE__, __LINE__, strerror(errno));
        }
        // 
        if(objSocketClient.IsOpen())
        {
            throw CException(-1, "%s(%ld): CSocketStream::Accept(): objSocketClient contents an opened socket, close it first.", 
                __FILE__, __LINE__, strerror(errno));
        }

        // Clean address: 
        objAddressClient.Clear();
        socklen_t nAddressLen = sizeof(objAddressClient.m_stAddress);

        // Accept:
        while(1)
        {
        	if((objSocketClient.m_fdSocket = accept(m_fdSocket, (sockaddr *)(&objAddressClient.m_stAddress), &nAddressLen)) < 0)
        	{
            #ifdef  EPROTO
        	    if(errno == EINTR || errno == ECONNABORTED)
            #else
                if (errno == ECONNABORTED)
            #endif
        	    {
                    continue;
                }else
                {
                    throw CException(-1, "%s(%ld): CSocketStream::Accept(): accept() failed: [%s]", __FILE__, __LINE__, strerror(errno));
                }
        	}else break;
        }
        // 
        objSocketClient.m_bConnected = true;
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketStream::Accept(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSocketStream::Accept(CSocketStream& objSocketClient, CSocketAddressUnix& objAddressClient)
{
    try
    {
        // Routine check: 
        if(AF_LOCAL != objSocketClient.GetDomain())
        {
            throw CException(-1, "%s(%ld): CSocketStream::Accept(CSocketStream&, CSocketAddressInet&): objSocketClient is not in AF_LOCAL domain.", 
                __FILE__, __LINE__, strerror(errno));
        }
        // 
        if(objSocketClient.IsOpen())
        {
            throw CException(-1, "%s(%ld): CSocketStream::Accept(): objSocketClient contents an opened socket, close it first.", 
                __FILE__, __LINE__, strerror(errno));
        }

        // Clean address: 
        objAddressClient.Clear();
        socklen_t nAddressLen = sizeof(objAddressClient.m_stAddress);

        // Accept: 
        while(1)
        {
        	if((objSocketClient.m_fdSocket = accept(m_fdSocket, (sockaddr *)(&objAddressClient.m_stAddress), &nAddressLen)) < 0)
        	{
            #ifdef  EPROTO
        	    if(errno == EINTR || errno == ECONNABORTED)
            #else
                if (errno == ECONNABORTED)
            #endif
        	    {
                    continue;
                }else
                {
                    throw CException(-1, "%s(%ld): CSocketStream::Accept(): accept() failed: [%s]", __FILE__, __LINE__, strerror(errno));
                }
        	}else break;
        }
        objAddressClient.m_stAddress.sun_path[nAddressLen] = '\0';
        // 
        objSocketClient.m_bConnected = true;
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketStream::Accept(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// 
void CSocketStream::Connect(const CSocketAddressInet& objAddress)
{
    if(!IsOpen()) throw CException(-1, "%s(%ld): CSocketStream::Connect(): Socket has not been opened.", __FILE__, __LINE__);
    //if(connect(m_fdSocket, (sockaddr *)(&objAddress.m_stAddress), sizeof(objAddress.m_stAddress)) < 0)
    if(connect(m_fdSocket, objAddress.GetAddressPointer(), sizeof(objAddress.m_stAddress)) < 0)
    {
        throw CException(-1, "%s(%ld): CSocketStream::Connect(): connect() failed: [%s]", __FILE__, __LINE__, strerror(errno));
    }
    // 
    m_bConnected = true;
}

// 
void CSocketStream::Connect(const CSocketAddressUnix& objAddress)
{
    if(!IsOpen()) throw CException(-1, "%s(%ld): CSocketStream::Connect(): Socket has not been opened.", __FILE__, __LINE__);
    //if(connect(m_fdSocket, (sockaddr *)(&objAddress.m_stAddress), sizeof(objAddress.m_stAddress)) < 0)
    if(connect(m_fdSocket, objAddress.GetAddressPointer(), sizeof(objAddress.m_stAddress)) < 0)
    {
        throw CException(-1, "%s(%ld): CSocketStream::Connect(): connect() failed: [%s]", __FILE__, __LINE__, strerror(errno));
    }
    // 
    m_bConnected = true;
}

// --------------------------------------------------------------------------- /
size_t CSocketStream::Read(void *pBuf, size_t nByte)
{
    // 
    size_t nLeft;
    ssize_t nRead = 0;
    //  Need to increase the pointer, so can not use void here.
    unsigned char* pRead;
    
    //Routine check: 
    if(pBuf == NULL)
    {
        throw CException(-1, "%s(%ld): CSocketStream::Read(void *, size_t): NULL pointer.", __FILE__, __LINE__);
    }
    if(nByte < 0) 
    {
        throw CException(-1, "%s(%ld): CSocketStream::Read(void *, size_t): Invalid buffer length: ", __FILE__, __LINE__, nByte);
    }
    if(!IsOpen())
    {
        throw CException(-1, "%s(%ld): CSocketStream::Read(void *, size_t): Socket has not been opened.", __FILE__, __LINE__);
    }
    // 
    if(nByte == 0) return(0);

    //Read device: 
    nLeft = nByte;
    pRead = (unsigned char *)pBuf;
    // 
    while(nLeft > 0)
    {
        if((nRead = read(m_fdSocket, pRead, nLeft)) <= 0)
        {
            //If device is a socket or pipe, we need to reread it: 
            if(errno == EINTR) continue;
            else
            {
                //Error occured: Here I returned value less than: -1000
                throw CException(-1024, "%s(%ld): CSocketStream::Read(void *, size_t): read() failed: [%s]", __FILE__, __LINE__, strerror(errno));
            }
        }else
        {
            nLeft   -= nRead;
            pRead   += nRead;
            break;
        }
    }
    return(nByte - nLeft);
}

// 
size_t CSocketStream::Write(const void *pBuf, size_t nByte)
{
    size_t nLeft;
    ssize_t nWrite = 0;
    unsigned char* pWrite;

    //Routine check: 
    if(pBuf == NULL) 
    {
        throw CException(-1, "%s(%ld): CSocketStream::Write(const void *, size_t): Null buffer pointer.", __FILE__, __LINE__);
    }
    if(nByte < 0) 
    {
        throw CException(-1, "%s(%ld): CSocketStream::Write(const void *, size_t): Invalid buffer length: ", __FILE__, __LINE__, nByte);
    }
    if(!IsOpen())
    {
        throw CException(-1, "%s(%ld): CSocketStream::Write(const void *, size_t): Socket has not been opened.", __FILE__, __LINE__);
    }
    // 
    if(nByte == 0) return(0);

    //Write device: 
    nLeft = nByte;
    pWrite = (unsigned char*)pBuf;
    while(nLeft > 0)
    {
        if((nWrite = write(m_fdSocket, pWrite, nLeft)) <= 0)
        {
            //If device is a socket or pipe, we need to reread it: 
            if(errno == EINTR) continue;
            else
            {
                //Error occured: Here I returned error less than -1000
                throw CException(-1024, "%s(%ld): CSocketStream::Write(const void *, size_t): write() failed: [%s]", __FILE__, __LINE__, strerror(errno));
            }
        }else
        {
            nLeft   -= nWrite;
            pWrite  += nWrite;
        }
    }
    return(nByte);
}

// --------------------------------------------------------------------------- /
void CSocketStream::ShutDown(int nHow)
{
    if(!IsOpen() || !IsConnected()) return;
    if(shutdown(m_fdSocket, nHow) != 0)
    {
        throw CException(-1, "%s(%ld): CSocketStream::ShutDown(): [Socket: %d, How: %d], shutdown() failed: [%s]", __FILE__, __LINE__, 
            m_fdSocket, nHow, strerror(errno));
    }
    // 
    m_bConnected = false;
}

// --------------------------------------------------------------------------- /
bool CSocketStream::IsConnected()
{
    return(m_bConnected);
}
