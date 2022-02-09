/* =========================================================================== /
* @category     SIP Message
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
#include <algorithm>
#include "CException.h"
#include "CBufferAnalyzer.h"
#include "CSipHeader.h"
#include "SipDefinition.h"

/* --------------------------------------------------------------------------- /
// CSipHeader 2.0
/ --------------------------------------------------------------------------- */
CSipHeader::CSipHeader(int nSize):CNameValuePair(nSize)
{
}

CSipHeader::CSipHeader(const CSipHeader& objSipHeader):CNameValuePair(objSipHeader.GetSize()) 
{
    try
    {
        operator = (objSipHeader);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipHeader::CSipHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CSipHeader::~CSipHeader()
{
}

void CSipHeader::Clear()
{
    try
    {
        // 
        CNameValuePair::Clear();
        // 
        m_sNameStandard.clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipHeader::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSipHeader::PrintStructure() const 
{
    CNameValuePair::PrintStructure();
    // 
    printf("Standard Name: [%s]", m_sNameStandard.c_str());
    printf("\n\n");
}


// 
const CSipHeader& CSipHeader::operator = (const CSipHeader& objSipHeader)
{
    try
    {
        // 
        CNameValuePair::operator = (objSipHeader);
        // 
        m_sNameStandard = objSipHeader.m_sNameStandard;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CNameValuePair::operator = (): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipHeader::operator == (const CSipHeader& objSipHeader) const
{
    if(!IsValid() || !objSipHeader.IsValid())
    {
        return(false);
    }
    if(!IsNameMatch(objSipHeader)) 
    {
        return(false);
    }
    if(m_pairValue.second != objSipHeader.m_pairValue.second)
    {
        return(false);
    }
    if(0 != strncmp(GetBufferPointer(m_pairValue.first), objSipHeader.GetBufferPointer(objSipHeader.m_pairValue.first), m_pairValue.second))
    {
        return(false);
    }
    return(true);
}

// 
bool CSipHeader::Parse() 
{
    try
    {
        // 
        if(CNameValuePair::Parse(":")) 
        {
            // 
            char cChar;
            // Standardlize name: 
            if(1 == m_pairName.second) 
            {
                // Shot header name: 
                if(!GetChar(m_pairName.first, cChar))
                {
                    throw CException(-1, "%s(%ld): CSipHeader::Parse(): Internal error, can not get a char from buffer[%ld].", __FILE__, __LINE__, 
                        m_pairName.first);
                }
                if(toupper(cChar) == toupper(SIP_HEADER_FROM_SHORT[0]))
                {
                    // From: 
                    m_sNameStandard = SIP_HEADER_FROM_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_TO_SHORT[0]))
                {
                    // To: 
                    m_sNameStandard = SIP_HEADER_TO_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_CALL_ID_SHORT[0]))
                {
                    // Call-ID: 
                    m_sNameStandard = SIP_HEADER_CALL_ID_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_VIA_SHORT[0]))
                {
                    // Via: 
                    m_sNameStandard = SIP_HEADER_VIA_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_CONTENT_TYPE_SHORT[0]))
                {
                    // Content-Type: 
                    m_sNameStandard = SIP_HEADER_CONTENT_TYPE_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_CONTENT_LENGTH_SHORT[0]))
                {
                    // Content-Length: 
                    m_sNameStandard = SIP_HEADER_CONTENT_LENGTH_STANDARD;
                }else if(toupper(cChar) == toupper(SIP_HEADER_CONTACT_SHORT[0]))
                {
                    // Contact: 
                    m_sNameStandard = SIP_HEADER_CONTACT_STANDARD;
                }else
                {
                    // Get an unknown short name: Just convert to upper case: 
                    m_sNameStandard = toupper(cChar);
                }
            }else 
            {
                // Normal hader name: Convert to upper case. 
                if(GetName(m_sNameStandard))
                {
                    std::transform(m_sNameStandard.begin(), m_sNameStandard.end(), m_sNameStandard.begin(), ::toupper);
                }else
                {
                    throw CException(-1, "%s(%ld): CSipHeader::Parse(): Internal error, can not get name from header.", __FILE__, __LINE__);
                }
            }
            // 
            SetValid();
        }else 
        {
            // CNameValuePair parse failed. 
        }
        return(IsValid());
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipHeader::Parse(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipHeader::IsNameMatch(const CSipHeader& objSipHeader) const
{
    if(IsValid() && objSipHeader.IsValid())
    {
        return(m_sNameStandard == objSipHeader.m_sNameStandard);
    }else 
    {
        return(false);
    }
}

// 
bool CSipHeader::GetNameStandard(std::string &sNameStandard) const
{
    if(m_sNameStandard.empty())
    {
        return(false);
    }else
    {
        sNameStandard = m_sNameStandard;
        return(true);
    }
}
