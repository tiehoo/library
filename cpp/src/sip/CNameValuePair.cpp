/* =========================================================================== /
* @category     Name Value Pair
* @package      Analyzer, Buffer
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
#include <stdio.h>
#include "SipDefinition.h"
#include "CException.h"
#include "CBufferAnalyzer.h"
#include "CNameValuePair.h"

/* --------------------------------------------------------------------------- /
// CNameValuePair 2.0
/ --------------------------------------------------------------------------- */
CNameValuePair::CNameValuePair(long nSize):CMessageBase(nSize)
{
    try
    {
        Clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::CNameValuePair(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CNameValuePair::CNameValuePair(const CNameValuePair& objNameValuePair):CMessageBase(objNameValuePair.GetSize()) 
{
    try
    {
        operator = (objNameValuePair);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::CNameValuePair(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CNameValuePair::~CNameValuePair()
{
}

//----------------------------------------------------------------------------//
void CNameValuePair::Clear()
{
    try
    {
        // 
        CMessageBase::Clear();
        // 
        m_pairName.first    = 0;
        m_pairName.second   = 0;
        // 
        m_pairValue.first   = 0;
        m_pairValue.second  = 0;
        // 
        m_cSeparator        = '\0';
        m_cQuotation        = '\0';
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CNameValuePair::PrintStructure() const 
{
    CMessageBase::PrintStructure();
    // 
    printf("Name(%d, %d): ", m_pairName.first, m_pairName.second);
    Print(m_pairName.first, m_pairName.second);
    printf("\n");
    // 
    printf("Separator: [%c]\n", m_cSeparator);
    // 
    printf("Value(%d, %d): ", m_pairValue.first, m_pairValue.second);
    Print(m_pairValue.first, m_pairValue.second);
    // 
    printf("\n");
    printf("Quotation: [%c]\n", m_cQuotation);
    // 
    printf("\n");
}

//----------------------------------------------------------------------------//
const CNameValuePair& CNameValuePair::operator = (const CNameValuePair& objNameValuePair)
{
    try
    {
        // 
        CMessageBase::operator = (objNameValuePair);
        // 
        m_pairName      = objNameValuePair.m_pairName;
        m_pairValue     = objNameValuePair.m_pairValue;
        m_cSeparator    = objNameValuePair.m_cSeparator;
        m_cQuotation    = objNameValuePair.m_cQuotation;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CMessageBase::operator = (): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CNameValuePair::operator == (const CNameValuePair& objNameValuePair) const
{
    if(!IsValid() || !objNameValuePair.IsValid()) 
    {
        return(false);
    }
    // 
    if(m_pairName.second != objNameValuePair.m_pairName.second) 
    {
        return(false);
    }
    if(strncasecmp(GetBufferPointer() + m_pairName.first, objNameValuePair.GetBufferPointer() + objNameValuePair.m_pairName.first, m_pairName.second) != 0)
    {
        return(false);
    }
    // 
    if(m_pairValue.second != objNameValuePair.m_pairValue.second) 
    {
        return(false);
    }
    if(strncasecmp(GetBufferPointer() + m_pairValue.first, objNameValuePair.GetBufferPointer() + objNameValuePair.m_pairValue.first, m_pairValue.second) != 0)
    {
        return(false);
    }
    // 
    return(true);
}

//----------------------------------------------------------------------------//
/*
name = "value"
*/
bool CNameValuePair::Parse(const char* szCharSetSeparator, const char* szCharSetQuotation) 
{
    try
    {
        // 
        assert(szCharSetSeparator);
        assert(szCharSetQuotation);
        // 
        long lOffset, lSkipLen, lSkipBackLen;
        char cTokenUsed, cTokenQuotation;
        // 
        CBufferAnalyzer objAnalyzer(*this);
        objAnalyzer.Rewind();

        // Skip preceding space: 
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        if(!objAnalyzer.IsEnd()) 
        {
            // Name: 
            m_pairName.first = objAnalyzer.GetOffset();
            // Find Separator: 
            lSkipLen = 0;
            if(objAnalyzer.FindChar(szCharSetSeparator, cTokenUsed, lSkipLen)) 
            {
                // Name: 
                lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairName.first + lSkipLen - 1 - 1);
                m_pairName.second   = lSkipLen - 1 - lSkipBackLen;
                // Separator: 
                m_cSeparator        = cTokenUsed;
            }else 
            {
                lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairName.first + lSkipLen - 1);
                m_pairName.second   = lSkipLen - lSkipBackLen;
                cTokenUsed = '\0';
            }
            // Value: 
            if('\0' != cTokenUsed) 
            {
                // Found separator: 
                objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                if(!objAnalyzer.IsEnd())
                {
                    lOffset = objAnalyzer.GetOffset();
                    if(objAnalyzer.FindChar(szCharSetQuotation, cTokenUsed, lSkipLen)) 
                    {
                        if(1 == lSkipLen) 
                        {
                            // Found quotation start: 
                            cTokenQuotation = cTokenUsed;
                            objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                            lOffset = objAnalyzer.GetOffset();
                            if(objAnalyzer.FindChar(szCharSetQuotation, cTokenUsed, lSkipLen)) 
                            {
                                // Found quotation end: 
                                if(cTokenQuotation != cTokenUsed) 
                                {
                                    // Quotations do not match: 
                                    SetInvalid("CNameValuePair: Quotations do not match, [%c,%c][%d]", cTokenQuotation, cTokenUsed, objAnalyzer.GetOffset());
                                    return(false);
                                }
                                // See if it is the end: 
                                objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                                if(!objAnalyzer.IsEnd()) 
                                {
                                    // 
                                    SetInvalid("CNameValuePair: Found content out of quotation mark, [%c,%c][%d]", cTokenQuotation, cTokenUsed, objAnalyzer.GetOffset());
                                    return(false);
                                }
                                // Found correct quotation wrapped value: 
                                m_cQuotation = cTokenQuotation;
                                // 
                                m_pairValue.first   = lOffset;
                                lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairValue.first + lSkipLen - 1 - 1);
                                m_pairValue.second  = lSkipLen - 1 - lSkipBackLen;
                            }else 
                            {
                                // Can not find quotation end: 
                                SetInvalid("CNameValuePair: Quotation not closed, [%c][%d]", cTokenQuotation, objAnalyzer.GetOffset());
                                return(false);
                            }
                        }else 
                        {
                            // Found quotation in the middle: 
                            SetInvalid("CNameValuePair: Found quotation in the middle of content: [%c][%d].", cTokenUsed, objAnalyzer.GetOffset());
                            return(false);
                        }
                    }else 
                    {
                        // Can not find quotation: Get value. 
                        m_pairValue.first   = lOffset;
                        lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairValue.first + lSkipLen - 1);
                        m_pairValue.second  = lSkipLen - lSkipBackLen;
                    }
                }else 
                {
                    // After separator are all space chars: No value. 
                }
            }else 
            {
                // Can not find separator: No value. 
            }
        }else 
        {
            // All space: [     ]
            SetInvalid("CNameValuePair: All space");
            return(false);
        }
        // 
        SetValid();
        return(IsValid());
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::Parse(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
bool CNameValuePair::IsNameEmpty() const 
{
    return(m_pairName.second <= 0);
}

bool CNameValuePair::IsSeparatorEmpty() const 
{
    return('\0' == m_cSeparator);
}

bool CNameValuePair::IsValueEmpty() const 
{
    return(m_pairValue.second <= 0);
}

bool CNameValuePair::IsQuotationEmpty() const 
{
    return('\0' == m_cQuotation);
}

//----------------------------------------------------------------------------//
bool CNameValuePair::GetName(std::string& sName) const 
{
    try
    {
        if(!IsNameEmpty())
        {
            Read(m_pairName.first, m_pairName.second, sName);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::GetName(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CNameValuePair::GetSeparator(char& cSeparator) const 
{
    if(!IsSeparatorEmpty())
    {
        cSeparator = m_cSeparator;
        return(true);
    }else 
    {
        return(false);
    }
}

bool CNameValuePair::GetQuotation(char& cQuotation) const
{
    if(!IsQuotationEmpty())
    {
        cQuotation = m_cQuotation;
        return(true);
    }else 
    {
        return(false);
    }
}

bool CNameValuePair::GetValue(std::string& sValue) const 
{
    try
    {
        if(!IsValueEmpty())
        {
            Read(m_pairValue.first, m_pairValue.second, sValue);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::GetValue(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
