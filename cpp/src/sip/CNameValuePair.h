/* =========================================================================== /
* @category         Name Value Pair
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CNameValuePair.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CNAMEVALUEPAIR_H
#define _CNAMEVALUEPAIR_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include "CMessageBase.h"

//---------------------------------------------------------------------------//
// Constants: 
//  Message Length: 
const long  DEF_NAME_VALUE_PAIR_LEN     = 256;

// 
const char  NAME_VALUE_PAIR_SEPARATOR[] = "=";  // 
const char  NAME_VALUE_PAIR_QUOTATION[] = "\"";   // 

// =========================================================================== /
// CNameValuePair 1.0
// =========================================================================== /
class CNameValuePair : public CMessageBase 
{
protected: 
    std::pair<int, int>         m_pairName;
    std::pair<int, int>         m_pairValue;
    char                        m_cSeparator;
    char                        m_cQuotation;
public: 
    explicit CNameValuePair(long nSize = DEF_NAME_VALUE_PAIR_LEN);
    CNameValuePair(const CNameValuePair& objNameValuePair);
    ~CNameValuePair();
// 
    virtual void Clear();
    virtual bool Parse(const char* szCharSetSeparator = NAME_VALUE_PAIR_SEPARATOR, const char* szCharSetQuotation = NAME_VALUE_PAIR_QUOTATION);
    virtual void PrintStructure() const;
// 
    virtual const CNameValuePair& operator = (const CNameValuePair& objNameValuePair);
    virtual bool  operator == (const CNameValuePair& objNameValuePair) const;
// 
    bool    IsNameEmpty() const;
    bool    IsSeparatorEmpty() const;
    bool    IsValueEmpty() const;
    bool    IsQuotationEmpty() const;
// 
    bool    GetName(std::string& sName) const;
    bool    GetSeparator(char& cSeparator) const;
    bool    GetQuotation(char& cQuotation) const;
    bool    GetValue(std::string& sValue) const;
};

#endif
