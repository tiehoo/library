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
#include "CSocketAddressUnix.h"

// AF_LOCAL
#include <sys/socket.h>

/* --------------------------------------------------------------------------- /
//CCSocketAddressUnix 1.0
/ --------------------------------------------------------------------------- */
void CSocketAddressUnix::Clear()
{
    // Clear address structure: 
    bzero(&m_stAddress, sizeof(m_stAddress));
    // Set family: 
    m_stAddress.sun_family = AF_LOCAL;
}

// --------------------------------------------------------------------------- /
CSocketAddressUnix::CSocketAddressUnix()
{
    // 
    m_pstAddress    = (sockaddr*)&m_stAddress;
    // 
    Clear();
}

// 
CSocketAddressUnix::CSocketAddressUnix(const CSocketAddressUnix& objAddress)
{
    try
    {
        /* DO NOT call this function! Will call CSocketAddress() and cause the reset of pointer: m_pstAddress.
        CSocketAddressUnix();
        */
        // 
        m_pstAddress    = (sockaddr*)&m_stAddress;
        // 
        Clear();
        // 
        operator = (objAddress);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressUnix::CSocketAddressUnix(const CSocketAddressUnix&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CSocketAddressUnix::~CSocketAddressUnix()
{
}

// --------------------------------------------------------------------------- /
const CSocketAddressUnix& CSocketAddressUnix::operator = (const CSocketAddressUnix& objAddress)
{
    try
    {
        // 
        memcpy(&m_stAddress, &objAddress.m_stAddress, sizeof(m_stAddress));
        return(*this);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressUnix::operator = (const CSocketAddressUnix&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// Unix domain socket: 
void CSocketAddressUnix::SetPath(std::string sPath)
{
    try
    {
        // 
        if(sPath.length() >= sizeof(m_stAddress.sun_path))
        {
            throw CException(-1, "%s(%ld): CSocketAddress::SetPath(string): Path name too long: %s.", __FILE__, __LINE__, sPath.c_str());
        }
        strncpy(m_stAddress.sun_path, sPath.c_str(), sPath.length());
        m_stAddress.sun_path[sPath.length()] = '\0';
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddress::SetPath(string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
std::string CSocketAddressUnix::GetPath() const
{
    return(std::string(m_stAddress.sun_path));
}

// --------------------------------------------------------------------------- /
std::string CSocketAddressUnix::GetString() const
{
    try
    {
        return(GetPath());
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSocketAddressUnix::GetString(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
