/* =========================================================================== /
* @category     Memory operation
* @package      Buffer
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
#include <stdio.h>
#include <string.h>
#include "CException.h"
#include "CBufferFrame.h"

// --------------------------------------------------------------------------- /
// CBufferFrame 1.0
// --------------------------------------------------------------------------- /
inline void CBufferFrame::SetSize(long nSize)
{
    if(nSize < 0)
    {
        throw CException(-1, "%s(%ld): CBufferFrame::SetSize(long): Invalid buffer size: %ld.", __FILE__, __LINE__, nSize);
    }
    m_nSize = nSize;
}
// 
inline void CBufferFrame::SetHighWater(long nHighWater)
{
    if(nHighWater < 0 || nHighWater > GetSize())
    {
        throw CException(-1, "%s(%ld): CBufferFrame::SetHighWater(long): Invalid high water: %ld, should between: %ld and %ld.", __FILE__, __LINE__, 
            nHighWater, 0, GetSize());
    }
    m_nHighWater = nHighWater;
}
// 
inline void CBufferFrame::SetAttribute(long nSize, long nHighWater)
{
    try
    {
        SetSize(nSize);
        SetHighWater(nHighWater);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::SetAttribute(long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------//
void CBufferFrame::BindBuffer(char *pBuffer, long nSize, long nHighWater)
{
    try
    {
        if(NULL == pBuffer)
        {
            throw CException(-1, "%s(%ld): CBufferFram::BindBuffer(char *, long, long): NULL pointer.", 
                __FILE__, __LINE__);
        }
        m_pBuffer = (char*)pBuffer;
        SetAttribute(nSize, nHighWater);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::BindBuffer(char *, long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
// 
void CBufferFrame::BindBuffer(CBufferFrame& objBufferFrame)
{
    try
    {
        BindBuffer(objBufferFrame.m_pBuffer, objBufferFrame.m_nSize, objBufferFrame.m_nHighWater);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::BindBuffer(CBufferFrame&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CBufferFrame::BindBuffer(char *szBuffer)
{
    try
    {
        if(NULL == szBuffer)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::BindBuffer(char *): NULL pointer.", __FILE__, __LINE__);
        }
        long nLen = strlen(szBuffer);
        BindBuffer(szBuffer, nLen, nLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::BindBuffer(char *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
inline bool CBufferFrame::IsBufferBound() const
{
    if(NULL == m_pBuffer) return(false);
    else return(true);
}

//---------------------------------------------------------------------------//
CBufferFrame::CBufferFrame()
{
    try
    {
        // Set buffer pointer NULL
        m_pBuffer       = NULL;
        // Initialize other attributes
        SetSize(0);
        SetHighWater(0);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::CBufferFrame(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
// 
CBufferFrame::CBufferFrame(CBufferFrame& objBufferFrame)
{
    try
    {
        // 
        BindBuffer(objBufferFrame);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::CBufferFrame(CBufferFrame&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CBufferFrame::CBufferFrame(char* szBuffer)
{
    try
    {
        BindBuffer(szBuffer);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::CBufferFrame(char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
// 
CBufferFrame::~CBufferFrame()
{
}

//---------------------------------------------------------------------------//
const CBufferFrame& CBufferFrame::operator = (CBufferFrame& objBufferFrame)
{
    try
    {
        BindBuffer(objBufferFrame);
        return(*this);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::operator = (CBufferFrame&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const CBufferFrame& CBufferFrame::operator = (char* szBuffer)
{
    try
    {
        BindBuffer(szBuffer);
        return(*this);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::operator = (char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//---------------------------------------------------------------------------//
// Size is the buffer size initialed outside: 
/*
inline long CBufferFrame::GetSize() const
{
    return(m_nSize);
}
// 
inline long CBufferFrame::GetHighWater() const
{
    return(m_nHighWater);
}

// Length is the area between low water and high water: 
inline long CBufferFrame::GetLength() const
{
    return(m_nHighWater >= 0 ? m_nHighWater : 0);
}
// 
inline bool CBufferFrame::IsEmpty() const
{
    if(GetLength() <= 0) return(true);
    else return(false);
}

// 
inline long CBufferFrame::GetSizeRemain() const
{
    return(m_nSize - m_nHighWater < 0 ? 0 : m_nSize - m_nHighWater);
}
// 
inline bool CBufferFrame::IsFull() const
{
    if(GetSizeRemain() <= 0) return(true);
    else return(false);
}
*/
//---------------------------------------------------------------------------//
/*
bool CBufferFrame::GetChar(long nPosition, char& cChar)
{
    try
    {
        if(nPosition < 0)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::GetChar(long nPosition, char&): Invalid position: %ld.", 
                __FILE__, __LINE__, nPosition);
        }
        // Return the char: 
        if(nPosition < m_nHighWater)
        {
            cChar = m_pBuffer[nPosition];
            return(true);
        }else return(false);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::GetChar(long nPosition, char&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/

//---------------------------------------------------------------------------//
long CBufferFrame::Read(long nPosition, long nLen, char *pBuffer, long nSize) const 
{
    try
    {
        // Sanity check: 
        if(NULL == pBuffer || nSize <= 0) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Read(long, long, char*, long): NULL pointer or invalid buffer size: %ld.", 
                __FILE__, __LINE__, nSize);
        }
        if(nPosition < 0 || nLen < 0)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Read(long, long, char*, long): Invalid position: %ld or read length: %ld.", 
                __FILE__, __LINE__, nPosition, nLen);
        }
        // Calculate read length: 
        int nAvailableLen = m_nHighWater - nPosition > 0 ? m_nHighWater - nPosition : 0;
        int nReadLen = nAvailableLen > nLen ? nLen : nAvailableLen;
        // 
        if(nReadLen >= nSize)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Read(long, long, char*, long): Buffer too small, at least: %ld, but only: %ld.", 
                __FILE__, __LINE__, nReadLen + 1, nSize);
        }
        // 
        if(nReadLen > 0)
        {
            memcpy(pBuffer, m_pBuffer + nPosition, nReadLen);
            pBuffer[nReadLen] = '\0';
        }else
        {
            pBuffer[0] = '\0';
        }
        // 
        return(nReadLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Read(long, long, char*, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Read(long nPosition, long nLen, std::string& sBuffer) const 
{
    try
    {
        // Sanity check: 
        if(nPosition < 0 || nLen < 0)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Read(long, long, std::string&): Invalid position: %ld or read length: %ld.", 
                __FILE__, __LINE__, nPosition, nLen);
        }
        // Calculate read length: 
        int nAvailableLen = m_nHighWater - nPosition > 0 ? m_nHighWater - nPosition : 0;
        int nReadLen = nAvailableLen > nLen ? nLen : nAvailableLen;
        // 
        if(nReadLen > 0)
        {
            sBuffer.assign(m_pBuffer + nPosition, nReadLen);
        }
        // 
        return(nReadLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Read(long, long, std::string&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
std::string CBufferFrame::Read(long nPosition, long nLen) const 
{
    try
    {
        // Sanity check: 
        if(nPosition < 0 || nLen < 0)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Read(long, long): Invalid position: %ld or read length: %ld.", 
                __FILE__, __LINE__, nPosition, nLen);
        }
        // Calculate read length: 
        int nAvailableLen = m_nHighWater - nPosition > 0 ? m_nHighWater - nPosition : 0;
        int nReadLen = nAvailableLen > nLen ? nLen : nAvailableLen;
        // 
        if(nReadLen > 0)
        {
            return(std::string(m_pBuffer + nPosition, nReadLen));
        }else return(std::string());
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Read(long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
void CBufferFrame::Clear()
{
    try
    {
        // 
        if(!IsBufferBound())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Clear(): Buffer has not been allocated yet.", __FILE__, __LINE__);
        }
        // 
        bzero(m_pBuffer, GetSize());
        // 
        SetHighWater(0);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Trim(long nLen)
{
    try
    {
        if(0 == nLen) return(0);
        // 
        if(nLen < 0)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Trim(long): Invalid length: %d.", __FILE__, __LINE__, nLen);
        }
        // 
        if(!IsBufferBound())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Trim(long): Buffer has not been bound yet.", __FILE__, __LINE__);
        }
        if(GetLength() < nLen) nLen = GetLength();
        SetHighWater(GetHighWater() - nLen);
        return(nLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Trim(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// Append will not affect current offset.
long CBufferFrame::Append(const char *pBuffer, long nLen)
{
    try
    {
        if(nLen == 0) return(0);
        // 
        if(pBuffer == NULL)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Append(const char*, long): Null pointer.", __FILE__, __LINE__);
        }
        if(nLen < 0) nLen = strlen(pBuffer);   // Use string length.
        if(nLen == 0) return(0);
        // 
        if(!IsBufferBound())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Append(const char*, long): Buffer has not been allocated yet.", __FILE__, __LINE__);
        }

        // Append data, increase high water: 
        if(GetSizeRemain() < nLen)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Append(const char*, long): Buffer overflowed, trying to append %ld bytes starting from %ld with in %ld bytes buffer.", 
                __FILE__, __LINE__, nLen, GetHighWater(), GetSize());
        }
        // Copy: 
        memcpy((char *)(m_pBuffer + m_nHighWater), pBuffer, nLen);
        SetHighWater(m_nHighWater + nLen);
        return(nLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Append(const char*, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Write(long nLocation, const char* pBuffer, long nLen) 
{
    try
    {
        if(nLen == 0) return(0);
        // 
        if(pBuffer == NULL) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Write(long, const char*, long): Null pointer.", __FILE__, __LINE__);
        }
        if(nLen < 0) nLen = strlen(pBuffer); // Use string length.
        if(nLen == 0) return(0);
        // 
        if(nLocation < 0 || nLocation >= GetSize()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Write(long, const char*, long): Invlaid writing location: %ld, should between 0 and %ld.", 
                __FILE__, __LINE__, nLocation, GetSize() - 1);
        }
        if(nLocation < 0 || nLocation > GetHighWater())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Invlaid writing location: %ld, should between 0 and high water: %ld.", 
                __FILE__, __LINE__, nLocation, GetHighWater());
        }
        // 
        if(!IsBufferBound()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Write(long, const char*, long): Buffer has not been allocated yet.", __FILE__, __LINE__);
        }
        
        // Get real length can write: 
        if(GetSize() - nLocation < nLen)
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Write(long, const char*, long): Buffer overflowed, trying to write %d bytes starting from %d with in %d bytes buffer.", 
                __FILE__, __LINE__, nLen, nLocation, GetSize());
        }
        
        // Copy: 
        memcpy((char *)(m_pBuffer + nLocation), pBuffer, nLen);
        
        // See if we need to raise high water: 
        if(nLocation + nLen > GetHighWater())
        {
            // Beyond water, increase high water
            SetHighWater(nLocation + nLen);
        }
        return(nLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Write(long, const char*, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Erase(long nLocation, long nLen)
{
    try
    {
        // 
        if(nLocation < 0 || nLocation >= GetSize()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Erase(long, long): Invlaid writing location: %ld, should between 0 and size: %ld.", 
                __FILE__, __LINE__, nLocation, GetSize() - 1);
        }
        if(nLocation < 0 || nLocation > GetHighWater())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Erase(long, long): Invlaid writing location: %ld, should between 0 and high water: %ld.", 
                __FILE__, __LINE__, nLocation, GetHighWater());
        }
        // 
        if(nLen < 0)
        {
            // To the end of buffer: 
            nLen = GetHighWater() - nLocation;
        }
        if(0 == nLen) return(0);
        // 
        if(!IsBufferBound()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Erase(long, long): Buffer has not been allocated yet.", __FILE__, __LINE__);
        }

        // 
        long nLenErase;
        long nLenCopy     = GetHighWater() - nLocation - nLen;
        if(nLenCopy > 0)
        {
            memcpy(m_pBuffer + nLocation, m_pBuffer + nLocation + nLen, nLenCopy);
            nLenErase = nLen;
        }else
        {
            // All from location has been removed: 
            nLenErase = GetHighWater() - nLocation;
        }
        
        // Update high water: 
        SetHighWater(GetHighWater() - nLenErase);
        // 
        return(nLenErase);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Erase(long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Insert(long nLocation, const char* pBuffer, long nLen)
{
    try
    {
        if(nLen == 0) return(0);
        // 
        if(pBuffer == NULL) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Null pointer.", __FILE__, __LINE__);
        }
        if(nLen < 0) nLen = strlen(pBuffer);   // Use string length.
        if(nLen == 0) return(0);
        // 
        if(nLocation < 0 || nLocation >= GetSize()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Invlaid writing location: %ld, should between 0 and size: %ld.", 
                __FILE__, __LINE__, nLocation, GetSize() - 1);
        }
        if(nLocation < 0 || nLocation > GetHighWater())
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Invlaid writing location: %ld, should between 0 and high water: %ld.", 
                __FILE__, __LINE__, nLocation, GetHighWater());
        }
        // 
        if(!IsBufferBound()) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Buffer has not been allocated yet.", __FILE__, __LINE__);
        }

        // Check size: 
        if(GetSizeRemain() < nLen) 
        {
            throw CException(-1, "%s(%ld): CBufferFrame::Insert(long, const char*, long): Buffer overflowed, trying to insert %d bytes starting from %d with in %d bytes buffer of %d length.", 
                __FILE__, __LINE__, nLen, nLocation, GetSize(), GetHighWater());
        }
        
        // Move the rest of buffer forward: 
        // To avoid memcpy() overlapping on some OS, need a loop to copy: 
        /*
        memcpy((char *)m_pBuffer + nLocation + nLen, (char*)m_pBuffer + nLocation, GetHighWater() - nLocation);
        */
        long lLenCopy = GetHighWater() - nLocation;
/*
        if(lLenCopy > 0)
        {
            long lCopyFrom  = GetHighWater() - 1;
            long lCopyTo    = nLocation + nLen;
            for(; lLenCopy > 0; --lLenCopy, --lCopyFrom, --lCopyTo)
            {
                m_pBuffer[lCopyTo] = m_pBuffer[lCopyFrom];
            }
        }
*/
        if(lLenCopy > 0)
        {
            long lCopyFrom  = GetHighWater() - 1;
            long lCopyTo    = lCopyFrom + nLen;
            for(; lLenCopy > 0; --lLenCopy, --lCopyFrom, --lCopyTo)
            {
                m_pBuffer[lCopyTo] = m_pBuffer[lCopyFrom];
            }
        }
        SetHighWater(GetHighWater() + nLen);
        
        // Insert data into area: 
        return(Write(nLocation, pBuffer, nLen));
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Insert(long, const char*, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
long CBufferFrame::Replace(long nLocation, long nLenSource, const char* pBuffer, long nLenReplace)
{
    try
    {
        // Erase: 
        Erase(nLocation, nLenSource);
        // Insert: 
        return(Insert(nLocation, pBuffer, nLenReplace));
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBufferFrame::Replace(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
void CBufferFrame::Print(long nOffset, long nLength) const
{
    if(!IsBufferBound()) 
    {
        throw CException(-1, "%s(%ld): CBufferFrame::Print(): Buffer has not been allocated yet.", __FILE__, __LINE__);
    }
    if(nOffset < 0)
    {
        throw CException(-1, "%s(%ld): CBufferFrame::Print(): Invalid offset: %ld.", __FILE__, __LINE__, nOffset);
    }
    if(nLength < 0)
    {
        // Print to the end. 
        nLength = m_nHighWater - nOffset + 1;
    }
    if(nOffset + nLength > m_nHighWater + 1)
    {
        throw CException(-1, "%s(%ld): CBufferFrame::Print(): Overflowed: Offset(%ld), Length(%ld).", __FILE__, __LINE__, nOffset, nLength);
    }
    // 
    printf("[");
    for(int i = nOffset; i < nOffset + nLength; i++) printf("%c", m_pBuffer[i]);
    printf("]");
}
