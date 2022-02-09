/* =========================================================================== /
* @category         SIP Message
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CSipMessage.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CSIPMESSAGE_H
#define _CSIPMESSAGE_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <utility>
#include <map>
#include <list>
// 
#include "CMessageBase.h"
#include "CSipHeader.h"
#include "CSipStartLine.h"

//---------------------------------------------------------------------------//
// Constants: 
//  Message Length: 
const int       MAX_SIP_MESSAGE_LEN             = 4*1024;       // 

//  Message type: 
const char      SIP_MESSAGE_TYPE_REQUEST        = 'Q';          // 
const char      SIP_MESSAGE_TYPE_RESPONSE       = 'P';          // 

//  Line terminator: 
const char SIP_MESSAGE_LINE_TERMINATOR[]        = "\r\n";       // 
/*
const char SIP_HEADER_TERMINATOR[]              = "\r\n\r\n";   // 
*/

// =========================================================================== /
// CSipMessage 1.0 
// =========================================================================== /
// CSipMessage is a CBufferAnalyzer include a start line(CMessageLine) a header list(CMessageField) a body(CMessageBase)
/*
   Both types 
   of messages consist of a start-line, one or more header fields, an 
   empty line indicating the end of the header fields, and an optional 
   message-body.
         generic-message  =  start-line
                             *message-header
                             CRLF
                             [ message-body ]
         start-line       =  Request-Line / Status-Line
         
         Request-Line     =  Method SP Request-URI SP SIP-Version CRLF
         Status-Line      =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*/

class CSipMessage : public CMessageBase 
{
protected: 
    // Parts pointer: < offset, length >
    std::pair<int, int>                                 m_pairStartLine;
    std::pair<int, int>                                 m_pairHeader;
    std::pair<int, int>                                 m_pairBody;
    // Line number to header map: < line, <start, len> > 
    std::map< int, std::pair<int, int> >                m_mapHeaderLine;
    // Name to line number map: < name, line >
    std::multimap< std::string, int >                   m_mmapHeaderName;
    // 
/*
    void    RemoveHeader(int nLine, CSipHeader& objSipHeader);
*/
    int     GetTopLine(const std::string sSipHeaderNameStandard);
public: 
//---------------------------------------------------------------------------//
    explicit CSipMessage(int nSize = MAX_SIP_MESSAGE_LEN);
    CSipMessage(const CSipMessage& objSipMessage);
    // 
    ~CSipMessage();
//---------------------------------------------------------------------------//
// 
    virtual void Clear();
    virtual void PrintStructure() const;
// 
    const   CSipMessage& operator = (const CSipMessage& objSipMessage);
//---------------------------------------------------------------------------//
    bool    Parse();
// 
    bool    GetStartLine(CSipStartLine& objSipStartLine);
// 
    // If more than one headers with the name in the message, this will return the first one with lower line number. 
    bool    GetHeader(const std::string sSipHeaderNameStandard, CSipHeader& objSipHeader);
    // If nAtLine <= 0, insert as last line. 
    void    InsertHeader(const CSipHeader& objSipHeader, int nAtLine = -1);
    void    PushHeader(const CSipHeader& objSipHeader);
    bool    PopHeader(const std::string sSipHeaderNameStandard, CSipHeader& objSipHeader);
    
//---------------------------------------------------------------------------//
/*
    // 
    bool    IsValid();
    bool    IsParsed();
    // Update new pointer location: 
    void    SynchronizePointer(const char* szStart, int nLength);
//---------------------------------------------------------------------------//
    // Start line: 
    void    SetStartLine(const char* szSipStartLine);
    void    SetStartLine(const CSipStartLine& objSipStartLine);
    bool    GetStartLine(CSipStartLine& objSipStartLine);
    // Header: 
    void    SetHeader(const char* szSipHeader);
    void    SetHeader(const CSipHeader& objHeader);
    // 
    bool    RemoveHeader(const char* szSipHeaderName);
    // If more than one headers with the name in the message, this will return the first one with lower line number.
    bool    GetHeader(const char* szSipHeaderName, CSipHeader& objSipHeader);
    // Stacked header: 
    void    PushHeader(const CSipHeader& objHeader);
    bool    PopHeader(const char* szSipHeaderName, CSipHeader& objSipHeader);
    bool    PopHeader(const char* szSipHeaderName);
    // Body: 
    void    SetBody(const char* szSipBody);
    void    SetBody(const CSipBody& objSipBody);
    bool    GetBody(CSipBody& objSipBody);
*/
};

#endif
