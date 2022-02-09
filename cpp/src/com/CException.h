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
#ifndef _CEXCEPTION_H
#define _CEXCEPTION_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <string>
#include <list>
#include <stdarg.h>

// =========================================================================== /
// CException 1.0
// =========================================================================== /
class CException : public std::exception
{
private: 
    int                     m_nCode;        // Error code, only one final code.
    // 
    std::list<std::string>  m_listMessage;  // Message list.
    // 
    void                    PushMessage(const char *szFormat, va_list stParameter) throw();
public: 
    CException(int nCode = 0, const char* szFormat = NULL, ...) throw();
    ~CException() throw();

    void                    SetCode(int nCode) throw();
    void                    PushMessage(const char *szFormat = NULL, ...) throw();

    int                     GetCode() const throw();
    std::string             GetMessageStack() const throw();

    // Compatible with std::exception
    virtual const char*     what() const throw();
};

#endif
