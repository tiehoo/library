/* =========================================================================== /
* @category     Memory Operation
* @package      Buffer
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CBUFFERANALYZER_H
#define _CBUFFERANALYZER_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <string.h>
#include <assert.h>
#include "CBufferFrame.h"

/* --------------------------------------------------------------------------- /
// CBufferAnalyzer 1.0 
// --------------------------------------------------------------------------- */
//Constants: 
const int MAX_SEQ_NUM     = 8;    // Max number of sequnce.
const int MAX_SEQ_LEN     = 64;   // Max length of sequnce.

//  String functions: 
/*
       #include <ctype.h>
       
       int isalnum(int c);
       int isalpha(int c);
       int isascii(int c);
       int isblank(int c);
       int iscntrl(int c);
       int isdigit(int c);
       int isgraph(int c);
       int islower(int c);
       int isprint(int c);
       int ispunct(int c);
       int isspace(int c);
       int isupper(int c);
       int isxdigit(int c);
*/
class CBufferAnalyzer
{
private: 
    // Buffer bound: 
    CBufferFrame&   m_objBuffer;
    // Seek direction: 
    bool            m_bSeekForward;
    // Case sensitive: 
    bool            m_bCaseSensitive;
    // Offset: 
    long            m_nOffset;
    // Define sequence set: "\n\r,\n"
    int             m_nSequenceNum;
    char            m_szSequenceSet[MAX_SEQ_NUM][MAX_SEQ_LEN];
public: 
    explicit        CBufferAnalyzer(CBufferFrame& objBuffer);
    ~CBufferAnalyzer();
//---------------------------------------------------------------------------//
// 
    // I develop those inline functions, can improve several times performance.
    inline
    char        ToLower(const char cChar) const
    {
        if(cChar >= 65 && cChar <= 90) return(cChar + 32);
        else return(cChar);
    }
    // 
    inline
    char        ToUpper(const char cChar) const
    {
        if(cChar >= 97 && cChar <= 122) return(cChar - 32);
        else return(cChar);
    }
//---------------------------------------------------------------------------//
    // Find a char in a non-terminated string: 
    inline 
    bool        IsCharInString(char cChar, const char *pString, long nLen, long& nLocation) const
    {
        // Sanity check: 
        //if(NULL == pString) return(false);
        assert(pString);
        // 
        long i;
        if(m_bCaseSensitive)
        { 
            for(i = 0; i < nLen && cChar != pString[i]; ++i);
        }else
        {
            for(i = 0; i < nLen && ToUpper(cChar) != ToUpper(pString[i]); ++i);
        }
        // 
        nLocation = i;
        // 
        if(nLen == i) return(false);
        else return(true);
    }
    // Find a char in a terminated string: 
    inline 
    bool        IsCharInString(char cChar, const char *szString, long& nLocation) const
    {
        return(IsCharInString(cChar, szString, strlen(szString), nLocation));
    }
    // If string1 is in string2: 
    inline 
    bool        IsStringInString(const char *pString1, long nLen1, const char *pString2, long nLen2, long& nLocation) const
    {
        //if(NULL == pString1 || NULL == pString2) return(false);
        assert(pString1);
        assert(pString2);
        if(nLen1 <= 0 || nLen2 <= 0) return(false);
    
        // A wonderful loop: 
        int nIndex1 = 0, nIndex2;
        for(nIndex2 = 0; nIndex2 < nLen2; ++nIndex2)
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
    // 
    inline 
    bool        IsStringInString(const char *szString1, const char *szString2, long& nLocation) const
    {
        return(IsStringInString(szString1, strlen(szString1), szString2, strlen(szString2), nLocation));
    }

//---------------------------------------------------------------------------//
// Basic functions: 
    // Set seek direction: 
    inline 
    void        SetForward()
    {
        m_bSeekForward = true;
    }
    inline 
    void        SetBackward()
    {
        m_bSeekForward = false;
    }
    // Set case sensitive: 
    inline 
    void        SetCaseSensitive()
    {
        m_bCaseSensitive = true;
    }
    inline 
    void        SetCaseInsensitive()
    {
        m_bCaseSensitive = false;
    }

    // Seek pointer: 
    inline 
    void        SetOffset(long nOffset)
    {
        if(nOffset < 0) return;
        // I don't check if nOffset passed buffer end here.
        m_nOffset = nOffset;
    }
    inline 
    long        GetOffset()
    {
        return(m_nOffset);
    }

    // Seek to begining of the buffer: 
    inline 
    void        Rewind()
    {
        SetOffset(0);
    }
    // Seek to the end of the buffer: 
    inline 
    void        SeekToEnd()
    {
        SetOffset(m_objBuffer.GetHighWater());
    }
    // Seek relatively: Return false if pass the buffer end.
    inline 
    void        Advance(long nRelativeOffset)
    {
        SetOffset(GetOffset() + nRelativeOffset);
    }

    // 
    inline 
    bool        IsEnd()
    {
        return(m_nOffset >= m_objBuffer.GetHighWater());
    }

//---------------------------------------------------------------------------//
    // Find a char in the set: Skip until find the token: return false if can not find.
    inline 
    bool        FindChar(const char* szTokenSet, char &cTokenUsed, long &lSkipLen)
    {
        // Find char starting from current offset: 
        //if(NULL == szTokenSet) return(false);
        assert(szTokenSet);
        // 
        long lLocation;
        char cChar;
        // 
        for(lSkipLen = 0; m_objBuffer.GetChar(m_nOffset, cChar); ++lSkipLen, ++m_nOffset)
        {
            if(IsCharInString(cChar, szTokenSet, lLocation))
            {
                // Found char
                lSkipLen++;             // Need to increase one.
                m_nOffset++;
                cTokenUsed = cChar;
                return(true);
            }
        }
        // EOF
        return(false);
    }
    
    // Find a sequence: Skip until find a sequence: \r\n
    inline 
    bool        FindSequence(const char* szSequence, long &lSkipLen)
    {
        //if(NULL == szSequence) return(false);
        assert(szSequence);
        
        // A wonderful loop: 
        char cChar;
        int nIndex = 0;
        for(lSkipLen = 0; m_objBuffer.GetChar(m_nOffset, cChar); ++lSkipLen, ++m_nOffset)
        {
            if(m_bCaseSensitive && cChar == szSequence[nIndex] ||
                !m_bCaseSensitive && ToUpper(cChar) == ToUpper(szSequence[nIndex]))
            {
                if('\0' == szSequence[nIndex + 1])
                {
                    lSkipLen++;             // Need to increase one.
                    m_nOffset++;
                    return(true);
                }else nIndex++;
            }else nIndex = 0;
        }
        return(false);
    }
/*
    // Find a sequence in the set: Skip until find a sequence: \r\n,\n
    bool        FindSequenceSet(const char* szSequenceSet, char *szSequenceUsed, long nSequenceUsedLen, long &lSkipLen, char cSequenceSetSeparation = ',');
*/
    // Skip chars in the set: 
    inline 
    long        Skip(const char* szCharSet)
    {
        // 
        assert(szCharSet);
        // 
        long lSkipLen, lLocation;
        char cChar;
        // 
        for(lSkipLen = 0; m_objBuffer.GetChar(m_nOffset, cChar); ++lSkipLen, ++m_nOffset)
        {
            if(!IsCharInString(cChar, szCharSet, lLocation)) break;
        }
        return(lSkipLen);
    }

    // Skip chars in the set backward: 
    inline 
    long        SkipBackFrom(const char* szCharSet, long nOffset)
    {
        // 
        assert(szCharSet);
        // 
        long lSkipLen, lLocation;
        char cChar;
        // 
        for(lSkipLen = 0; m_objBuffer.GetChar(nOffset, cChar); ++lSkipLen, --nOffset)
        {
            if(!IsCharInString(cChar, szCharSet, lLocation)) break;
        }
        return(lSkipLen);
    }
};

#endif
