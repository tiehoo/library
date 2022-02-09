/* =========================================================================== /
* @category     Message
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
#include <string.h>
#include "CException.h"
#include "CParameterList.h"

// --------------------------------------------------------------------------- /
// CParameterList 1.0
// --------------------------------------------------------------------------- /
CParameterList::CParameterList(long nSize):CMessageBase(nSize) 
{
    try
    {
        Clear();
    }catch(CException& eEx)
    {
        eEx.SetMessage("CParameterList::CParameterList(): Failed.");
        throw eEx;
    }
}

// 
CParameterList::CParameterList(const CParameterList& objParameterList)
{
    try
    {
        operator = (objParameterList);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CParameterList::CParameterList(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CParameterList::~CMessageParameterList()
{
}

//----------------------------------------------------------------------------//
void CParameterList::Clear()
{
    try
    {
        // 
        CMessageBase::Clear();
        // 
        m_mapParameter.clear();
        m_cSeparator    = '\0';
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CParameterList::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

/* 
name="value"; name="value"
*/
bool CParameterList::Parse(const char* szSeparatorList, const char* szSeparatorNameValue, const char* szQuotation)
{
}

// --------------------------------------------------------------------------- /
const CParameterList& CParameterList::operator = (const CParameterList& objParameterList)
{
    try
    {
        // Buffer: 
        CMessageLine::operator = (objParameterList);
        // Member: 
        m_mapParameter  = objParameterList.m_mapParameter;
        m_cSeparator    = objParameterList.m_cSeparator;
        return(*this);
    }catch(CException& eEx)
    {
        eEx.SetMessage("CParameterList::operator = (): Failed.");
        throw eEx;
    }
}

bool CParameterList::operator == (const CParameterList& objParameterList) const
{
    try
    {
        if(!IsValid() || !objParameterList.IsValid()) return(false);
        return(m_mapParameter == objParameterList.m_mapParameter);
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CParameterList::operator == (): Failed.");
        throw eEx;
    }
}

bool CParameterList::IsEmpty() const
{
    try
    {
        if(!IsValid() || 0 == GetLength()) 
        {
            return(true);
        }else
        {
            return(m_mapParameter.empty());
        }
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CMessageParameterList::IsEmpty(): Failed.");
        throw eEx;
    }
}

// 
int CParameterList::GetCount() const
{
    return(m_mapParameter.size());
}
