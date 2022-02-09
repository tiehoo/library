/* =========================================================================== /
* @category         Generic value.
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CException.h"
#include "CRegularExpression.h"
#include "CValue.h"

//---------------------------------------------------------------------------------------------//
// CValue: 
//---------------------------------------------------------------------------------------------//
// 
bool CValue::IsCharInString(const char cChar, const char *szString, const int nMaxLen, const bool bIsCaseSensitive) const
{
    int i;
    if(NULL == szString) throw CException(-1, "%s(%ld): CValue::IsCharInString(char, const char *, int, bool): Null pointer.", __FILE__, __LINE__);
    if(bIsCaseSensitive)
    {
        for(i = 0; szString[i] != '\0' && i < nMaxLen && cChar != szString[i]; ++i);
    }else
    {
        for(i = 0; szString[i] != '\0' && i < nMaxLen && tolower(cChar) != tolower(szString[i]); ++i);
    }
    if(szString[i] == '\0' || i == nMaxLen) return(false);
    else return(true);
}

// 
bool CValue::IsCharInString(const char cChar, const char *szString, const bool bIsCaseSensitive) const
{
    try
    {
        if(NULL == szString) throw CException(-1, "%s(%ld): CValue::IsCharInString(): Null pointer.", __FILE__, __LINE__);
        return(IsCharInString(cChar, szString, strlen(szString), bIsCaseSensitive));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::IsCharInString(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

tValueType CValue::GetType() const
{
    return(m_emType);
}

//---------------------------------------------------------------------------------------------//
CValue::CValue()
{
    try
    {
        SetNull();
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const CValue& objValue)
{
    try
    {
        operator = (objValue);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(const CValue&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const char* pValue, const int nLen)
{
    try
    {
        SetValue(pValue, nLen);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(const char*, int): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const std::string sValue)
{
    try
    {
        SetValue(sValue);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(const std::string&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const char cValue)
{
    try
    {
        SetValue(cValue);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(char): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const long lValue)
{
    try
    {
        SetValue(lValue);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::CValue(const double fValue)
{
    try
    {
        SetValue(fValue);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::CValue(double): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CValue::~CValue()
{
}

//---------------------------------------------------------------------------------------------//
void CValue::SetNull()
{
    m_emType    = T_NULL;
    // 
    m_sValue.clear();
    m_cValue    = '\0';
    m_lValue    = 0;
    m_fValue    = 0;
}

bool CValue::IsNull() const
{
    if(T_NULL == m_emType)
    {
         return(true);
    }else
    {
        return(false);
    }
}
void CValue::SetValue(const char* pValue, int nLen)
{
    if(NULL == pValue) throw CException(-1, "%s(%ld): CValue::SetValue(const char*, int): NULL pointer.", __FILE__, __LINE__);
    if(nLen < 0) throw CException(-1, "%s(%ld): CValue::SetValue(const char*, int): Invalid length: %d.", __FILE__, __LINE__, nLen);
    m_emType    = T_STRING;
    m_sValue.assign(pValue, nLen);
}

void CValue::SetValue(const std::string sValue)
{
    m_emType    = T_STRING;
    m_sValue    = sValue;
}

void CValue::SetValue(const char cValue)
{
    m_emType    = T_CHAR;
    m_cValue    = cValue;
}

void CValue::SetValue(const long lValue)
{
    m_emType    = T_LONG;
    m_lValue    = lValue;
}

void CValue::SetValue(const double fValue)
{
    m_emType    = T_DOUBLE;
    m_fValue    = fValue;
}

//---------------------------------------------------------------------------------------------//
const CValue& CValue::operator = (const CValue& objValue) 
{
    if(this != &objValue)
    {
        m_emType    = objValue.m_emType;
        m_sValue    = objValue.m_sValue;
        m_cValue    = objValue.m_cValue;
        m_lValue    = objValue.m_lValue;
        m_fValue    = objValue.m_fValue;
    }
    return(*this);
}

const CValue& CValue::operator = (std::string sValue)
{
    try
    {
        SetValue(sValue);
        return(*this);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator = (std::string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const CValue& CValue::operator = (char cValue)
{
    try
    {
        SetValue(cValue);
        return(*this);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator = (char): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const CValue& CValue::operator = (long lValue)
{
    try
    {
        SetValue(lValue);
        return(*this);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator = (long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const CValue& CValue::operator = (double fValue)
{
    try
    {
        SetValue(fValue);
        return(*this);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator = (double): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------------------------//
bool CValue::operator == (const CValue& objValue) const
{
    try
    {
        if(m_emType != objValue.m_emType)
        {
            return(false);
        }else
        {
            switch(m_emType)
            {
                case T_NULL: // NULL is never equal to and value. Use IsNull();
                {
                    return(false);
                }
                case T_STRING: 
                {
                    return(m_sValue == objValue.m_sValue);
                }
                case T_CHAR: 
                {
                    return(m_cValue == objValue.m_cValue);
                }
                case T_LONG: 
                {
                    return(m_lValue == objValue.m_lValue);
                }
                case T_DOUBLE: 
                {
                    return(m_fValue == objValue.m_fValue);
                }
                default: 
                    throw CException(-1, "%s(%ld): CValue::operator ==(const CValue& objValue): Invalid value type: [%d].", __FILE__, __LINE__, m_emType);
            }
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator == (const CValue& objValue): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CValue::operator < (const CValue& objValue) const
{
    try
    {
        if(m_emType != objValue.m_emType)
        {
            return(false);
        }else
        {
            switch(m_emType)
            {
                case T_NULL: // NULL is never equal to and value. Use IsNull();
                {
                    return(false);
                }
                case T_STRING: 
                {
                    return(m_sValue < objValue.m_sValue);
                }
                case T_CHAR: 
                {
                    return(m_cValue < objValue.m_cValue);
                }
                case T_LONG: 
                {
                    return(m_lValue < objValue.m_lValue);
                }
                case T_DOUBLE: 
                {
                    return(m_fValue < objValue.m_fValue);
                }
                default: 
                    throw CException(-1, "%s(%ld): CValue::operator < (const CValue& objValue): Invalid value type: [%d].", __FILE__, __LINE__, m_emType);
            }
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::operator < (const CValue& objValue): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------------------------//
std::string CValue::ToString(std::string sNullValue) const
{
    int nLen;
    char szValue[32];
    switch(m_emType)
    {
    case T_NULL: 
        return(sNullValue);
    case T_STRING: 
        return(m_sValue);
    case T_CHAR: 
        return(std::string(1, m_cValue));
    case T_LONG: 
        nLen = snprintf(szValue, sizeof(szValue), "%ld", m_lValue);
        if(nLen >= sizeof(szValue)) throw CException(-1, "%s(%ld): CValue::ToString(std::string): Overflowed when trying to convert: %ld.", __FILE__, __LINE__, m_lValue);
        return(std::string(szValue));
    case T_DOUBLE: 
        nLen = snprintf(szValue, sizeof(szValue), "%lf", m_fValue);
        if(nLen >= sizeof(szValue)) throw CException(-1, "%s(%ld): CValue::ToString(std::string): Overflowed when trying to convert: %lf.", __FILE__, __LINE__, m_fValue);
        return(std::string(szValue));
    default: 
        throw CException(-1, "%s(%ld): CValue::ToString(std::string): Invalid type, can not make the conversion.", __FILE__, __LINE__);
    }
}

// 
char CValue::ToChar(char cNullValue, bool* bIsTruncated) const
{
    long lValue;
    switch(m_emType)
    {
    case T_NULL: 
        if(NULL != bIsTruncated) *bIsTruncated = false;
        return(cNullValue);
    case T_STRING: 
        if(m_sValue.empty()) return(cNullValue);
        else
        {
            if(NULL != bIsTruncated)
            {
                if(m_sValue.length() > 1) *bIsTruncated = true;
                else *bIsTruncated = false;
            }
            return(m_sValue.at(0));
        }
    case T_CHAR: 
        if(NULL != bIsTruncated) *bIsTruncated = false;
        return(m_cValue);
    case T_DOUBLE: 
        lValue = (long) m_fValue;
        if(lValue > 0 && lValue < 256)
        {
            *bIsTruncated = false;
            return((char)lValue);
        }else throw CException(-1, "%s(%ld): CValue::ToChar(char, bool*): Invalid double value: %lf, can not convert.", __FILE__, __LINE__, m_fValue);
    case T_LONG: 
        if(m_lValue > 0 && m_lValue < 256)
        {
            *bIsTruncated = false;
            return((char)m_lValue);
        }else throw CException(-1, "%s(%ld): CValue::ToChar(char, bool*): Invalid long value: %ld, can not convert.", __FILE__, __LINE__, m_lValue);
    default: 
        throw CException(-1, "%s(%ld): CValue::ToChar(char, bool*): Invalid type, can not make the conversion.", __FILE__, __LINE__);
    }
}

long CValue::ToLong(long lNullValue) const
{
    try
    {
        char    *pEnd;
        long    lValue;
        CRegularExpression objRegExp;
        switch(m_emType)
        {
        case T_NULL: 
            return(lNullValue);
        case T_STRING: // Convert value: 
            // 1. Check blank string: If empty treat as NULL.
            if(m_sValue.empty()) return(lNullValue);
            
            // 2. Check 0 value.
            if(objRegExp.IsMatch("^[+-]?0*$", m_sValue.c_str()))
            {
                // It's a 0 value: 
                return(0);
            }
            // Here I don't care about what is following after '.'
            if(objRegExp.IsMatch("^[+-]?0*\\..*$", m_sValue.c_str()))
            {
                // It's a 0 value: 
                return(0);
            }
    
            // 3. Convert value: 
            lValue = strtol(m_sValue.c_str(), &pEnd, 10);
            // TODO: errorno is not thread safe: 
            if(0 == lValue || '\0' != *pEnd)
            {
                // Ignore decimal part.
                if('.' != *pEnd)
                {
                    throw CException(-1, "%s(%ld): CValue::ToLong(long): Conversion failed: [%s], value maybe: [%ld], stopped at: [%c].", __FILE__, __LINE__, 
                        m_sValue.c_str(), lValue, *pEnd);
                }
            }
            return(lValue);
        case T_CHAR: 
            return((long)m_cValue);
        case T_LONG: 
            return(m_lValue);
        case T_DOUBLE: 
            return((long)m_fValue);
        default: 
            throw CException(-1, "%s(%ld): CValue::ToLong(long): Invalid type, can not make the conversion.", __FILE__, __LINE__);
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::ToLong(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
double CValue::ToDouble(double fNullValue) const
{
    try
    {
        char    *pEnd;
        double  fValue;
        CRegularExpression objRegExp;
        switch(m_emType) 
        {
        case T_NULL: 
            return(fNullValue);
        case T_STRING: // Convert value: 
            // 1. Check blank string: If empty treat as NULL.
            if(m_sValue.empty()) return(fNullValue);
            
            // 2. Check 0 value.
            if(objRegExp.IsMatch("^[+-]?0*\\.?0{1,}$", m_sValue.c_str())) 
            {
                // It's a 0 value: 
                return(0.0);
            }
            
            // 3. Convert value: 
            fValue = strtod(m_sValue.c_str(), &pEnd);
            // TODO: errorno is not thread safe: 
            if((fValue > -0.000000000000001 && fValue < 0.000000000000001) || '\0' != *pEnd)
            {
                throw CException(-1, "CValue::ToDouble(double): Conversion failed: [%s], value maybe: [%lf], stopped at: [%c].", __FILE__, __LINE__, 
                    m_sValue.c_str(), fValue, *pEnd);
            }
            return(fValue);
        case T_CHAR: 
            return((double)m_cValue);
        case T_LONG: 
            return((double)m_lValue);
        case T_DOUBLE: 
            return(m_fValue);
        default: 
            throw CException(-1, "%s(%ld): CValue::ToDouble(double): Invalid type, can not make the conversion.", __FILE__, __LINE__);
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::ToDouble(double): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------------------------//
std::string CValue::ToUpper() const
{
    try
    {
        std::string sValue;
        // 
        switch(m_emType) 
        {
        case T_NULL: 
            return(std::string());
        case T_STRING: // Convert value: 
            sValue = m_sValue;
            for(std::string::size_type i = 0; i < sValue.length(); ++i)
            {
                sValue[i] = toupper(sValue[i]);
            }
            return(sValue);
        case T_CHAR: 
            return(std::string(1, toupper(m_cValue)));
        case T_LONG: 
        case T_DOUBLE: 
            return(ToString());
        default: 
            throw CException(-1, "%s(%ld): CValue::ToUpper(): Invalid type, can not make the conversion.", __FILE__, __LINE__);
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::ToUpper(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
std::string CValue::ToLower() const
{
    try
    {
        std::string sValue;
        // 
        switch(m_emType) 
        {
        case T_NULL: 
            return(std::string());
        case T_STRING: // Convert value: 
            sValue = m_sValue;
            for(std::string::size_type i = 0; i < sValue.length(); ++i)
            {
                sValue[i] = tolower(sValue[i]);
            }
            return(sValue);
        case T_CHAR: 
            return(std::string(1, tolower(m_cValue)));
        case T_LONG: 
        case T_DOUBLE: 
            return(ToString());
        default: 
            throw CException(-1, "%s(%ld): CValue::ToUpper(): Invalid type, can not make the conversion.", __FILE__, __LINE__);
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::ToLower(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------------------------//
std::string CValue::LeftTrim(const char *szToken) const
{
    try
    {
        if(NULL == szToken)
        {
            throw CException(-1, "%s(%ld): CValue::LeftTrim(const char *): NULL pointer.", __FILE__, __LINE__);
        }
        // 
        std::string sValue = ToString();
        if(sValue.empty()) return(sValue);
        std::string::size_type i;
        for(i = 0; IsCharInString(sValue.at(i), szToken) && i < sValue.length(); ++i);
        return(sValue.substr(i));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::LeftTrim(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

std::string CValue::RightTrim(const char *szToken) const
{
    try
    {
        if(NULL == szToken)
        {
            throw CException(-1, "%s(%ld): CValue::RightTrim(const char *): NULL pointer.", __FILE__, __LINE__);
        }
        // 
        std::string sValue = ToString();
        if(sValue.empty()) return(sValue);
        std::string::size_type i;
        for(i = sValue.length() - 1; IsCharInString(sValue.at(i), szToken) && i >= 0; --i);
        return(sValue.substr(0, i + 1));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CValue::RightTrim(const char *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

std::string CValue::AllTrim(const char *szToken) const
{
    try
    {
        CValue objValue(RightTrim(szToken));
        return(objValue.LeftTrim(szToken));
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CValue::AllTrim(const char *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
