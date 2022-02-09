/* =========================================================================== /
* @category     SocketAddress
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
#include <stdio.h>
#include "CSocketAddress.h"

/* --------------------------------------------------------------------------- /
//CCSocketAddress 1.0
/ --------------------------------------------------------------------------- */
// 
CSocketAddress::CSocketAddress()
{
    m_pstAddress = NULL;
}

// 
CSocketAddress::~CSocketAddress()
{
}

// --------------------------------------------------------------------------- /
const sockaddr* CSocketAddress::GetAddressPointer() const
{
    return(m_pstAddress);
}
