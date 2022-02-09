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
#include "CSocket.h"

// socket()
#include <errno.h>

// --------------------------------------------------------------------------- /
// CSocket 1.0
// --------------------------------------------------------------------------- /
void CSocket::SetDomain(int nDomain)
{
    m_lId = 0;
    switch(nDomain)
    {
        case AF_INET: 
        case PF_LOCAL: 
            break;
        default: 
            throw CException(-1, "%s(%ld): CSocket::SetDomain(int): Invalid domain: [%d]", __FILE__, __LINE__, nDomain);
    }
    m_nDomain = nDomain;
}

void CSocket::SetType(int nType)
{
    switch(nType)
    {
        case SOCK_STREAM: 
        case SOCK_DGRAM: 
            break;
        default: 
            throw CException(-1, "%s(%ld): CSocket::SetType(int): Invalid type: [%d]", __FILE__, __LINE__, nType);
    }
    m_nType = nType;
}

// --------------------------------------------------------------------------- /
CSocket::CSocket(int nDomain, int nType, bool bOpenSocket, bool bAutoClose) 
{
    try
    {
        // 
        m_fdSocket      = -1;
        // 
        SetDomain(nDomain);
        SetType(nType);
        // 
        if(bOpenSocket)
        {
            Open();
        }
        // 
        m_bAutoClose    = bAutoClose;
        // 
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CSocket::CSocket(int, int, bool, bool): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSocket::~CSocket()
{
    try
    {
        if(!IsOpen()) return;
        if(IsAutoClose())
        {
            // Note: UDP don't have to be closed: 
            Close();
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocket::~CSocket(): Failed.", __FILE__, __LINE__);
        throw eEx;
        return;
    }
}

// --------------------------------------------------------------------------- /
void CSocket::Bind(const CSocketAddressInet& objAddress)
{
    try
    {
        if(!IsOpen()) throw CException(-1, "%s(%ld): CSocket::Bind(): Socket has not been opened.", __FILE__, __LINE__);
        //Bind first: 
        if(bind(m_fdSocket, (sockaddr *)(&objAddress.m_stAddress), sizeof(objAddress.m_stAddress)) != 0)
        {
            throw CException(-1, "%s(%ld): CSocket::Bind(): bind() [%s] failed: [%s]", 
                __FILE__, __LINE__, objAddress.GetString().c_str(), strerror(errno));
        }
        // Set address: My address.
        SetAddressString(objAddress.GetString());
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocket::Bind(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSocket::Bind(const CSocketAddressUnix& objAddress)
{
    try
    {
        if(!IsOpen()) throw CException(-1, "%s(%ld): CSocket::Bind(): Socket has not been opened.", __FILE__, __LINE__);
        //Bind first: 
        if(bind(m_fdSocket, (sockaddr *)(&objAddress.m_stAddress), sizeof(objAddress.m_stAddress)) != 0)
        {
            throw CException(-1, "%s(%ld): CSocket::Bind(): bind() [%s] failed: [%s]", 
                __FILE__, __LINE__, objAddress.GetString().c_str(), strerror(errno));
        }
        // Set address: My address.
        SetAddressString(objAddress.GetString());
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocket::Bind(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
void CSocket::SetId(long lId)
{
    if(lId <= 0) throw CException(-1, "%s(%ld): CSocket::SetId(long): Invlaid ID: %ld.", __FILE__, __LINE__, lId);
    m_lId = lId;
}

long CSocket::GetId()
{
    return(m_lId);
}

// --------------------------------------------------------------------------- /
void CSocket::SetAddressString(std::string sAddressString)
{
    m_sAddressString = sAddressString;
}

std::string CSocket::GetAddressString()
{
    return(m_sAddressString);
}

// --------------------------------------------------------------------------- /
bool CSocket::IsAutoClose()
{
    return(m_bAutoClose);
}

// 
bool CSocket::IsOpen()
{
    if(m_fdSocket > 0) return(true);
    else return(false);
}

// --------------------------------------------------------------------------- /
void CSocket::Open()
{
    if(IsOpen()) return;
    else
    {
        if((m_fdSocket = socket(m_nDomain, m_nType, 0)) < 0)
        {
            throw CException(-1, "%s(%ld): CSocket::CSocket(): socket() failed: [%s]", __FILE__, __LINE__, strerror(errno));
        }
    }
}

// 
void CSocket::Close()
{
    if(!IsOpen()) return;
    // Retry two times: 
    for(int i = 0; i < 3; i++)
    {
        if(close(m_fdSocket) != 0)
        {
            if(errno == EINTR) continue;    // Interrupted by signal.
            else
            {
                throw CException(-1, "%s(%ld): CSocket::Close(): [Socket: %ld], close() failed: [%s]", __FILE__, __LINE__, 
                    m_fdSocket, strerror(errno));
            }
        }else break;
    }
    m_fdSocket = -1;
    // 
    if(AF_LOCAL == GetDomain() && !m_sAddressString.empty())
    {
        if(0 != unlink(m_sAddressString.c_str()))
        {
            // Ignore.
        }
    }
}

int CSocket::GetDomain()
{
    return(m_nDomain);
}

int CSocket::GetType()
{
    return(m_nType);
}

// 
int CSocket::GetDescriptor()
{
    if(!IsOpen())
    {
        throw CException(-1, "%s(%ld): CSocket::GetDescriptor(): Descriptor has not been initialized, socket not opened.", 
                    __FILE__, __LINE__);
    }
    return(m_fdSocket);
}
