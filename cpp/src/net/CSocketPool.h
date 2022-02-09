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
#ifndef _CSOCKETPOOL_H
#define _CSOCKETPOOL_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>
#include <map>
// 
#include "CSocket.h"
#include "CTimeInterval.h"

// --------------------------------------------------------------------------- /
// CSocketPool 1.0
// --------------------------------------------------------------------------- /
// 
class CSocketPool
{
private: 
    long                m_lId;
    // Socket map: <ID, Socket*>, all sockets allocated.
    //  ID will keep increasing in a round robin mode.
    std::map<long, CSocket*>
                        m_mapSocket;

    // Socket indexes: Only for opened and binded sockets.
    //  Binding index: <address string, ID>
    std::map<std::string, long>
                        m_mapAddress;
// 
    long                GetNextSocketId();
// 
    void                Free();
public: 
    CSocketPool();
    ~CSocketPool();
// 
    // Allocate a socket, return the reference: 
    CSocket*            AllocateSocket(int nDomain = AF_INET, int nType = SOCK_STREAM, bool bOpenSocket = true, bool bAutoClose = true);
    // If return NULL: Can not find. 
    void                SetSocketAddress(long lSocketId, std::string sAddressString);
    // 
    void                FreeSocket(long lSocketId);
// 
    // If you don't want to set time out. Set pTimeInterval to NULL.
    //  Note: Set TimeInterval to 0 is still a valid value.
    int                 Select(std::list<CSocket*>& listSocket, const CTimeInterval* pTimeInterval);
    bool                Poll();
// 
    // If can not find return false.
    bool                GetSocketId(std::string sAddressString, long& lSocketId) const;
    // 
    bool                DoesSocketExist(long lSocketId) const;
    CSocket*            GetSocket(long lSocketId);
    bool                DoesSocketExist(std::string sAddressString) const;
    CSocket*            GetSocket(std::string sAddressString);
// 
    int                 GetSize();
};

#endif
