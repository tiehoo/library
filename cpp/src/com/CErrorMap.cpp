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

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <utility>
#include "CException.h"
#include "CErrorMap.h"

// --------------------------------------------------------------------------- /
// CErrorMap 1.0
// --------------------------------------------------------------------------- /
CErrorMap::CErrorMap()
{
    m_mapError.clear();
}

CErrorMap::~CErrorMap()
{
}

// --------------------------------------------------------------------------- /
void CErrorMap::AppendError(long nErrorCode, std::string sErrorMessage, std::string sCause, std::string sAction)
{
    tErrorEntry stError;
    stError.lCode       = nErrorCode;
    stError.sMessage    = sErrorMessage;
    stError.sCause      = sCause;
    stError.sAction     = sAction;
    // 
    std::pair<std::map<long, tErrorEntry>::const_iterator, bool> pairRet;
    pairRet = m_mapError.insert(std::make_pair(nErrorCode, stError));
    if(!pairRet.second)
    {
        // Found duplicate entries: 
        throw CException(-1, "%s(%ld): CErrorMap::AppendError(): Found duplicated error code: %d.", __FILE__, __LINE__, nErrorCode);
    }
}

// 
std::string CErrorMap::GetErrorMessage(long nErrorCode)
{
    std::map<long, tErrorEntry>::const_iterator iErr;
    iErr = m_mapError.find(nErrorCode);
    if(iErr == m_mapError.end()) return(std::string());
    else return(iErr->second.sMessage);
}

std::string CErrorMap::GetErrorCause(long nErrorCode)
{
    std::map<long, tErrorEntry>::const_iterator iErr;
    iErr = m_mapError.find(nErrorCode);
    if(iErr == m_mapError.end()) return(std::string());
    else return(iErr->second.sCause);
}

std::string CErrorMap::GetErrorAction(long nErrorCode)
{
    std::map<long, tErrorEntry>::const_iterator iErr;
    iErr = m_mapError.find(nErrorCode);
    if(iErr == m_mapError.end()) return(std::string());
    else return(iErr->second.sAction);
}

tErrorEntry CErrorMap::GetErrorEntry(long nErrorCode)
{
    std::map<long, tErrorEntry>::const_iterator iErr;
    iErr = m_mapError.find(nErrorCode);
    if(iErr == m_mapError.end()) return(tErrorEntry());
    else return(iErr->second);
}

// --------------------------------------------------------------------------- /
/*
void CErrorMap::LoadErrorMap()
{
}
*/
