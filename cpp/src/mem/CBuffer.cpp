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
#include <string.h>
#include "CException.h"
#include "CBuffer.h"

// --------------------------------------------------------------------------- /
// CBuffer 1.0
// --------------------------------------------------------------------------- /
// 
size_t CBuffer::Allocate(long nSize)
{
    try
    {
        if(NULL == m_pBuffer)
        {
            if(nSize <= 0)
            {
                throw CException(-1, "%s(%ld): CBuffer::Allocate(): Invalid buffer size: %ld.", __FILE__, __LINE__, nSize);
            }
            
            // Try to allocate one byte more.
            m_pBuffer = new char[nSize + 1];
            if(NULL == m_pBuffer)
            {
                throw CException(-1, "%s(%ld): CBuffer::Allocate(): Failed to allocate buffer of size: %ld.", __FILE__, __LINE__, nSize);
            }
            // Add '\0': 
            m_pBuffer[nSize] = '\0';
            // Note: Here I set high water to 0.
            SetAttribute(nSize, 0);
            // Clear buffer to 0.
            Clear();
            
            // 
            return(GetSize());
        }else
        {
            throw CException(-1, "%s(%ld): CBuffer::Allocate(): Buffer has already been allocated, current size is: %ld, trying to reallocate to: %ld.", 
                __FILE__, __LINE__, GetSize(), nSize);
        }
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::Allocate(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
CBuffer::CBuffer(long nSize)
{
    try
    {
        Allocate(nSize);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::CBuffer(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CBuffer::CBuffer(const CBuffer& objBuffer)
{
    try
    {
        Allocate(objBuffer.GetSize());
        // Copy content: 
        SetBuffer(objBuffer);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::CBuffer(const CBuffer&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CBuffer::CBuffer(const char* szBuffer)
{
    try
    {
        if(NULL == szBuffer)
        {
            throw CException(-1, "%s(%ld): CBuffer::CBuffer(const char*): NULL pointer.", __FILE__, __LINE__);
        }
        Allocate(strlen(szBuffer));
        // Copy content: 
        SetBuffer(szBuffer);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::CBuffer(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CBuffer::CBuffer(const char* pBuffer, long nSize)
{
    try
    {
        if(NULL == pBuffer)
        {
            throw CException(-1, "%s(%ld): CBuffer::CBuffer(const char*): NULL pointer.", __FILE__, __LINE__);
        }
        Allocate(nSize);
        // Copy content: 
        SetBuffer(pBuffer, nSize);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::CBuffer(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CBuffer::~CBuffer()
{
    if(IsBufferBound())
    {
        delete [] m_pBuffer;
    }
}

//----------------------------------------------------------------------------//
// This one used to protect from calling parent's function.
void CBuffer::SetBuffer(const char *pBuffer, long nLen)
{
    try
    {
        // Clear buffer: 
        SetHighWater(0);
        // 
        Append(pBuffer, nLen);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::SetBuffer(const char *, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CBuffer::SetBuffer(const char *szBuffer)
{
    try
    {
        SetBuffer(szBuffer, strlen(szBuffer));
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::SetBuffer(char *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CBuffer::SetBuffer(const CBuffer& objBuffer)
{
    try
    {
        SetBuffer(objBuffer.m_pBuffer, objBuffer.GetLength());
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::SetBuffer(const CBuffer&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
const CBuffer& CBuffer::operator = (const CBuffer& objBuffer)
{
    try
    {
        SetBuffer(objBuffer);
        return(*this);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::operator = (CBuffer&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const CBuffer& CBuffer::operator = (const char *szBuffer)
{
    try
    {
        SetBuffer(szBuffer);
        return(*this);
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CBuffer::operator = (const char *): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
const char* CBuffer::GetBufferPointer(long lOffset) const
{
    if(!IsBufferBound())
    {
        throw CException(-1, "%s(%ld): CBuffer::GetBufferPointer(): Buffer has not been allocated yet.", __FILE__, __LINE__);
    }
    if(lOffset < 0)
    {
        throw CException(-1, "%s(%ld): CBuffer::GetBufferPointer(): Invalid offset: %ld.", __FILE__, __LINE__, lOffset);
    }
    // 
    return(m_pBuffer + lOffset);
}

// 
const char* CBuffer::c_str() const
{
    if(!IsBufferBound())
    {
        throw CException(-1, "%s(%ld): CBuffer::GetBufferPointer(): Buffer has not been allocated yet.", __FILE__, __LINE__);
    }
    // Max: size + 1.
    m_pBuffer[GetHighWater()] = '\0';
    return(GetBufferPointer());
}
