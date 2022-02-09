/* =========================================================================== /
* @category     Application
* @package      Application
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CVALUE_H
#define _CVALUE_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>

//------------------------------------------------------------------------------
// CValue 1.0
//------------------------------------------------------------------------------
// Constants and type: 
typedef enum emValueType
{
    T_NULL,     // 
    T_STRING,   // 
    T_CHAR,     // 
    T_LONG,     // 
    T_DOUBLE,   // 
} tValueType;

// 
class CValue
{
private: 
    tValueType      m_emType;
    std::string     m_sValue;
    char            m_cValue;
    long            m_lValue;
    double          m_fValue;
// 
    bool            IsCharInString(const char cChar, const char *szString, const int nMaxLen, const bool bIsCaseSensitive = true) const;
    bool            IsCharInString(const char cChar, const char *szString, const bool bIsCaseSensitive = true) const;
public: 
// 
    CValue();
    CValue(const CValue& objValue);
    // 
    CValue(const char* pValue, int nLen);
    CValue(const std::string sValue);
    CValue(const char cValue);
    CValue(const long lValue);
    CValue(const double fValue);
    // 
    ~CValue();
// 
    void            SetNull();
    bool            IsNull() const;
    // 
    void            SetValue(const char* pValue, const int nLen);
    void            SetValue(const std::string sValue);
    void            SetValue(const char cValue);
    void            SetValue(const long lValue);
    void            SetValue(const double fValue);
// 
    tValueType      GetType() const;
// 
    const CValue&   operator = (const CValue& objValue);
    const CValue&   operator = (std::string sValue);
    const CValue&   operator = (char cValue);
    const CValue&   operator = (long lValue);
    const CValue&   operator = (double fValue);
//
    bool            operator == (const CValue& objValue) const;
    bool            operator <  (const CValue& objValue) const;
// 
    // If string is empty, return default value.
    std::string     ToString(std::string sNullValue = "") const;
    char            ToChar(char cNullValue = '\0', bool* bIsTruncated = NULL) const;
    long            ToLong(long lNullValue = 0) const;
    double          ToDouble(double fNullValue = 0.0) const;
// 
    std::string     ToLower() const;
    std::string     ToUpper() const;
// 
    std::string     LeftTrim(const char *szToken = " \t") const;
    std::string     RightTrim(const char *szToken = " \t") const;
    std::string     AllTrim(const char *szToken = " \t") const;
};

#endif
