/* =========================================================================== /
* @category         SIP Message
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CSIPSTARTLINE_H
#define _CSIPSTARTLINE_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CBufferAnalyzer.h"
#include "CMessageBase.h"
#include "CUri.h"

//---------------------------------------------------------------------------//
//Constants: 
//  Message Length: 
const long  DEF_SIP_STARTLINE_LEN   = 512;

//  Line type: 
const char SIP_LINE_TYPE_REQUEST    = 'R';
const char SIP_LINE_TYPE_STATUS     = 'S';

//  SIP version: 
const char SIP_VERSION_TAG[]        = "SIP";
const char SIP_VERSION_SEPARATOR[]  = "/";

// =========================================================================== /
// CSipStartLine 1.0 
// =========================================================================== /
/* 
    start-line       =  Request-Line / Status-Line
    Request-Line     =  Method SP Request-URI SP SIP-Version CRLF
    Status-Line      =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
    
    Example: 
    INVITE sip:17187773456@209.47.41.30:80;transport=udp SIP/2.0..
    SIP/2.0 100 Stop retransmission..
*/
class CSipStartLine : public CMessageBase 
{
protected: 
    char                    m_cType;                // Line type. 
    // Request-Line: 
    std::pair<int, int>     m_pairMethod;
    std::pair<int, int>     m_pairRequestUri;
    // Status-Line: 
    std::pair<int, int>     m_pairStatusCode;
    std::pair<int, int>     m_pairReasonPhrase;
    // 
    std::pair<int, int>     m_pairSipVersion;
public: 
// 
    explicit CSipStartLine(long nSize = DEF_SIP_STARTLINE_LEN);
    CSipStartLine(const CSipStartLine& objStartLine);
    ~CSipStartLine();
// 
    virtual void Clear();
    virtual bool Parse();
            bool ParseSipVersion(CBufferAnalyzer& objAnalyzer);
    virtual void PrintStructure() const;
// 
    virtual const CSipStartLine& operator = (const CSipStartLine& objSipStartLine);
// 
    // Type: 
    char    GetType() const;                              // Get line type, SIP_LINE_TYPE_REQUEST, SIP_LINE_TYPE_STATUS.
    bool    IsStatusLine() const;
    bool    IsRequestLine() const;
    // 
    bool    GetSipVersion(std::string& sSipVersion) const;
    // 
    bool    GetMethod(std::string& sMethod) const;
    bool    GetRequestUri(std::string& sRequestUri) const;
    bool    GetRequestUri(CUri& objUri) const;
    // 
    bool    GetStatusCode(std::string& sStatusCode) const;
    bool    GetReasonPhrase(std::string& sReasonPhrase) const;
};

#endif
