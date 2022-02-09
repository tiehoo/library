/* =========================================================================== /
* @category     String operation
* @package      Analyzer
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
#include "CException.h"
#include "CBufferAnalyzer.h"

// --------------------------------------------------------------------------- /
// CBufferAnalyzer 1.0
// --------------------------------------------------------------------------- /
CBufferAnalyzer::CBufferAnalyzer(CBufferFrame& objBuffer):m_objBuffer(objBuffer)
{
    SetForward();
    SetCaseSensitive();
    Rewind();
}

CBufferAnalyzer::~CBufferAnalyzer()
{
}

// --------------------------------------------------------------------------- /
/*
// Find a char in a non-terminated string: 
bool CBufferAnalyzer::IsCharInString(char cChar, const char *pString, long nLen, long& nLocation) const
{
    if(NULL == pString)
    {
        throw CException(-1, "%s(%ld): CBufferAnalyzer::IsCharInString(char, const char *, long, long*): Null pointer.", 
            __FILE__, __LINE__);
    }
    // 
    long i;
    if(m_bCaseSensitive)
    {
        for(i = 0; i < nLen && cChar != pString[i]; i++);
    }else
    {
        for(i = 0; i < nLen && ToUpper(cChar) != ToUpper(pString[i]); i++);
    }
    // 
    nLocation = i;
    // 
    if(nLen == i) 
    {
        return(false);
    }else 
    {
        return(true);
    }
}
*/
/*
// Find a char in a terminated string: 
bool CBufferAnalyzer::IsCharInString(char cChar, const char *szString, long& nLocation) const
{
    if(NULL == szString)
    {
        throw CException(-1, "%s(%ld): CBufferAnalyzer::IsCharInString(char, const char *, long*): Null pointer.", 
            __FILE__, __LINE__);
    }
    // 
    long i;
    if(m_bCaseSensitive)
    {
        for(i = 0; '\0' != szString[i] && cChar != szString[i]; i++);
    }else
    {
        for(i = 0; '\0' != szString[i] && ToUpper(cChar) != ToUpper(szString[i]); i++);
    }
    // 
    nLocation = i;
    // 
    if('\0' == szString[i]) 
    {
        return(false);
    }else 
    {
        return(true);
    }
}
*/
/*
// If string1 is in string2: 
bool CBufferAnalyzer::IsStringInString(const char *pString1, long nLen1, const char *pString2, long nLen2, long& nLocation) const
{
    if(NULL == pString1 || NULL == pString2)
    {
        throw CException(-1, "%s(%ld): CBufferAnalyzer::IsStringInString(const char *, long, const char *, long, long*): Null pointer.", 
            __FILE__, __LINE__);
    }
    if(pString1[0] == '\0') return(false);

    // A wonderful loop: 
    int nIndex1 = 0, nIndex2;
    for(nIndex2 = 0; nIndex2 < nLen2; nIndex2++)
    {
        if(m_bCaseSensitive && pString2[nIndex2] == pString1[nIndex1] ||
            !m_bCaseSensitive && ToUpper(pString2[nIndex2]) == ToUpper(pString1[nIndex1]))
        {
            if(nIndex1 + 1 >= nLen1)
            {
                nLocation = nIndex2 - nIndex1;
                return(true);
            }else nIndex1++;
        }else nIndex1 = 0;
    }
    return(false);
}
*/
/*
// 
bool CBufferAnalyzer::IsStringInString(const char *szString1, const char *szString2, long& nLocation) const
{
    try
    {
        return(IsStringInString(szString1, strlen(szString1), szString2, strlen(szString2), nLocation));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferAnalyzer::IsStringInString(const char *, const char *, long*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/

//---------------------------------------------------------------------------//
/*
// Set seek direction: 
void CBufferAnalyzer::SetForward()
{
    m_bSeekForward = true;
}

void CBufferAnalyzer::SetBackward()
{
    m_bSeekForward = false;
}

// Set case sensitive: 
void CBufferAnalyzer::SetCaseSensitive()
{
    m_bCaseSensitive = true;
}

void CBufferAnalyzer::SetCaseInsensitive()
{
    m_bCaseSensitive = false;
}

//---------------------------------------------------------------------------//
// Seek pointer: 
void CBufferAnalyzer::SetOffset(long nOffset)
{
    if(nOffset < 0) 
    {
        throw CException(-1, "%s(%ld): CBufferAnalyzer::SetOffset(long): Invalid offset: %ld.", 
            __FILE__, __LINE__, nOffset);
    }
    // I don't check if nOffset passed buffer end here.
    m_nOffset = nOffset;
}

// 
long CBufferAnalyzer::GetOffset()
{
    return(m_nOffset);
}

//---------------------------------------------------------------------------//
// Seek to begining of the buffer: 
void CBufferAnalyzer::Rewind()
{
    m_nOffset = 0;
}

// Seek to the end of the buffer: 
void CBufferAnalyzer::SeekToEnd()
{
    m_nOffset = m_objBuffer.GetHighWater();
}

// Seek relatively: 
void CBufferAnalyzer::Advance(long nRelativeOffset)
{
    m_nOffset += nRelativeOffset;
}
*/

//---------------------------------------------------------------------------//
/*
// Find a char in the set: Skip until find the token: return false if can not find.
bool CBufferAnalyzer::FindChar(const char* szTokenStringing, char &cTokenUsed, long &lSkipLen)
{
    // Find char starting from current offset: 
    try
    {
        // 
        if(NULL == szTokenString)
        {
            throw CException(-1, "%s(%ld): CBufferAnalyzer::FindChar(const char*, char&, long&): NULL pointer.");
        }
        // 
        char cChar;
        // 
        for(lSkipLen = 0; m_objFrameSeek.get(cChar); lSkipLen++)
        {
            if(IsCharInStr(cChar, szTokenString, m_bIsCaseSensitive))
            {
                // Found char
                lSkipLen++;             // Need to increase one.
                cTokenUsed = cChar;
                return(true);
            }
        }
        // EOF
        return(false);
    }catch(CException &eEx)
    {
        eEx.SetMessage("CBufferAnalyzer::FindChar(): Failed.");
        throw eEx;
    }
}
*/
/*
// Find a sequence in the set: Skip until find a sequence: \r\n,\n
bool CBufferAnalyzer::FindSequenceSet(const char* szSequenceSet, char *szSequenceUsed, long nSequenceUsedLen, long &lSkipLen, char cSequenceSetSeparation)
{
    try
    {
        // Check parameter: 
        if(NULL == szSequenceUsed)
        {
            throw CException(-1, "%s(%ld): CBufferAnalyzer::FindSequence(): NULL pointer.", __FILE__, __LINE__);
        }
        // 
        if(nSeqUsedLen <= 0)
        {
            throw CException(-1, "%s(%ld): CBufferAnalyzer::FindSequence(): Invalid used sequance length: %d.", __FILE__, __LINE__, nSeqUsedLen);
        }
        
        // Set sequence set: 
        if(NULL != szSequenceSet)
        {
            SetSequenceSet(szSequenceSet, cSepChar);
        }
        // 
        char cChar;
        bool bIsMatched = false;
        int  nCurSeqNum = 0, nSeqIdx = 0;
        // A wonderful loop: 
        for(lSkipLen = 0; m_objFrameSeek.get(cChar); lSkipLen++)
        {
            if(m_bIsCaseSensitive && cChar == m_szSequenceSet[nCurSeqNum][nSeqIdx] || 
                !m_bIsCaseSensitive && tolower(cChar) == tolower(m_szSequenceSet[nCurSeqNum][nSeqIdx]))
            {
                // Current sequence matches: 
                bIsMatched = true;
            }else
            {
                // Can not match current sequence, check others: 
                for(nCurSeqNum = 0; nCurSeqNum < m_nSequenceNum; nCurSeqNum++)
                {
                    if(
                        (m_bIsCaseSensitive && cChar == m_szSequenceSet[nCurSeqNum][nSeqIdx] && 
                            (nSeqIdx == 0 || strncmp(szSequenceUsed, m_szSequenceSet[nCurSeqNum], nSeqIdx) == 0)) || 
                        (!m_bIsCaseSensitive && tolower(cChar) == tolower(m_szSequenceSet[nCurSeqNum][nSeqIdx]) && 
                            (nSeqIdx == 0 || strncasecmp(szSequenceUsed, m_szSequenceSet[nCurSeqNum], nSeqIdx) == 0))
                      )
                    {
                        // 
                        bIsMatched = true;
                        break;
                    }
                }
                if(nCurSeqNum == m_nSequenceNum) bIsMatched = false;
            }
            if(bIsMatched)
            {
                // Need to store matched sequence: 
                if(nSeqIdx >= nSeqUsedLen - 1)
                {
                    throw CException(-1, "CBufferAnalyzer::FindSequence(): Matched sequence buffer too small: %d", 
                        nSeqUsedLen);
                }
                szSequenceUsed[nSeqIdx] = cChar;
                // 
                if(m_szSequenceSet[nCurSeqNum][nSeqIdx + 1] == '\0')
                {
                    // Found a matched sequence: 
                    szSequenceUsed[nSeqIdx + 1] = '\0';
                    lSkipLen++;         // Need to increase one.
                    //nSkipLen = lSkipLen - strlen(m_szSequenceSet[nCurSeqNum]) + 1;
                    return(true);
                }
                nSeqIdx++;
            }else
            {
                nSeqIdx = 0;
            }
        }   // for(lSkipLen = 0; m_objFrameSeek.get(cChar); lSkipLen++)
        
        // Can not find sequence: 
        szSequenceUsed[0] = '\0';
        return(false);
    }catch(CException &eEx)
    {
        eEx.SetMessage("CBufferAnalyzer::FindSequence(): Failed.");
        throw eEx;
    }
}
*/
/*
//---------------------------------------------------------------------------//
// Skip chars in the set: 
long CBufferAnalyzer::Skip(const char* szCharSet)
{
}
*/
