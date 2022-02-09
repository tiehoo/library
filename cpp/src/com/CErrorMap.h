/* =========================================================================== /
* @category     ERROR
* @package      
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */

#ifndef _CERRORCODE_H
#define _CERRORCODE_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>
#include <map>

// Constants: 
// Error: 3xxxx;
// Warnning: 1xxxx;

/*
// RADIUS client: 
//  Warnning: 
//  Get duplicated packet: 
const int   WAR_CODE_RAD_DUPLICATE_PACKET       = 11001;
const char  WAR_MSG_RAD_DUPLICATE_PACKET[]      = "# Radius client got an duplicated/old packet, this means radius server is responding slowly or network condition between client and server is poor.";
//  Get invalid response: Check sum not right.
const int   WAR_CODE_RAD_INVALID_SUM            = 11011;
const char  WAR_MSG_RAD_MSG_INVALID_SUM[]       = "# Get an invalid response, check sum verification failed. Discarded silently.";
//  Get invalid response: Code is wrong.
const int   WAR_CODE_RAD_INVALID_CODE           = 11012;
const char  WAR_MSG_RAD_MSG_INVALID_CODE[]      = "# Get an invalid response, code is wrong. Discarded silently.";

//  Error: 
//  Server timed out: 
const int   ERR_CODE_RAD_SERVER_TIMEOUT         = 31001;
const char  ERR_MSG_RAD_MSG_SERVER_TIMEOUT[]    = "# Radius server timed out after (#second, #microscond).";
*/

// --------------------------------------------------------------------------- /
// CErrorMap 1.0
// --------------------------------------------------------------------------- /
// Constants: 
typedef struct stErrorEntry
{
    long        lCode;      // 31001
    std::string sMessage;   // Radius server timed out after (#second, #microscond).
    std::string sCause;     // Radius server did not respond in time, 1) Radius server is getting slow. 2) Time out value is too small. 3) Network condition between client and server is not good enough.
    std::string sAction;    // Check radius server's performance; Check time out value, enlarge it if possible; Check network between client and server.
    // 
    const struct stErrorEntry&  operator = (const struct stErrorEntry& stError)
    {
        lCode       = stError.lCode;
        sMessage    = stError.sMessage;
        sCause      = stError.sCause;
        sAction     = stError.sAction;
	return(*this);
    }
    // 
    void Clear()
    {
        lCode   = 0;
    }
    // 
    stErrorEntry()
    {
        Clear();
    }
} tErrorEntry;

// 
class CErrorMap
{
private: 
    std::map<long, tErrorEntry>  m_mapError;
    // 
public: 
    CErrorMap();
    ~CErrorMap();
// 
    void            AppendError(long nErrorCode, std::string sErrorMessage, std::string sCause = "", std::string sAction = "");
    // 
    std::string     GetErrorMessage(long nErrorCode);
    std::string     GetErrorCause(long nErrorCode);
    std::string     GetErrorAction(long nErrorCode);
    // 
    tErrorEntry     GetErrorEntry(long nErrorCode);
// 
/*
    virtual void    LoadErrorMap();
*/
};

#endif
