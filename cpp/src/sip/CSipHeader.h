/* =========================================================================== /
* @category         SIP Header
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CSipHeader.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CSIPHEADER_H
#define _CSIPHEADER_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include "CNameValuePair.h"

//---------------------------------------------------------------------------//
// Constants: 
//  Message Length: 
const size_t    DEF_SIP_HEADER_LEN          = 512;

//  Line terminator: 
const char SIP_HEADER_SEPARATER[]           = ":";       // 

// =========================================================================== /
// CSipHeader 1.0
// =========================================================================== /
class CSipHeader : public CNameValuePair 
{
protected: 
        int             m_nLine;                        // Which line is this header in. 
        std::string     m_sNameStandard;                // Standard name of this header. 
public: 
    explicit CSipHeader(int nSize = DEF_SIP_HEADER_LEN);
    CSipHeader(const CSipHeader& objSipHeader);
    ~CSipHeader();
// 
    virtual void Clear();
    virtual bool Parse();
    virtual void PrintStructure() const;
// 
    virtual const   CSipHeader& operator = (const CSipHeader& objSipHeader);
    bool            operator == (const CSipHeader& objSipHeader) const;
// 
    bool            IsNameMatch(const CSipHeader& objSipHeader) const;
// 
    bool            GetNameStandard(std::string &sNameStandard) const;
// 
    inline 
    const std::string&  GetNameStandard() const
    {
        return(m_sNameStandard);
    }
};

#endif
