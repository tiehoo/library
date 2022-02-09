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
#include "CSipStartLine.h"

/* --------------------------------------------------------------------------- /
// CSipStartLine 2.0
/ --------------------------------------------------------------------------- */
CSipStartLine::CSipStartLine(long nSize):CMessageBase(nSize)
{
    try
    {
        Clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::CSipStartLine(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CSipStartLine::CSipStartLine(const CSipStartLine& objSipStartLine):CMessageBase(objSipStartLine.GetSize()) 
{
    try
    {
        operator = (objSipStartLine);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::CSipStartLine(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CSipStartLine::~CSipStartLine()
{
}

//----------------------------------------------------------------------------//
void CSipStartLine::Clear()
{
    try
    {
        // 
        CMessageBase::Clear();
        // 
        m_cType = '\0';
        // 
        m_pairMethod.first    = 0;
        m_pairMethod.second   = 0;
        // 
        m_pairRequestUri.first   = 0;
        m_pairRequestUri.second  = 0;
        // 
        m_pairStatusCode.first   = 0;
        m_pairStatusCode.second  = 0;
        // 
        m_pairReasonPhrase.first   = 0;
        m_pairReasonPhrase.second  = 0;
        // 
        m_pairSipVersion.first  = 0;
        m_pairSipVersion.second = 0;
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CSipStartLine::PrintStructure() const 
{
    CMessageBase::PrintStructure();
    // 
    printf("Type: [%c]\n", m_cType);
    // 
    printf("Method(%d, %d)", m_pairMethod.first, m_pairMethod.second);
    Print(m_pairMethod.first, m_pairMethod.second);
    printf("\n");
    // 
    printf("RequestUri(%d, %d)", m_pairRequestUri.first, m_pairRequestUri.second);
    Print(m_pairRequestUri.first, m_pairRequestUri.second);
    printf("\n");
    // 
    printf("StatusCode(%d, %d)", m_pairStatusCode.first, m_pairStatusCode.second);
    Print(m_pairStatusCode.first, m_pairStatusCode.second);
    printf("\n");
    // 
    printf("ReasonPhrase(%d, %d)", m_pairReasonPhrase.first, m_pairReasonPhrase.second);
    Print(m_pairReasonPhrase.first, m_pairReasonPhrase.second);
    printf("\n");
    // 
    printf("SipVersion(%d, %d)", m_pairSipVersion.first, m_pairSipVersion.second);
    Print(m_pairSipVersion.first, m_pairSipVersion.second);
    printf("\n");
    printf("\n");
}

//----------------------------------------------------------------------------//
const CSipStartLine& CSipStartLine::operator = (const CSipStartLine& objSipStartLine)
{
    try
    {
        // 
        CMessageBase::operator = (objSipStartLine);
        // 
        m_cType             = objSipStartLine.m_cType;
        // 
        m_pairMethod        = objSipStartLine.m_pairMethod;
        m_pairRequestUri    = objSipStartLine.m_pairRequestUri;
        m_pairStatusCode    = objSipStartLine.m_pairStatusCode;
        m_pairReasonPhrase  = objSipStartLine.m_pairReasonPhrase;
        m_pairSipVersion    = objSipStartLine.m_pairSipVersion;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::operator = (): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
bool CSipStartLine::ParseSipVersion(CBufferAnalyzer& objAnalyzer)
{
    try
    {
        // 
        long lOffset, lSkipLen, lSkipBackLen;
        char cTokenUsed;
        // SIP version: SIP/2.0
        //  Find / 
        lSkipLen = 0;
        if(objAnalyzer.FindChar(SIP_VERSION_SEPARATOR, cTokenUsed, lSkipLen)) 
        {
            if(4 == lSkipLen)
            {
                // Found /
                objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                // Find second space: 
                m_pairSipVersion.first = objAnalyzer.GetOffset();
                if(objAnalyzer.FindChar(SIP_DEF_CHARSET_SPACE, cTokenUsed, lSkipLen))
                {
                    // Status line SIP version: 
                    m_pairSipVersion.second = lSkipLen - 1;
                }else 
                {
                    // Can not find a space, here must be the end of the buffer. 
                    // Request line SIP version: 
                    m_pairSipVersion.second = lSkipLen;
                }
                return(true);
            }else 
            {
                SetInvalid("CSipStartLine: Status line, invalid chars between SIP and /.");
                return(false);
            }
        }else 
        {
            SetInvalid("CSipStartLine: Status line, can not find [/].");
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::Parse(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipStartLine::Parse() 
{
    try
    {
        // 
        long lOffset, lSkipLen, lSkipBackLen;
        char cTokenUsed;
        // 
        CBufferAnalyzer objAnalyzer(*this);
        objAnalyzer.Rewind();

        // Find first space: 
        //  Skip preceding space: 
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        if(!objAnalyzer.IsEnd()) 
        {
            // Record pointer: 
            lOffset = objAnalyzer.GetOffset();
            // Find first space: 
            if(objAnalyzer.FindChar(SIP_DEF_CHARSET_SPACE, cTokenUsed, lSkipLen))
            {
                // Found first space: Determine type. 
                if(0 == strncasecmp(GetBufferPointer(lOffset), SIP_VERSION_TAG, strlen(SIP_VERSION_TAG))) 
                {
                // 1. Status line: SIP/2.0 100 Stop retransmission..
                    objAnalyzer.SetOffset(lOffset);
                    if(!ParseSipVersion(objAnalyzer))
                    {
                        SetInvalid("CSipStartLine: Invalid SIP version");
                        return(false);
                    }
                    // Status-Code: 
                    objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                    m_pairStatusCode.first = objAnalyzer.GetOffset();
                    lSkipLen = 0;
                    if(objAnalyzer.FindChar(SIP_DEF_CHARSET_SPACE, cTokenUsed, lSkipLen))
                    {
                        m_pairStatusCode.second = lSkipLen - 1;
                        // Reason-Phrase: 
                        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                        m_pairReasonPhrase.first    = objAnalyzer.GetOffset();
                        lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairReasonPhrase.first + lSkipLen - 1);
                        m_pairReasonPhrase.second   = GetLength() - m_pairReasonPhrase.first - lSkipBackLen;
                    }else 
                    {
                        if(lSkipLen > 0) 
                        {
                            // Reached end of the buffer, no reason phase. 
                            m_pairStatusCode.second = lSkipLen;
                        }else 
                        {
                            // 
                            SetInvalid("CSipStartLine: Status line no status code");
                            return(false);
                        }
                    }
                    // 
                    m_cType = SIP_LINE_TYPE_STATUS;
                }else 
                {
                // 2. Request Line: INVITE sip:17187773456@209.47.41.30:80;transport=udp SIP/2.0..
                    // Method: 
                    m_pairMethod.first  = lOffset;
                    m_pairMethod.second = lSkipLen - 1;
                    // Request-URI
                    objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                    m_pairRequestUri.first  = objAnalyzer.GetOffset();
                    if(objAnalyzer.FindChar(SIP_DEF_CHARSET_SPACE, cTokenUsed, lSkipLen))
                    {
                        m_pairRequestUri.second = lSkipLen - 1;
                        // SIP version: 
                        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
                        if(0 == strncasecmp(GetBufferPointer(objAnalyzer.GetOffset()), SIP_VERSION_TAG, strlen(SIP_VERSION_TAG))) 
                        {
                            if(!ParseSipVersion(objAnalyzer))
                            {
                                SetInvalid("CSipStartLine: Invalid SIP version");
                                return(false);
                            }
                        }else 
                        {
                            // Can not find SIP version: 
                            SetInvalid("CSipStartLine: Request line no SIP version found");
                            return(false);
                        }
                    }else 
                    {
                        // Can not find SIP version: 
                        SetInvalid("CSipStartLine: Request line no SP between Request-URI and SIP-Version found");
                        return(false);
                    }
                    // 
                    m_cType = SIP_LINE_TYPE_REQUEST;
                }
            }else 
            {
                // Can not find first space: 
                SetInvalid("CSipStartLine: No first space found");
                return(false);
            }
        }else 
        {
            // All space: [     ]
            SetInvalid("CSipStartLine: All space");
            return(false);
        }
        // 
        SetValid();
        return(true);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::Parse(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
char CSipStartLine::GetType() const
{
    return(m_cType);
}

// 
bool CSipStartLine::IsStatusLine() const
{
    if(m_cType == SIP_LINE_TYPE_STATUS) 
    {
        return(true);
    }else 
    {
        return(false);
    }
}

// 
bool CSipStartLine::IsRequestLine() const
{
    if(m_cType == SIP_LINE_TYPE_REQUEST) 
    {
        return(true);
    }else 
    {
        return(false);
    }
}

// 
bool CSipStartLine::GetSipVersion(std::string& sSipVersion) const
{
    try
    {
        if(m_pairSipVersion.second > 0)
        {
            Read(m_pairSipVersion.first, m_pairSipVersion.second, sSipVersion);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetSipVersion(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipStartLine::GetMethod(std::string& sMethod) const
{
    try
    {
        if(m_pairMethod.second > 0)
        {
            Read(m_pairMethod.first, m_pairMethod.second, sMethod);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetMethod(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipStartLine::GetRequestUri(std::string& sRequestUri) const
{
    try
    {
        if(m_pairRequestUri.second > 0)
        {
            Read(m_pairRequestUri.first, m_pairRequestUri.second, sRequestUri);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetRequestUri(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipStartLine::GetRequestUri(CUri& objUri) const
{
    try
    {
        if(m_pairRequestUri.second > 0)
        {
            objUri.SetBuffer(GetBufferPointer() + m_pairRequestUri.first, m_pairRequestUri.second);
            return(true);
        }else 
        {
            return(false);
        }
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetRequestUri(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipStartLine::GetStatusCode(std::string& sStatusCode) const
{
    try
    {
        if(m_pairStatusCode.second > 0)
        {
            Read(m_pairStatusCode.first, m_pairStatusCode.second, sStatusCode);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetStatusCode(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipStartLine::GetReasonPhrase(std::string& sReasonPhrase) const
{
    try
    {
        if(m_pairReasonPhrase.second > 0)
        {
            Read(m_pairReasonPhrase.first, m_pairReasonPhrase.second, sReasonPhrase);
            return(true);
        }else
        {
            return(false);
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipStartLine::GetReasonPhrase(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
