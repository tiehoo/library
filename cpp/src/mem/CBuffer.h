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
#ifndef _CBUFFER_H
#define _CBUFFER_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include "CBufferFrame.h"

/* --------------------------------------------------------------------------- /
//CBuffer 1.0 
/ --------------------------------------------------------------------------- */
//Constants: 

// 
class CBuffer : public CBufferFrame
{
protected: 
    // Allocate the buffer as nSize: 
    size_t          Allocate(long nSize);
public: 
    explicit        CBuffer(long nSize);
    explicit        CBuffer(const char* szBuffer);
    explicit        CBuffer(const CBuffer& objBuffer);
                    CBuffer(const char* pBuffer, long nSize);
    ~CBuffer();
// 
    // Copy data to buffer: 
    void            SetBuffer(const char *szBuffer);
    void            SetBuffer(const char *pBuffer, long nLen);
    void            SetBuffer(const CBuffer& objBuffer);
// 
    // You must terminate pBuf by '\0'.
    const CBuffer&  operator = (const CBuffer& objBuffer);
    const CBuffer&  operator = (const char *szBuffer);
// 
    const char*     GetBufferPointer(long lOffset = 0) const;
    const char*     c_str() const;
};

#endif
