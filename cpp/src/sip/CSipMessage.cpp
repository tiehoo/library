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
#include "CException.h"
#include "CBufferAnalyzer.h"
#include "CSipMessage.h"

/* --------------------------------------------------------------------------- /
// CSipMessage 2.0
/ --------------------------------------------------------------------------- */
CSipMessage::CSipMessage(int nSize):CMessageBase(nSize)
{
}

CSipMessage::CSipMessage(const CSipMessage& objSipMessage):CMessageBase(objSipMessage.GetSize()) 
{
    try
    {
        operator = (objSipMessage);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipMessage::CSipMessage(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CSipMessage::~CSipMessage()
{
}

void CSipMessage::Clear()
{
    try
    {
        // 
        CMessageBase::Clear();
        // 
        m_pairStartLine.first   = 0;
        m_pairStartLine.second  = 0;
        
        m_pairHeader.first      = 0;
        m_pairHeader.second     = 0;
        
        m_pairBody.first        = 0;
        m_pairBody.second       = 0;
        
        m_mapHeaderLine.clear();
        m_mmapHeaderName.clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipMessage::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}


void CSipMessage::PrintStructure() const 
{
    CMessageBase::PrintStructure();
    // 
    printf("[Start Line: %d, %d]\n", m_pairStartLine.first, m_pairStartLine.second);
    Print(m_pairStartLine.first, m_pairStartLine.second);
    printf("\n\n");
    // 
    printf("[Header: %d, %d]\n", m_pairHeader.first, m_pairHeader.second);
    Print(m_pairHeader.first, m_pairHeader.second);
    printf("\n\n");
    // 
    printf("[Body: %d, %d]\n", m_pairBody.first, m_pairBody.second);
    Print(m_pairBody.first, m_pairBody.second);
    printf("\n\n");
    // 
    printf("[Header List(Line)]\n");
    std::map< int, std::pair<int, int> >::const_iterator i;
    for(i = m_mapHeaderLine.begin(); i != m_mapHeaderLine.end(); ++i) 
    {
        printf("Line: %02d(%04d, %03d): ", (*i).first, (*i).second.first, (*i).second.second);
        Print((*i).second.first, (*i).second.second);
        printf("\n");
    }
    // 
    printf("\n[Header List(Name)]\n");
    std::multimap< std::string, int >::const_iterator j;
    for(j = m_mmapHeaderName.begin(); j != m_mmapHeaderName.end(); ++j) 
    {
        printf("Header: %s: %02d\n", (*j).first.c_str(), (*j).second);
    }
}

// 
const CSipMessage& CSipMessage::operator = (const CSipMessage& objSipMessage)
{
    try
    {
        // 
        CBuffer::SetBuffer(objSipMessage);
        // 
        m_pairStartLine     = objSipMessage.m_pairStartLine;
        m_pairHeader        = objSipMessage.m_pairHeader;
        m_pairBody          = objSipMessage.m_pairBody;
        // 
        m_mapHeaderLine     = objSipMessage.m_mapHeaderLine;
        m_mmapHeaderName    = objSipMessage.m_mmapHeaderName;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CMessageBase::operator = (): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipMessage::Parse() 
{
    try
    {
        long lOffset, lSkipLen, lHeaderLen, lOffsetHeader, nLine;
        // 
        CBufferAnalyzer objAnalyzer(*this);
        objAnalyzer.Rewind();

        // 1. Start line: 
        lOffset     = objAnalyzer.GetOffset();
        lSkipLen    = 0;
        if(!objAnalyzer.FindSequence(SIP_MESSAGE_LINE_TERMINATOR, lSkipLen)) 
        {
            // Can not find firstline terminator: 
            SetInvalid("CSipMessage:: No start line");
            return(false);
        }else
        {
            m_pairStartLine.first   = lOffset;
            m_pairStartLine.second  = lSkipLen;
        }

        // 2. Headers: 
        bool bDidFind;
        lOffset         = objAnalyzer.GetOffset();
        lSkipLen        = 0;
        lHeaderLen      = 0;
        nLine           = 1;
        CSipHeader      objHeader;
        // 
        while(1)
        {
            objHeader.Clear();
            lOffsetHeader   = objAnalyzer.GetOffset();
            bDidFind = objAnalyzer.FindSequence(SIP_MESSAGE_LINE_TERMINATOR, lSkipLen);
            // 
            if(lSkipLen > 2)
            {
                m_mapHeaderLine.insert(std::pair< int, std::pair<int, int> >( nLine, std::pair<int, int>(lOffsetHeader, lSkipLen)));
                // 
                objHeader.SetBuffer(GetBufferPointer() + lOffsetHeader, lSkipLen);
                if(!objHeader.Parse())
                {
                    SetInvalid("CSipMessage: Invalid header");
                    return(false);
                }
                m_mmapHeaderName.insert(std::pair< std::string, int >(objHeader.GetNameStandard(), nLine));
            }
            // 
            lHeaderLen += lSkipLen;
            if(!bDidFind) 
            {
                // Can not find \r\n: 
                break;
            }
            if(2 == lSkipLen)
            {
                // Found \r\n\r\n
                break;
            }
            nLine++;
        }
        m_pairHeader.first  = lOffset;
        m_pairHeader.second = lHeaderLen;
        
        // 3. Body: 
        m_pairBody.first    = objAnalyzer.GetOffset();
        m_pairBody.second   = GetLength() - m_pairStartLine.second - m_pairHeader.second;
        
        // 
        SetValid();
        return(IsValid());
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CSipMessage::Parse(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CSipMessage::GetStartLine(CSipStartLine& objSipStartLine)
{
    try
    {
        // 
        if(m_pairStartLine.second > 0) 
        {
            objSipStartLine.SetBuffer(GetBufferPointer() + m_pairStartLine.first, m_pairStartLine.second);
            return(true);
        }
        return(false);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::GetStartLine(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipMessage::GetHeader(const std::string sSipHeaderNameStandard, CSipHeader& objSipHeader)
{
    try
    {
        // 
        std::pair< std::multimap< std::string, int >::const_iterator, std::multimap< std::string, int >::const_iterator > rangeHeader;
        rangeHeader = m_mmapHeaderName.equal_range(sSipHeaderNameStandard);
        if(rangeHeader.first == rangeHeader.second) 
        {
            // Header not found. 
            return(false);
        }
        // Get line: 
        std::map< int, std::pair<int, int> >::const_iterator iLine;
        iLine = m_mapHeaderLine.find((*rangeHeader.first).second);
        if(m_mapHeaderLine.end() == iLine) 
        {
            // Can not find this line: 
            throw CException(-1, "%s(%ld): CSipMessage::GetHeader(): Internal error can not find line: %d in header line map.", __FILE__, __LINE__, (*rangeHeader.first).second);
        }
        // 
        objSipHeader.SetBuffer(GetBufferPointer() + (*iLine).second.first, (*iLine).second.second);
        return(true);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::GetHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CSipMessage::InsertHeader(const CSipHeader& objSipHeader, int nAtLine)
{
    try
    {
        if(!objSipHeader.IsValid())
        {
            throw CException(-1, "%s(%ld): CSipMessage::InsertHeader(): Invalid header.", __FILE__, __LINE__);
        }
        // 
        if(objSipHeader.GetLength() <= 0) 
        {
            throw CException(-1, "%s(%ld): CSipMessage::InsertHeader(): Header is empty.", __FILE__, __LINE__);
        }
        // 
        int nLine, nLineStart, nLineLength;
        // 
        if(nAtLine == 0) 
        {
            // 
            throw CException(-1, "%s(%ld): CSipMessage::InsertHeader(): Invalid line: %d.", __FILE__, __LINE__, nAtLine);
        }else if(nAtLine < 0) 
        {
            // Insert after the last line: 
            nLine = m_mapHeaderLine.size() + 1;
        }else 
        {
            // Insert as nAtLine: 
            if(nAtLine > m_mapHeaderLine.size() + 1)
            {
                throw CException(-1, "%s(%ld): CSipMessage::InsertHeader(): Invalid line: %d.", __FILE__, __LINE__, nAtLine);
            }
            nLine = nAtLine;
        }

        // Get line start location: 
        std::map< int, std::pair<int, int> >::iterator iLine;
        iLine = m_mapHeaderLine.find(nLine);
        if(m_mapHeaderLine.end() == iLine)
        {
            // Can not find a line: I assume the line is just after the last line. 
            nLineStart = m_pairHeader.first + m_pairHeader.second - 2;
        }else 
        {
            // Need to move succeeding line down: 
            nLineStart = (*iLine).second.first;
        }
        // Get line length: 
        nLineLength = objSipHeader.GetLength();

        // Insert into buffer: 
        Insert(nLineStart, objSipHeader.GetBufferPointer(), nLineLength);

        // Maintain structures: 
        if(m_mapHeaderLine.end() == iLine)
        {
            // Last line, add: 
            m_mapHeaderLine.insert(std::pair< int, std::pair<int, int> >(nLine, std::pair<int, int>(nLineStart, nLineLength)));
        }else 
        {
            // Update header-line map: 
            // Not last header, move lines down: 
            std::map< int, std::pair<int, int> >::reverse_iterator iHeaderLine, iHeaderLineUpper;

            // Add one more line as last line, and copy former last line into: 
            iHeaderLine = iHeaderLineUpper = m_mapHeaderLine.rbegin();
            m_mapHeaderLine.insert(std::pair< int, std::pair<int, int> >((*iHeaderLineUpper).first + 1, std::pair<int, int>((*iHeaderLineUpper).second.first + nLineLength, (*iHeaderLineUpper).second.second)));

            // Update every line lower than line inserted: 
            iHeaderLineUpper++;
            for(; iHeaderLine != m_mapHeaderLine.rend() && (*iHeaderLine).first > nLine; ++iHeaderLine, ++iHeaderLineUpper)
            {
                (*iHeaderLine).second.first     = (*iHeaderLineUpper).second.first + nLineLength;
                (*iHeaderLine).second.second    = (*iHeaderLineUpper).second.second;
            }
            
            // Update inserted line itself: 
            (*iLine).second.second = nLineLength;

            // Update header-name map: 
            std::multimap< std::string, int >::iterator iHeaderName;
            for(iHeaderName = m_mmapHeaderName.begin(); iHeaderName != m_mmapHeaderName.end(); ++iHeaderName) 
            {
                if((*iHeaderName).second >= nLine)
                {
                    (*iHeaderName).second++;
                }
            }
        }
        // Add line: 
        m_mmapHeaderName.insert(std::pair< std::string, int >(objSipHeader.GetNameStandard(), nLine));

        // Update header, body: 
        m_pairHeader.second += nLineLength;
        m_pairBody.first    += nLineLength;
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::InsertHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

int CSipMessage::GetTopLine(const std::string sSipHeaderNameStandard)
{
    int nLine;
    std::pair< std::multimap< std::string, int >::iterator, std::multimap< std::string, int >::iterator > rangeHeader;
    std::multimap< std::string, int >::iterator i;
    rangeHeader = m_mmapHeaderName.equal_range(sSipHeaderNameStandard);
    if(rangeHeader.first == rangeHeader.second) 
    {
        // No existing header found, add to the last header: 
        nLine = -1;
    }else 
    {
        // insert on top of all existing header: 
        // Get top line: 
        nLine = 1000;
        for(i = rangeHeader.first; i != rangeHeader.second; ++i)
        {
            if((*i).second < nLine) 
            {
                nLine       = (*i).second;
            }
        }
    }
    return(nLine);
}

void CSipMessage::PushHeader(const CSipHeader& objSipHeader) 
{
    try
    {
        int nLine;
        if((nLine = GetTopLine(objSipHeader.GetNameStandard())) <= 0)
        {
            // Can not find a line: . 
        }
        // 
        InsertHeader(objSipHeader, nLine);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::PushHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CSipMessage::PopHeader(const std::string sSipHeaderNameStandard, CSipHeader& objSipHeader)
{
    try
    {
        // Get header: 
        int nLine, nStart, nLen;
        std::map< int, std::pair<int, int> >::iterator iLine;
        // 
        if((nLine = GetTopLine(sSipHeaderNameStandard)) <= 0)
        {
            // Can not find header: 
            return(false);
        }
        // 
        iLine = m_mapHeaderLine.find(nLine);
        if(m_mapHeaderLine.end() == iLine) 
        {
            // Can not find this line: 
            throw CException(-1, "%s(%ld): CSipMessage::RemoveHeader(): Internal error can not find line: %d in header line map.", __FILE__, __LINE__, nLine);
        }
        // 
        nStart  = (*iLine).second.first;
        nLen    = (*iLine).second.second;
        // 
        objSipHeader.SetBuffer(GetBufferPointer() + nStart, nLen);

        // Remove header: 
        Erase(nStart, nLen);
  
        // Maintain structures: 
        // Header-line: 
        // Update every line lower than poped line: 
        std::map< int, std::pair< int, int> >::iterator iHeaderLine, iHeaderLineLower;
        iHeaderLineLower = iHeaderLine = iLine;
        iHeaderLineLower++;
        for(; iHeaderLineLower != m_mapHeaderLine.end(); ++iHeaderLineLower, ++iHeaderLine) 
        {
            (*iHeaderLine).second.first     = (*iHeaderLineLower).second.first - nLen;
            (*iHeaderLine).second.second    = (*iHeaderLineLower).second.second;
        }
        // Remove last line: 
        m_mapHeaderLine.erase(m_mapHeaderLine.size());

        // Header-name: 
        std::multimap< std::string, int >::iterator iHeaderName, iHeaderNameRemove;
        for(iHeaderName = m_mmapHeaderName.begin(); iHeaderName != m_mmapHeaderName.end(); ++iHeaderName) 
        {
            if((*iHeaderName).second == nLine)
            {
                iHeaderNameRemove = iHeaderName;
            }else if((*iHeaderName).second > nLine)
            {
                (*iHeaderName).second--;
            }
        }
        // 
        m_mmapHeaderName.erase(iHeaderNameRemove);
        
        // 
        m_pairHeader.second -= nLen;
        m_pairBody.first  -= nLen;

        // 
        return(true);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::PushHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

/*
bool CSipMessage::RemoveHeader(int nLine, CSipHeader& objSipHeader) 
{
    try
    {
        // Get header: 
        std::map< int, std::pair<int, int> >::iterator iLine;
        iLine = m_mapHeaderLine.find(nLine);
        if(m_mapHeaderLine.end() == iLine) 
        {
            // Can not find this line: 
            throw CException(-1, "%s(%ld): CSipMessage::RemoveHeader(): Internal error can not find line: %d in header line map.", __FILE__, __LINE__, nLine);
        }
        // 
        int nStart, nLen;
        nStart  = (*iLine).second.first;
        nLen    = (*iLine).second.second;
        objSipHeader.SetBuffer(GetBufferPointer() + nStart, nLen);
        // Remove header: 
        Erase((*iLine).second.first, (*iLine).second.second);
        
        // Maintain structures: 
        std::map< int, std::pair< int, int> >::iterator iHeaderLine, iHeaderLineNext;
        iHeaderLineNext = iHeaderLine = iLine;
        iHeaderLineNext++;
        for(; iHeaderLineNext != m_mapHeaderLine.end(); ++iHeaderLineNext, ++iHeaderLine) 
        {
            (*iHeaderLine).second.first     = (*iHeaderLineNext).second.first - nLen;
            (*iHeaderLine).second.second    = (*iHeaderLineNext).second.second;
        }
        // Remove last line: 
        m_mapHeaderLine.erase(iHeaderLineNext);
        
        
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CSipMessage::RemoveHeader(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/
