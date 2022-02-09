/* =========================================================================== /
* @category         Exception
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
// 
#include <stdio.h>
#include "CException.h"

// =========================================================================== /
// CException 1.0
// =========================================================================== /
void CException::PushMessage(const char *szFormat, va_list stParameter) throw()
{
    // 1. Sanity check: 
    if(NULL == szFormat) return;    // Do not throw here.
    if(NULL == stParameter)
    {
        m_listMessage.push_back(static_cast<std::string>(szFormat));
    }else
    {
        char szTemp[4*1024];
        vsnprintf(szTemp, sizeof(szTemp), szFormat, stParameter);
        m_listMessage.push_back(static_cast<std::string>(szTemp));
    }
}

//----------------------------------------------------------------------------//
CException::CException(int nCode, const char *szFormat, ...) throw()
{
    //1. Set error code: 
    SetCode(nCode);

    //2. Set error messages: 
    va_list stParameter;
    va_start(stParameter, szFormat);
    PushMessage(szFormat, stParameter);
    va_end(stParameter);
}

CException::~CException() throw()
{
}

//----------------------------------------------------------------------------//
void CException::SetCode(int nCode) throw()
{
    m_nCode = nCode;
}

void CException::PushMessage(const char *szFormat, ...) throw()
{
    // 1. Set error messages: 
    va_list stParameter;
    va_start(stParameter, szFormat);
    PushMessage(szFormat, stParameter);
    va_end(stParameter);
}

//----------------------------------------------------------------------------//
// 
int CException::GetCode() const throw()
{
    return(m_nCode);
}

std::string CException::GetMessageStack() const throw()
{
    // 
    std::string sStack;
    // Code: 
    char szTemp[32];
    sprintf(szTemp, "Error Code: [%ld]\r\n", GetCode());
    sStack += szTemp;
    // Message stack: 
    int nLine = 1;
    for(std::list<std::string>::const_iterator i = m_listMessage.begin(); i != m_listMessage.end(); ++i)
    {
        sprintf(szTemp, "%02d> ", nLine);
        sStack = sStack + szTemp + "[" + *i + "]\r\n";
        nLine++;
    }
    return(sStack);
}

//----------------------------------------------------------------------------//
const char* CException::what() const throw()
{
    std::string sStack = GetMessageStack();
    return(sStack.c_str());
}
