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
#include <errno.h>
#include <utility>
// 
#include "CException.h"
#include "CSocketPool.h"
#include "CSocketStream.h"

// --------------------------------------------------------------------------- /
// CSocketPool 1.0
// --------------------------------------------------------------------------- /
// 
long CSocketPool::GetNextSocketId()
{
    m_lId++;
    if(m_lId < 0) m_lId = 1;
    return(m_lId);
}

// Free all allocated sockets in pool: 
void CSocketPool::Free()
{
    // Just delete CSocket allocated.
    std::map<long, CSocket*>::iterator iSocket;
    for(iSocket = m_mapSocket.begin(); iSocket != m_mapSocket.end(); ++iSocket)
    {
        if(NULL != iSocket->second)
        {
            delete iSocket->second;
            iSocket->second = NULL;
        }
    }
}

// --------------------------------------------------------------------------- /
// 
CSocketPool::CSocketPool()
{
    m_lId = 0;
}

CSocketPool::~CSocketPool()
{
    try
    {
        Free();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::~CSocketPool(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// Allocate a socket, return the pointer: 
CSocket* CSocketPool::AllocateSocket(int nDomain, int nType, bool bOpenSocket, bool bAutoClose)
{
    try
    {
        // 
        CSocket* pSocket = NULL;
        // Allocate by type: 
        switch(nType)
        {
            case SOCK_STREAM: 
                pSocket = new CSocketStream(nDomain, bOpenSocket, bAutoClose);
                if(NULL == pSocket)
                {
                    throw CException(-1, "%s(%ld): CSocketAddress::AllocateSocket(int, int, bool, bool): Failed to allocate CSocketStream object.", 
                        __FILE__, __LINE__);
                }
                break;
            case SOCK_DGRAM: 
//                pSocket = new CSocketDatagram(nDomain, nType, bOpenSocket, bAutoClose);
                if(NULL == pSocket)
                {
                    throw CException(-1, "%s(%ld): CSocketAddress::AllocateSocket(int, int, bool, bool): Failed to allocate CSocketDatagram object.", 
                        __FILE__, __LINE__);
                }
                break;
            default: 
                throw CException(-1, "%s(%ld): CSocketAddress::AllocateSocket(int, int, bool, bool): Don't support domain: %d.", __FILE__, __LINE__, 
                    nDomain);
        }
        // 
        pSocket->SetId(GetNextSocketId());
        // I can still just don't check duplicate key here, just assume this won't happen.
        if(m_mapSocket.end() != m_mapSocket.find(pSocket->GetId()))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::AllocateSocket(int, int, bool, bool): Internal error, found duplicated ID: %ld.", __FILE__, __LINE__, 
                pSocket->GetId());
        }
        m_mapSocket.insert(std::make_pair(pSocket->GetId(), pSocket));
        // 
        return(pSocket);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::AllocateSocket(int, int, bool, bool): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// If return NULL: Can not find.
void CSocketPool::SetSocketAddress(long lSocketId, std::string sAddressString)
{
    try
    {
        // Check address index: 
        std::map<std::string, long>::iterator iAddress;
        if(m_mapAddress.end() != (iAddress = m_mapAddress.find(sAddressString)))
        {
            // Found binding already in address index, but diffrent: 
            if(iAddress->second == lSocketId)
            {
                // Alread bound to the same socket: Ignore.
                return;
            }else
            {
                throw CException(-1, "%s(%ld): CSocketAddress::SetSocketAddress(long, string): Address %s has bound to: %ld already, can not bind to: %ld any more.", 
                    __FILE__, __LINE__, sAddressString.c_str(), iAddress->second, lSocketId);
            }
        }
        // Check socket index: 
        std::map<long, CSocket*>::iterator iSocket;
        if(m_mapSocket.end() == (iSocket = m_mapSocket.find(lSocketId)))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::SetSocketAddress(long, string): Can not find socket with ID: %ld in pool.", __FILE__, __LINE__, 
                lSocketId);
        }
        // Update socket: 
        iSocket->second->SetAddressString(sAddressString);
        // Add new binding: 
        m_mapAddress.insert(make_pair(sAddressString, lSocketId));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::SetSocketAddress(long, string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CSocketPool::FreeSocket(long lSocketId)
{
    try
    {
        std::map<long, CSocket*>::iterator iSocket;
        if(m_mapSocket.end() == (iSocket = m_mapSocket.find(lSocketId)))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::FreeSocket(long): Can not find socket with ID: %ld in pool.", __FILE__, __LINE__, 
                lSocketId);
        }
        // Erase address entry: 
        std::map<std::string, long>::iterator iAddress;
        if(m_mapAddress.end() != (iAddress = m_mapAddress.find(iSocket->second->GetAddressString())))
        {
            m_mapAddress.erase(iAddress);
        }
        // Free the node: 
        delete iSocket->second;
        // Erase socket entry: 
        m_mapSocket.erase(iSocket);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::FreeSocket(CSocket*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
/*
       On  Linux, select() modifies timeout to reflect the amount of time not slept; most other implementations do not
       do this.  (POSIX.1-2001 permits either behaviour.)  This causes problems both when Linux code which reads time-
       out  is  ported  to  other operating systems, and when code is ported to Linux that reuses a struct timeval for
       multiple select()s in a loop without reinitializing it.   Consider  timeout  to  be  undefined  after  select()
       returns.
*/
/*
       Other than the difference in the precision of the timeout argument, the following pselect() call:
           ready = pselect(nfds, &readfds, &writefds, &exceptfds,
                           timeout, &sigmask);
       is equivalent to atomically executing the following calls:

           sigset_t origmask;

           sigprocmask(SIG_SETMASK, &sigmask, &origmask);
           ready = select(nfds, &readfds, &writefds, &exceptfds, timeout);
           sigprocmask(SIG_SETMASK, &origmask, NULL);

       The  reason  that pselect() is needed is that if one wants to wait for either a signal or for a file descriptor
       to become ready, then an atomic test is needed to prevent race conditions.  (Suppose the signal handler sets  a
       global flag and returns. Then a test of this global flag followed by a call of select() could hang indefinitely
       if the signal arrived just after the test but just before the call.  By contrast, pselect() allows one to first
       block signals, handle the signals that have come in, then call pselect() with the desired sigmask, avoiding the
       race.)
*/
/*
       1.     You should always try to use select() without a timeout. Your program should have nothing to do if there
              is no data available. Code that depends on timeouts is not usually portable and is difficult to debug.

       2.     The value nfds must be properly calculated for efficiency as explained above.

       3.     No  file descriptor must be added to any set if you do not intend to check its result after the select()
              call, and respond appropriately. See next rule.

       4.     After select() returns, all file descriptors in all sets should be checked to see if they are ready.

       5.     The functions read(), recv(), write(), and send() do not necessarily read/write the full amount of  data
              that  you have requested. If they do read/write the full amount, its because you have a low traffic load
              and a fast stream. This is not always going to be the case. You should cope with the case of your  func-
              tions only managing to send or receive a single byte.

       6.     Never read/write only in single bytes at a time unless your are really sure that you have a small amount
              of data to process. It is extremely inefficient not to read/write as much data as you  can  buffer  each
              time.  The buffers in the example above are 1024 bytes although they could easily be made larger.

       7.     The  functions read(), recv(), write(), and send() as well as the select() call can return -1 with errno
              set to EINTR, or with errno set to EAGAIN (EWOULDBLOCK).  These results must be  properly  managed  (not
              done  properly  above). If your program is not going to receive any signals then it is unlikely you will
              get EINTR. If your program does not set non-blocking I/O, you  will  not  get  EAGAIN.  Nonetheless  you
              should still cope with these errors for completeness.

       8.     Never call read(), recv(), write(), or send() with a buffer length of zero.

       9.     If  the functions read(), recv(), write(), and send() fail with errors other than those listed in 7., or
              one of the input functions returns 0, indicating end of file, then you should not pass  that  descriptor
              to  select()  again.  In the above example, I close the descriptor immediately, and then set it to -1 to
              prevent it being included in a set.

       10.    The timeout value must be initialized with each new call to select(), since some operating systems  mod-
              ify the structure. pselect() however does not modify its timeout structure.

       11.    I  have  heard that the Windows socket layer does not cope with OOB data properly. It also does not cope
              with select() calls when no file descriptors are set at all. Having no file descriptors set is a  useful
              way to sleep the process with sub-second precision by using the timeout.  (See further on.)
*/
int CSocketPool::Select(std::list<CSocket*>& listSocket, const CTimeInterval* pTimeInterval)
{
    try
    {
        // 
        int nMaxDescriptor1 = 0;
        fd_set m_setDescriptorRead;
        int nReadyCount;
        struct timeval  stTimeVal;

        // select: 
        while(1)
        {
            // Prepare file descriptor set: 
            FD_ZERO(&m_setDescriptorRead);
            // Add socket: 
            std::map<long, CSocket*>::iterator  iSocket;
            for(iSocket = m_mapSocket.begin(); iSocket != m_mapSocket.end(); ++iSocket)
            {
                if(iSocket->second->IsOpen())
                {
                    FD_SET(iSocket->second->GetDescriptor(), &m_setDescriptorRead);
                    if(iSocket->second->GetDescriptor() > nMaxDescriptor1) nMaxDescriptor1 = iSocket->second->GetDescriptor();
                }
            }
            nMaxDescriptor1++;

            // Check and prepare TimeInterval and select: 
            if(NULL == pTimeInterval) 
            {
                nReadyCount = select(nMaxDescriptor1, &m_setDescriptorRead, NULL, NULL, NULL);
            }else if(pTimeInterval->Sign() >= 0) 
            {
                pTimeInterval->GetTimeVal(stTimeVal);
                nReadyCount = select(nMaxDescriptor1, &m_setDescriptorRead, NULL, NULL, &stTimeVal);
            }else 
            {
                throw CException(-1, "%s(%ld): CSocketAddress::Select(CTimeInterval, list<CSocket*>): Time out value is negative: %ld, %ld.", __FILE__, __LINE__, 
                        pTimeInterval->GetSecond(), pTimeInterval->GetMicroSecond());
            }

            // 
            if(nReadyCount < 0)
            {
                // 
                if(EINTR == errno)
                {
                    // FIXME: Do I have adjust timeout here?
                    continue;
                }else
                {
                    throw CException(-1, "%s(%ld): CSocketPool::Select(list<CSocket*>, CTimeInterval*): select() failed: [%s]", __FILE__, __LINE__, 
                        strerror(errno));
                }
            }
            
            // Return the value: 
            listSocket.clear();
            if(0 == nReadyCount) return(nReadyCount);
            // 
            for(iSocket = m_mapSocket.begin(); iSocket != m_mapSocket.end(); ++iSocket)
            {
                if(iSocket->second->IsOpen() && FD_ISSET(iSocket->second->GetDescriptor(), &m_setDescriptorRead))
                {
                    listSocket.push_back(iSocket->second);
                }
            }
            if(listSocket.size() != nReadyCount)
            {
                throw CException(-1, "%s(%ld): CSocketPool::Select(list<CSocket*>, CTimeInterval*): Internal error: Expecting %ld ready socket, but only found %ld.", 
                    __FILE__, __LINE__, nReadyCount, listSocket.size());
            }
            return(nReadyCount);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::Select(CTimeInterval, std::list<CSocket*>): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSocketPool::Poll()
{
}

// --------------------------------------------------------------------------- /
bool CSocketPool::GetSocketId(std::string sAddressString, long& lSocketId) const
{
    try
    {
        // Check address index: 
        std::map<std::string, long>::const_iterator iAddress;
        if(m_mapAddress.end() != (iAddress = m_mapAddress.find(sAddressString)))
        {
            lSocketId = iAddress->second;
            return(true);
        }else return(false);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::GetSocketId(string, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSocketPool::DoesSocketExist(long lSocketId) const
{
    try
    {
        // Check socket index: 
        std::map<long, CSocket*>::const_iterator iSocket;
        if(m_mapSocket.end() != (iSocket = m_mapSocket.find(lSocketId))) return(true);
        else return(false);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::DoesSocketExist(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSocket* CSocketPool::GetSocket(long lSocketId)
{
    try
    {
        if(!DoesSocketExist(lSocketId))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::GetSocket(long): Can not find socket with ID: %ld in pool.", __FILE__, __LINE__, 
                    lSocketId);
        }
        return(m_mapSocket[lSocketId]);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::GetSocket(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSocketPool::DoesSocketExist(std::string sAddressString) const
{
    try
    {
        long lSocketId;
        return(GetSocketId(sAddressString, lSocketId));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::DoesSocketExist(string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSocket* CSocketPool::GetSocket(std::string sAddressString)
{
    try
    {
        long lSocketId;
        if(!GetSocketId(sAddressString, lSocketId))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::GetSocket(string): Can not find socket with ID: %ld in pool.", __FILE__, __LINE__, 
                    lSocketId);
        }
        return(GetSocket(lSocketId));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketPool::GetSocket(string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
int CSocketPool::GetSize()
{
    // Count binded only: 
    return(m_mapAddress.size());
}
