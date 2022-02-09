/* ========================================================================== /
// Name:         CMd5.h
// Description:  
// Function:     
// Vertion£º     (1.0)
// Auther:	     
// Corporation:  
// Copyright (c) 
// Compile:      
// Modified      (MM/DD/YY)  (Version) (Purpose)
/=========================================================================== */
#ifndef _CMD5_H
#define _CMD5_H

// Definiton: 
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

//---------------------------------------------------------------------------//
//CMd5 1.0
//---------------------------------------------------------------------------//
// 
const int   MD5_DIGEST_LEN      = 16;
const int   MD5_DIGEST_HEX_LEN  = 32;

// 
typedef struct
{
    uint32 total[2];
    uint32 state[4];
    uint8  buffer[64];
}tMd5Context;

// 
class CMd5
{
private: 
    uint8       m_arrPadding[64];
    // 
    tMd5Context m_stContext;
    // 
    void        GetUnit32();
    // 
    void        Process(uint8 arrData[64]);
public: 
    CMd5();
    ~CMd5();
// 
    void        Start();
    void        Update(uint8 *pInput, size_t nLen);
    // Finalize: 
    void        GetDigest(uint8 arrDigest[MD5_DIGEST_LEN], int nSize);
    void        GetDigestHex(char* szHex, int nSize);
    // 
    void        DigestToHex(uint8 digest[MD5_DIGEST_LEN], int nSizeBin, char* szHex, int nSizeHex);
};

#endif
