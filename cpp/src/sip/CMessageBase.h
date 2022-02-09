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
#ifndef _CMESSAGEBASE_H
#define _CMESSAGEBASE_H

/* =========================================================================== /
*  Place includes, constant defines here.
/============================================================================ */
#include <string>
#include "CBuffer.h"

//Constants: 
const long  DEF_MESSAGE_SIZE    = 512;   // Default message length.

// =========================================================================== /
// CMessageBase 1.0
// =========================================================================== /
// CMessageBase is a CBuffer with general message information.
class CMessageBase : public CBuffer
{
protected: 
    // 
    bool                m_bIsValid;             // Is message valid.
    std::string         m_strError;             // Parser error.
public: 
// 
    explicit CMessageBase(long nSize = DEF_MESSAGE_SIZE);
    explicit CMessageBase(const CMessageBase& objMessageBase);
    explicit CMessageBase(const char* szBuffer);
             CMessageBase(const char* pBuffer, long nSize);
    ~CMessageBase();
// 
    virtual void        Clear();
    virtual void        PrintStructure() const;
// 
    // Need to copy buffer, and duplicate valid flag.
    virtual const CMessageBase& operator = (const CMessageBase& objMessageBase);
// 
    // 
    inline 
    void                SetValid()
    {
        m_bIsValid = true;
    }
    void                SetInvalid(const char *szFormat = NULL, ...);
    // 
    inline 
    bool                IsValid() const
    {
        return(m_bIsValid);
    }
    const char*         GetError() const;
};

#endif
