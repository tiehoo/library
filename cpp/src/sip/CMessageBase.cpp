/* =========================================================================== /
* @category     Message
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
// 
#include <stdio.h>
#include "CException.h"
#include "CMessageBase.h"

/* --------------------------------------------------------------------------- /
// CMessageBase 1.0
/ --------------------------------------------------------------------------- */
CMessageBase::CMessageBase(long nSize) : CBuffer(nSize)
{
    SetInvalid();
}

CMessageBase::CMessageBase(const char* szBuffer) : CBuffer(szBuffer)
{
    SetInvalid();
}

CMessageBase::CMessageBase(const CMessageBase& objMessageBase):CBuffer(objMessageBase.GetSize())
{
    try
    {
        operator = (objMessageBase);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CMessageBase::CMessageBase(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CMessageBase::CMessageBase(const char* pBuffer, long nSize) : CBuffer(pBuffer, nSize)
{
    SetInvalid();
}

CMessageBase::~CMessageBase()
{
}

//----------------------------------------------------------------------------//
void CMessageBase::Clear()
{
    try
    {
        // 
        CBuffer::Clear();
        // 
        m_bIsValid = false;
        m_strError.clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CMessageBase::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMessageBase::PrintStructure() const 
{
    printf("IsValid: [%d]\n", m_bIsValid);
    printf("Error: [%s]\n\n", GetError());
}

//----------------------------------------------------------------------------//
const CMessageBase& CMessageBase::operator = (const CMessageBase& objMessageBase) 
{
    try
    {
        // 
        CBuffer::operator = (objMessageBase);
        // 
        m_bIsValid = objMessageBase.m_bIsValid;
        m_strError = objMessageBase.m_strError;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CMessageBase::operator = (): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
void CMessageBase::SetInvalid(const char *szFormat, ...) 
{
    m_bIsValid = false;
    if(NULL != szFormat)
    {
        va_list stParameter;
        va_start(stParameter, szFormat);
        m_strError += "<";
        if(NULL == stParameter) 
        {
            m_strError += szFormat;
        }else 
        {
            char szTemp[4*1024];
            vsnprintf(szTemp, sizeof(szTemp), szFormat, stParameter);
            m_strError += szTemp;
        }
        m_strError += ">";
        va_end(stParameter);
    }
}

// 
const char* CMessageBase::GetError() const
{
    return(m_strError.c_str());
}
