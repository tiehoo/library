/* ========================================================================== /
// Name:         CMd5.cpp
// Description:  
// Function:     
// Vertion£º     (1.0)
// Auther:	     
// Corporation:  
// Copyright (c) 
// Compile:      
// Modified      (MM/DD/YY)  (Version) (Purpose)
/=========================================================================== */
#include <string.h>
#include "CException.h"
#include "CMd5.h"

//---------------------------------------------------------------------------//
// CMd5 1.0
//---------------------------------------------------------------------------//
#define GET_MD5_UINT32(n, b, i)                 \
{                                               \
    (n) = ( (uint32) (b)[(i)    ]       )       \
        | ( (uint32) (b)[(i) + 1] <<  8 )       \
        | ( (uint32) (b)[(i) + 2] << 16 )       \
        | ( (uint32) (b)[(i) + 3] << 24 );      \
}

#define PUT_MD5_UINT32(n, b, i)                 \
{                                               \
    (b)[(i)    ] = (uint8) ( (n)       );       \
    (b)[(i) + 1] = (uint8) ( (n) >>  8 );       \
    (b)[(i) + 2] = (uint8) ( (n) >> 16 );       \
    (b)[(i) + 3] = (uint8) ( (n) >> 24 );       \
}

// 
CMd5::CMd5()
{
    // Initialize padding array: 
    bzero(m_arrPadding, sizeof(m_arrPadding));
    m_arrPadding[0] = 0x80;
}

CMd5::~CMd5()
{
}

// 
void CMd5::Start()
{
    m_stContext.total[0] = 0;
    m_stContext.total[1] = 0;
    // 
    m_stContext.state[0] = 0x67452301;
    m_stContext.state[1] = 0xEFCDAB89;
    m_stContext.state[2] = 0x98BADCFE;
    m_stContext.state[3] = 0x10325476;
}

void CMd5::Process(uint8 data[64])
{
    uint32 X[16], A, B, C, D;
    // 
    GET_MD5_UINT32( X[0],  data,  0 );
    GET_MD5_UINT32( X[1],  data,  4 );
    GET_MD5_UINT32( X[2],  data,  8 );
    GET_MD5_UINT32( X[3],  data, 12 );
    GET_MD5_UINT32( X[4],  data, 16 );
    GET_MD5_UINT32( X[5],  data, 20 );
    GET_MD5_UINT32( X[6],  data, 24 );
    GET_MD5_UINT32( X[7],  data, 28 );
    GET_MD5_UINT32( X[8],  data, 32 );
    GET_MD5_UINT32( X[9],  data, 36 );
    GET_MD5_UINT32( X[10], data, 40 );
    GET_MD5_UINT32( X[11], data, 44 );
    GET_MD5_UINT32( X[12], data, 48 );
    GET_MD5_UINT32( X[13], data, 52 );
    GET_MD5_UINT32( X[14], data, 56 );
    GET_MD5_UINT32( X[15], data, 60 );

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

    A = m_stContext.state[0];
    B = m_stContext.state[1];
    C = m_stContext.state[2];
    D = m_stContext.state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P( A, B, C, D,  0,  7, 0xD76AA478 );
    P( D, A, B, C,  1, 12, 0xE8C7B756 );
    P( C, D, A, B,  2, 17, 0x242070DB );
    P( B, C, D, A,  3, 22, 0xC1BDCEEE );
    P( A, B, C, D,  4,  7, 0xF57C0FAF );
    P( D, A, B, C,  5, 12, 0x4787C62A );
    P( C, D, A, B,  6, 17, 0xA8304613 );
    P( B, C, D, A,  7, 22, 0xFD469501 );
    P( A, B, C, D,  8,  7, 0x698098D8 );
    P( D, A, B, C,  9, 12, 0x8B44F7AF );
    P( C, D, A, B, 10, 17, 0xFFFF5BB1 );
    P( B, C, D, A, 11, 22, 0x895CD7BE );
    P( A, B, C, D, 12,  7, 0x6B901122 );
    P( D, A, B, C, 13, 12, 0xFD987193 );
    P( C, D, A, B, 14, 17, 0xA679438E );
    P( B, C, D, A, 15, 22, 0x49B40821 );

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P( A, B, C, D,  1,  5, 0xF61E2562 );
    P( D, A, B, C,  6,  9, 0xC040B340 );
    P( C, D, A, B, 11, 14, 0x265E5A51 );
    P( B, C, D, A,  0, 20, 0xE9B6C7AA );
    P( A, B, C, D,  5,  5, 0xD62F105D );
    P( D, A, B, C, 10,  9, 0x02441453 );
    P( C, D, A, B, 15, 14, 0xD8A1E681 );
    P( B, C, D, A,  4, 20, 0xE7D3FBC8 );
    P( A, B, C, D,  9,  5, 0x21E1CDE6 );
    P( D, A, B, C, 14,  9, 0xC33707D6 );
    P( C, D, A, B,  3, 14, 0xF4D50D87 );
    P( B, C, D, A,  8, 20, 0x455A14ED );
    P( A, B, C, D, 13,  5, 0xA9E3E905 );
    P( D, A, B, C,  2,  9, 0xFCEFA3F8 );
    P( C, D, A, B,  7, 14, 0x676F02D9 );
    P( B, C, D, A, 12, 20, 0x8D2A4C8A );

#undef F
    
#define F(x,y,z) (x ^ y ^ z)

    P( A, B, C, D,  5,  4, 0xFFFA3942 );
    P( D, A, B, C,  8, 11, 0x8771F681 );
    P( C, D, A, B, 11, 16, 0x6D9D6122 );
    P( B, C, D, A, 14, 23, 0xFDE5380C );
    P( A, B, C, D,  1,  4, 0xA4BEEA44 );
    P( D, A, B, C,  4, 11, 0x4BDECFA9 );
    P( C, D, A, B,  7, 16, 0xF6BB4B60 );
    P( B, C, D, A, 10, 23, 0xBEBFBC70 );
    P( A, B, C, D, 13,  4, 0x289B7EC6 );
    P( D, A, B, C,  0, 11, 0xEAA127FA );
    P( C, D, A, B,  3, 16, 0xD4EF3085 );
    P( B, C, D, A,  6, 23, 0x04881D05 );
    P( A, B, C, D,  9,  4, 0xD9D4D039 );
    P( D, A, B, C, 12, 11, 0xE6DB99E5 );
    P( C, D, A, B, 15, 16, 0x1FA27CF8 );
    P( B, C, D, A,  2, 23, 0xC4AC5665 );

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P( A, B, C, D,  0,  6, 0xF4292244 );
    P( D, A, B, C,  7, 10, 0x432AFF97 );
    P( C, D, A, B, 14, 15, 0xAB9423A7 );
    P( B, C, D, A,  5, 21, 0xFC93A039 );
    P( A, B, C, D, 12,  6, 0x655B59C3 );
    P( D, A, B, C,  3, 10, 0x8F0CCC92 );
    P( C, D, A, B, 10, 15, 0xFFEFF47D );
    P( B, C, D, A,  1, 21, 0x85845DD1 );
    P( A, B, C, D,  8,  6, 0x6FA87E4F );
    P( D, A, B, C, 15, 10, 0xFE2CE6E0 );
    P( C, D, A, B,  6, 15, 0xA3014314 );
    P( B, C, D, A, 13, 21, 0x4E0811A1 );
    P( A, B, C, D,  4,  6, 0xF7537E82 );
    P( D, A, B, C, 11, 10, 0xBD3AF235 );
    P( C, D, A, B,  2, 15, 0x2AD7D2BB );
    P( B, C, D, A,  9, 21, 0xEB86D391 );

#undef F

    m_stContext.state[0] += A;
    m_stContext.state[1] += B;
    m_stContext.state[2] += C;
    m_stContext.state[3] += D;
}

void CMd5::Update(uint8 *input, size_t length)
{
    try
    {
        if(NULL == input) throw CException(-1, "%s(%ld): CMd5::Update(): NULL pointer.", __FILE__, __LINE__);
        if( ! length ) return;
    
        uint32 left, fill;
    
        left = m_stContext.total[0] & 0x3F;
        fill = 64 - left;
    
        m_stContext.total[0] += length;
        m_stContext.total[0] &= 0xFFFFFFFF;
    
        if( m_stContext.total[0] < length )
            m_stContext.total[1]++;
    
        if( left && length >= fill )
        {
            memcpy( (void *) (m_stContext.buffer + left),
                    (void *) input, fill );
            Process(m_stContext.buffer);
            length -= fill;
            input  += fill;
            left = 0;
        }
    
        while( length >= 64 )
        {
            Process(input);
            length -= 64;
            input  += 64;
        }
    
        if( length )
        {
            memcpy( (void *) (m_stContext.buffer + left),
                    (void *) input, length );
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMd5::Update(uint8 *, size_t): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMd5::GetDigest(uint8 digest[MD5_DIGEST_LEN], int nSize)
{
    try
    {
        if(NULL == digest) throw CException(-1, "%s(%ld): CMd5::GetDigest(uint8, int): NULL pointer.", __FILE__, __LINE__);
        if(nSize < MD5_DIGEST_LEN) throw CException(-1, "%s(%ld): CMd5::GetDigest(uint8, int): Digest buffer too small: %d, need 16 bytes at least.", __FILE__, __LINE__, nSize);
        
        uint32 last, padn;
        uint32 high, low;
        uint8  msglen[8];
    
        high = ( m_stContext.total[0] >> 29 )
             | ( m_stContext.total[1] <<  3 );
        low  = ( m_stContext.total[0] <<  3 );
    
        PUT_MD5_UINT32( low,  msglen, 0 );
        PUT_MD5_UINT32( high, msglen, 4 );
    
        last = m_stContext.total[0] & 0x3F;
        padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
    
        Update(m_arrPadding, padn );
        Update(msglen, 8 );
    
        PUT_MD5_UINT32( m_stContext.state[0], digest,  0 );
        PUT_MD5_UINT32( m_stContext.state[1], digest,  4 );
        PUT_MD5_UINT32( m_stContext.state[2], digest,  8 );
        PUT_MD5_UINT32( m_stContext.state[3], digest, 12 );
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMd5::GetDigest(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMd5::DigestToHex(uint8 digest[MD5_DIGEST_LEN], int nSizeBin, char* szHex, int nSizeHex)
{
    try
    {
        // 
        if(NULL == digest || NULL == szHex) 
        {
            throw CException(-1, "%s(%ld): CMd5::DigestToHex(uint8, int, char*, int): NULL pointer.", __FILE__, __LINE__);
        }
        if(nSizeBin != MD5_DIGEST_LEN)
        {
            throw CException(-1, "%s(%ld): CMd5::DigestToHex(uint8, int, char*, int): Wrong digest size: %d, digest size should be %d.", __FILE__, __LINE__, 
                nSizeBin, MD5_DIGEST_LEN);
    	}
    	if(nSizeHex <= MD5_DIGEST_HEX_LEN)
    	{
    	    throw CException(-1, "%s(%ld): CMd5::DigestToHex(uint8, int, char*, int): szHex too small: %d, at least need %d bytes.", __FILE__, __LINE__, 
    	        nSizeHex, MD5_DIGEST_HEX_LEN + 1);
    	}
    	// 
    	unsigned short i;
    	unsigned char j;
        // 
    	for (i = 0; i < MD5_DIGEST_LEN; i++)
    	{
    		j = (digest[i] >> 4) & 0xf;
    		if(j <= 9) szHex[i*2] = (j + '0');
    		else szHex[i*2] = (j + 'a' - 10);
    		j = digest[i] & 0xf;
    		if(j <= 9) szHex[i*2+1] = (j + '0');
    		else szHex[i*2+1] = (j + 'a' - 10);
    	}
    	szHex[MD5_DIGEST_HEX_LEN] = '\0';
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMd5::DigestToHex(uint8, int, char*, int): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CMd5::GetDigestHex(char* szHex, int nSize)
{
    try
    {
        // 
        if(NULL == szHex)
        {
            throw CException(-1, "%s(%ld): CMd5::GetDigestHex(char*, int): NULL pointer.", __FILE__, __LINE__);
    	}
    	if(nSize <= MD5_DIGEST_HEX_LEN)
    	{
    	    throw CException(-1, "%s(%ld): CMd5::GetDigestHex(): szHex too small: %d, at least need 33 bytes.", __FILE__, __LINE__, nSize);
    	}
    	// Get digest first: 
    	uint8 digest[MD5_DIGEST_LEN];
    	GetDigest(digest, sizeof(digest));
    	
    	// Convert to hex: 
    	DigestToHex(digest, sizeof(digest), szHex, nSize);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CMd5::GetDigestHex(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
