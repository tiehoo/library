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
#ifndef _CBUFFERFRAME_H
#define _CBUFFERFRAME_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>

/*
// The buffer frame keeps the structure of buffer and manage the current pointer.
// It provide safe read and write functions.
*/
// --------------------------------------------------------------------------- /
// CBufferFrame 1.0
// --------------------------------------------------------------------------- /
class CBufferFrame
{
protected: 
    char*                   m_pBuffer;                      // Buffer base pointer
    // 
    long                    m_nSize;                        // Buffer: [0, size)
    // 
    long                    m_nHighWater;                   // Data: [0, high); Pass the end pointer.
public: 
// 
    CBufferFrame();
    explicit CBufferFrame(CBufferFrame& objBufferFrame);
    explicit CBufferFrame(char* szBuffer);
    CBufferFrame(char* pBuffer, long nSize, long nHighWater);
    ~CBufferFrame();
// 
    void                    SetSize(long nSize);
    void                    SetHighWater(long nHighWater);
    // SetAttribute() will deal with the process order of called functions: 
    void                    SetAttribute(long nSize, long nHighWater);
// 
    // Bind buffer to the frame: 
    void                    BindBuffer(CBufferFrame& objBufferFrame);
    void                    BindBuffer(char *szBuffer);
    void                    BindBuffer(char *pBuffer, long nSize, long nHighWater);
    // 
    bool                    IsBufferBound() const;            // Check if the buffer has been set or allocated.
// 
    const CBufferFrame&     operator = (CBufferFrame& objBufferFrame);
    const CBufferFrame&     operator = (char* szBuffer);
// Infomational part: 
  // Basic: 
    inline
    long                    GetSize() const 
    {
        return(m_nSize);
    }
    inline
    long                    GetHighWater() const 
    {
        return(m_nHighWater);
    }

  // Advanced: 
    //  Length related: 
    //      Length between 0 and high water.[0, high]
    inline 
    long                    GetLength() const
    {
        return(m_nHighWater >= 0 ? m_nHighWater : 0);
    }
    //      If Length <= 0;
    inline 
    bool                    IsEmpty() const
    {
        if(GetLength() <= 0) return(true);
        else return(false);
    }
    //  Size related: 
    //      Size between high water and max size.[high, size)
    long                    GetSizeRemain() const
    {
        return(m_nSize - m_nHighWater < 0 ? 0 : m_nSize - m_nHighWater);
    }
    //      If GetSizeRemain <= 0;
    bool                    IsFull() const
    {
        if(GetSizeRemain() <= 0) return(true);
        else return(false);
    }
// Operational part: 
  // Read: 
    // Note: This function is heavily used in the parser classes. Must use inline here!
    inline 
    bool                    GetChar(long nPosition, char& cChar) const 
    {
        if(nPosition < 0) return(false);
        // Return the char: 
        if(nPosition < m_nHighWater)
        {
            cChar = m_pBuffer[nPosition];
            return(true);
        }else return(false);
    }
    // 
    long                    Read(long nPosition, long nLen, char *pBuffer, long nSize) const;
    long                    Read(long nPosition, long nLen, std::string& sBuffer) const;
    std::string             Read(long nPosition, long nLen) const;
  // Write: 
    void                    Clear();
    long                    Trim(long nLen);
    long                    Append(const char* pBuffer, long nLen = -1);
    long                    Write(long nLocation, const char* pBuffer, long nLen = -1);
    long                    Erase(long nLocation, long nLen = -1);
    long                    Insert(long nLocation, const char* pBuffer, long nLen = -1);
    long                    Replace(long nLocation, long nLenSource, const char* pBuffer, long nLenReplace = -1);
// Print: 
    void                    Print(long nOffset = 0, long nLength = -1) const;
};

#endif
