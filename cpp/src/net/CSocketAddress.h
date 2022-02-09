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
#ifndef _CSOCKETADDRESS_H
#define _CSOCKETADDRESS_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// struct sockaddr_in
#include <netinet/in.h>

// --------------------------------------------------------------------------- /
// CSocketAddress 1.0
// --------------------------------------------------------------------------- /
// Base class

class CSocketAddress
{
protected: 
    sockaddr*       m_pstAddress;
public: 
    CSocketAddress();
    ~CSocketAddress();
//
    const sockaddr* GetAddressPointer() const;
};

#endif
