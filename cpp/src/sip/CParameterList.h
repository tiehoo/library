/* =========================================================================== /
* @category         Message
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CPARAMTERLIST_H
#define _CPARAMTERLIST_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <map>
#include "CMessageBase.h"
#include "CNameValuePair.h"

//Constants: 
const int   DEF_PARAMETER_LIST_LEN       = 512;     // Default parameter length.

// =========================================================================== /
// CMessageParameterList 1.0
// =========================================================================== /
// CParameterList who's format is name1=value1;name2=value2;...
class CParameterList : public CMessageBase
{
private: 
    std::map<std::string, CNameValuePair>
                                    m_mapParameter;
    char                            m_cSeparator;
public: 
    explicit CParameterList(long nSize = DEF_PARAMETER_LIST_LEN);
    explicit CParameterList(const CParameterList& objParameterList);
    ~CParameterList();
//  
    virtual void                    Clear();
    virtual bool                    Parse(const char* szSeparatorList, const char* szSeparatorNameValue, const char* szQuotation);
    virtual void                    PrintStructure() const;
//  
    const CParameterList&           operator =  (const CParameterList& objParameterList);
    bool                            operator == (const CParameterList& objParameterList) const;
//  
    bool                            IsEmpty() const;
    int                             GetCount() const;
//  
    // 
    bool                            GetSeparator(char& cSeparator) const;
    bool                            GetParameter(const char* szName, std::string& sValue) const;
    bool                            GetParameter(const char* szName, CNameValuePair& objParameter) const;
    // If strValue = "", I will only generate: name. For supporting of: lr=r equal to lr;
/*
    void                            SetParameter(const char* szName, const char* szValue);
    void                            SetParameter(const char* szName, const std::string& strValue);
    void                            SetParameter(const std::string& strName, const std::string& strValue);
    // 
    void                            RemoveParameter(const char* szName);
    void                            RemoveParameter(const std::string& strName);
*/
};

#endif
