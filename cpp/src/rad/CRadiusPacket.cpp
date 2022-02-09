/* =========================================================================== /
* @category     RADIUS
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
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
// 
#include "CException.h"
#include "CMd5.h"
#include "CDateTime.h"
#include "CRadiusPacket.h"
#include "CUtility.h"

// --------------------------------------------------------------------------- /
// CRadiusPacket 1.0
// --------------------------------------------------------------------------- /
CRadiusPacket::CRadiusPacket() 
{
    try
    {
        ClearPacket();
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::CRadiusPacket(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
// 
CRadiusPacket::CRadiusPacket(std::string sSecret)
{
    try
    {
        ClearPacket();
        SetSecret(sSecret);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::CRadiusPacket(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CRadiusPacket::~CRadiusPacket()
{
}

// --------------------------------------------------------------------------- /
// Private: 
void CRadiusPacket::SetLength(int nLength)
{
    uint16_t nLenNetOrder;
    // No more than: 65535.
    if(nLength < RAD_PACKET_HEADER_LEN || nLength >= RAD_PACKET_HEADER_LEN + RAD_MAX_ATTRIBUTE_AREA_LEN)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetLength(): Invalid length: %d, should between %d and %d.", __FILE__, __LINE__, 
            nLength, RAD_PACKET_HEADER_LEN, RAD_PACKET_HEADER_LEN + RAD_MAX_ATTRIBUTE_AREA_LEN);
    }
    // Convert to network byte order: 
    // 16 bit order: 
    nLenNetOrder = htons(nLength);
    memcpy(&m_stPacket.nLength, &nLenNetOrder, RAD_PACKET_LENGTH_LEN);
}

/*
// 
int CRadiusPacket::GenerateRandomAuthenticator(uint8_t nIdentifier, uint8_t* pAuthenticator, int nLength) const
{
    if(nLength < RAD_AUTHENTICATOR_LEN) 
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::GenerateRandomAuthenticator(): Buffer is too small %d, at least need %d bytes.", __FILE__, __LINE__, 
            nLength, RAD_AUTHENTICATOR_LEN);
    }

    // Generate an authenticator according to last identifier and current time: 
    int i;
    CDateTime objDateTime;  // Set to now already.
    srand(objDateTime.GetMicroSecond() + nIdentifier);
    for(i = 0; i < RAD_AUTHENTICATOR_LEN; i++) 
    {
        pAuthenticator[i] = (int)(95.0 * (rand()/(RAND_MAX + 1.0)));
    }
    return(i);
}
*/
void CRadiusPacket::GenerateRandomAuthenticator()
{
    // Generate an authenticator according to last identifier and current time: 
    int i;
    CDateTime objDateTime;  // Set to now already.
    srand(objDateTime.GetMicroSecond() + GetIdentifier());
    for(i = 0; i < RAD_AUTHENTICATOR_LEN; i++) 
    {
        m_stPacket.arrAuthenticator[i] = (int)(95.0 * (rand()/(RAND_MAX + 1.0)));
    }
}


// 
bool CRadiusPacket::FindAttribute(uint8_t nType, tRadiusAttribute& stAttribute) const
{
    int nLenRead = 0;
    uint8_t* pAttr;
    // 
    pAttr = (uint8_t*)&m_stPacket.arrAttribute;
    while(1)
    {
        // Go through all attributes: 
        if(nLenRead >= GetAttributeLength()) break;   // I don't want to read out of the area.
        // 
        if(((tRadiusAttribute*)pAttr)->nType == nType)
        {
            stAttribute.nType   = ((tRadiusAttribute*)pAttr)->nType;
            stAttribute.nLength = ((tRadiusAttribute*)pAttr)->nLength;
            memcpy(stAttribute.arrValue, ((tRadiusAttribute*)pAttr)->arrValue, ((tRadiusAttribute*)pAttr)->nLength - 2);
            // 
            return(true);
        }
        // No need to convert nLength to host byte order, it's 8 bit.
        nLenRead += ((tRadiusAttribute*)pAttr)->nLength;
        // 
        pAttr += ((tRadiusAttribute*)pAttr)->nLength;
    }
    return(false);
}

// Public: 
// --------------------------------------------------------------------------- /
void CRadiusPacket::ClearPacket()
{
    bzero(&m_stPacket, sizeof(m_stPacket));
    SetLength(RAD_PACKET_HEADER_LEN);
    // 
    m_sSecret.clear();
}

void CRadiusPacket::ClearAttribute()
{
    bzero(&(m_stPacket.arrAttribute), RAD_MAX_ATTRIBUTE_AREA_LEN);
    SetLength(RAD_PACKET_HEADER_LEN);
}

const CRadiusPacket& CRadiusPacket::operator = (const CRadiusPacket& objRadiusPacket)
{
    memcpy(&m_stPacket, &objRadiusPacket.m_stPacket, sizeof(m_stPacket));
}

void CRadiusPacket::SetSecret(std::string sSecret)
{
    if(sSecret.empty()) 
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetSecret(): sSecret is empty.", __FILE__, __LINE__);
    }
    // 
    m_sSecret = sSecret;
}

bool CRadiusPacket::IsSecretSet() const
{
    return(!m_sSecret.empty());
}

// --------------------------------------------------------------------------- /
/*
      Call the shared secret S and the pseudo-random 128-bit Request
      Authenticator RA.  Break the password into 16-octet chunks p1, p2,
      etc.  with the last one padded at the end with nulls to a 16-octet
      boundary.  Call the ciphertext blocks c(1), c(2), etc.  We'll need
      intermediate values b1, b2, etc.

         b1 = MD5(S + RA)       c(1) = p1 xor b1
         b2 = MD5(S + c(1))     c(2) = p2 xor b2
                .                       .
                .                       .
                .                       .
         bi = MD5(S + c(i-1))   c(i) = pi xor bi

      The String will contain c(1)+c(2)+...+c(i) where + denotes
      concatenation.
*/
int CRadiusPacket::EncryptPassword(const char *szPassword, uint8_t *pPassEncrypted, int nSize) const
{
    try
    {
        if(NULL == szPassword || NULL == pPassEncrypted)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): NULL pointer.", __FILE__, __LINE__);
        }
        if(nSize <= 0)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): Invalid buffer length: %d.", __FILE__, __LINE__, nSize);
        }
        // 
        int nLen, nPassLen, nLenCopy;
        unsigned char   bi[16], pi[16], ci[16];
        unsigned char   arrHarsh[256];  // Store [secret + bi]
        CMd5            objMd5;
        
        // Check shared secret: 
        if(m_sSecret.length() <= 0)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): No shared secret set.", __FILE__, __LINE__);
        }
        // Protect arrHarsh: 
        if(m_sSecret.length() + sizeof(bi) >= sizeof(arrHarsh))
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): Internal error, shared secret too long: %d, no more than: %d.", __FILE__, __LINE__, 
                m_sSecret.length(), sizeof(arrHarsh) - sizeof(bi));
        }
        
        // Copy secret to harsh source: 
        memcpy(arrHarsh, m_sSecret.c_str(), m_sSecret.length());

        // Handle every 16-octet: 
        nLen = 0;
        nPassLen = strlen(szPassword);
        for(int i = 0; ; i++)
        {
            // MD5 source: 
            if(0 == i)
            {
                // S+RA: 
                memcpy(&arrHarsh[m_sSecret.length()], m_stPacket.arrAuthenticator, RAD_AUTHENTICATOR_LEN);
                nLenCopy = m_sSecret.length() + RAD_AUTHENTICATOR_LEN;
            }else
            {
                // S+c(i-1)
                memcpy(&arrHarsh[m_sSecret.length()], ci, 16);
                nLenCopy = m_sSecret.length() + 16;
            }
            
            // bi: 
            objMd5.Start();
            objMd5.Update(arrHarsh, nLenCopy);
            objMd5.GetDigest(bi, sizeof(bi));

            // Cut pi: 
            memset(pi, '\0', 16);   // Initialize and add padding '0';
            if(nPassLen > (i+1)*16) nLenCopy = 16;
            else nLenCopy = 16 - ((i+1)*16 - nPassLen);
            // 
            if(nLenCopy <= 0) break;    // No more, break;
            memcpy(pi, &szPassword[i*16], nLenCopy);

            // Calculate ci trunk: 
            if((i+1)*16 >= nSize)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): pPassEncrypted overflowed, trying to copy %d bytes, but only %d bytes.", __FILE__, __LINE__, 
                    (i+1)*16, nSize);
            }
            // XOR 2 bytes: 
            CUtility::Xor(bi, pi, ci, 16);
            memcpy(&pPassEncrypted[i*16], ci, 16);
            nLen += 16;

            // 
            if(nLenCopy < 16) break;    // No more, break;
        }
        // Return length: 
        return(nLen);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::EncryptPassword(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// bi = MD5(S + c(i-1))   c(i) = pi xor bi
void CRadiusPacket::DecryptPassword(const uint8_t *pPassEncrypted, int nPassLen, char* szPassword, int nSize) const
{
    try
    {
        if(NULL == pPassEncrypted || NULL == szPassword)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): NULL pointer.", __FILE__, __LINE__);
        }
        if(nSize <= 0)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::DecryptPassword(): Invalid buffer length: %d.", __FILE__, __LINE__, nSize);
        }
        if(nPassLen <= 0 || nPassLen > 128)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::DecryptPassword(): Invalid pass length: %d, should between 1 and 128.", __FILE__, __LINE__, nPassLen);
        }
        // Encrypted password's length must be 16 * n;
        if(nPassLen%16 != 0)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::DecryptPassword(): Encrypted password length should be 16's integer times, but: %d.", __FILE__, __LINE__, nPassLen);
        }
        // 
        int nLen, nLenCopy;
        unsigned char   bi[16], pi[16], ci[16];
        unsigned char   arrHarsh[256];          // Store [secret + bi]
        CMd5            objMd5;
        
        // Check shared secret: 
        if(m_sSecret.length() <= 0)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): No shared secret set.", __FILE__, __LINE__);
        }
        // Protect arrHarsh: 
        if(m_sSecret.length() + sizeof(bi) >= sizeof(arrHarsh))
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): Internal error, shared secret too long: %d, no more than: %d.", __FILE__, __LINE__, 
                m_sSecret.length(), sizeof(arrHarsh) - sizeof(bi));
        }
        
        // Copy secret to harsh source: 
        memcpy(arrHarsh, m_sSecret.c_str(), m_sSecret.length());

        // Handle every 16-octet: 
        nLen = 0;
        for(int i = 0; ; ++i)
        {
            // MD5 source: 
            if(0 == i)
            {
                // S+RA: 
                memcpy(&arrHarsh[m_sSecret.length()], m_stPacket.arrAuthenticator, RAD_AUTHENTICATOR_LEN);
                nLenCopy = m_sSecret.length() + RAD_AUTHENTICATOR_LEN;
            }else
            {
                // S+c(i-1)
                memcpy(&arrHarsh[m_sSecret.length()], ci, 16);
                nLenCopy = m_sSecret.length() + 16;
            }
            
            // bi: 
            objMd5.Start();
            objMd5.Update(arrHarsh, nLenCopy);
            objMd5.GetDigest(bi, sizeof(bi));

            // Cut ci: 
            memset(ci, '\0', 16);                           // Initialize and add padding '0';
            if(nPassLen > (i+1)*16) nLenCopy = 16;
            else nLenCopy = 16 - ((i+1)*16 - nPassLen);
            if(nLenCopy <= 0) break;                        // No more, break;
            memcpy(ci, &pPassEncrypted[i*16], nLenCopy);

            // Calculate pi trunk: 
            if((i+1)*16 >= nSize)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::EncryptPassword(): szPassword overflowed, trying to copy %d bytes, but only %d.", __FILE__, __LINE__, 
                    (i+1)*16, nSize);
            }
            // XOR 2 bytes: 
            CUtility::Xor(bi, ci, pi, 16);
            memcpy(&szPassword[i*16], pi, 16);
            nLen += 16;
            // 
            if(nLenCopy < 16) break;                        // No more, break;
        }
        szPassword[nLen] = '\0';
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::DecryptPassword(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// -----------------------------------------------------------------------------
// Set a packet to read: 
void CRadiusPacket::SetPacket(const uint8_t *pPacket, int nLength)
{
    if(NULL == pPacket)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetPacket(): NULL pointer.", __FILE__, __LINE__);
    }
    if(nLength < 0)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetPacket(): Invlaid packet length: %d.", __FILE__, __LINE__, nLength);
    }
    if(nLength >= sizeof(m_stPacket))
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetPacket(): Packet too long: %d.", __FILE__, __LINE__, nLength);
    }
    // 
    memcpy(&m_stPacket, pPacket, nLength);
}

// 
void CRadiusPacket::SetCode(uint8_t nCode)
{
    m_stPacket.nCode = nCode;
}

// If retransmit, keep the same identifier: 
void CRadiusPacket::SetIdentifier(uint8_t nIdentifier)
{
    // No more than: 255
    m_stPacket.nIdentifier = nIdentifier;
}

// 
void CRadiusPacket::SetAuthenticator(const uint8_t *pAuthenticator, int nLength)
{
    if(nLength < RAD_AUTHENTICATOR_LEN)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetAuthenticator(): Length is too small %d, at least need 16 bytes.", __FILE__, __LINE__, 
            nLength, RAD_AUTHENTICATOR_LEN);
    }
    // 
    if(NULL == pAuthenticator)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::SetAuthenticator(): NULL pointer.", __FILE__, __LINE__);
    }
    // 
    memcpy(m_stPacket.arrAuthenticator, pAuthenticator, RAD_AUTHENTICATOR_LEN);
}

// Must call SetIdentifier before calling this function: 
/*
void CRadiusPacket::SetRandomAuthenticator()
{
    try
    {
        uint8_t arrAuthenticator[RAD_AUTHENTICATOR_LEN];
        GenerateRandomAuthenticator(GetIdentifier(), arrAuthenticator, sizeof(arrAuthenticator));
        SetAuthenticator(arrAuthenticator);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::SetRandomAuthenticator(): Falied.", __FILE__, __LINE__);
        throw eEx;
    }
}
*/

// Here, the nLength is the length of the value, not the attribute structure. 
int CRadiusPacket::AddAttribute(uint8_t nType, int nLength, const uint8_t* pValue, char cKind)
{
    try
    {
        // 1. Generic checking: 
        if(NULL == pValue)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, uint8_t, uint8_t*, char): NULL pointer.", __FILE__, __LINE__);
        }
        if(nLength == 0) return(0);    // Don't add 0 length attribute.
        if(nLength < 0) 
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, uint8_t, uint8_t*, char): Invalid attribute length: %d, should between 0 and %d.", __FILE__, __LINE__, 
                nLength, RAD_MAX_ATTRIBUTE_LEN);
        }
        
        // 3. Do the conversion: 
        uint8_t arrTemp[RAD_MAX_ATTRIBUTE_LEN];
        switch(cKind)
        {
        case RAD_ATTR_KIND_IP: 
            // Convert IP: 
            if(inet_pton(AF_INET, (char *)pValue, &arrTemp) <= 0)
            {
                char szErr[256];
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): inet_pton() failed, type: [%d], address: [%s], [%d]:[%s]", __FILE__, __LINE__, 
                    nType, pValue, errno, strerror_r(errno, szErr, sizeof(szErr)));
            }
            nLength = 4;
            break;
        case RAD_ATTR_KIND_ULONG: 
            if(nLength != 4)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): Not a uint32_t pointer, length should be 4, but get: %d.", __FILE__, __LINE__, nLength);
            }
            uint32_t lValue, lResult;
            lValue = *(uint32_t *)pValue;
            lResult = htonl(lValue);
            memcpy(arrTemp, &lResult, 4);
            break;
        case RAD_ATTR_KIND_STRING: 
            // Copy attribute: Protect before copying.
            if(nLength > RAD_MAX_ATTRIBUTE_LEN) 
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, uint8_t, uint8_t*, char): Invalid attribute length: %d, should between 0 and %d.", __FILE__, __LINE__, 
                    nLength, RAD_MAX_ATTRIBUTE_LEN);
            }
            memcpy(arrTemp, pValue, nLength);
            break;
        default: 
            throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, const uint8_t*, char): Invalid kind: [%c].", __FILE__, __LINE__, cKind);
        }
        
        // 4. Treat special fields: 
        switch(nType)
        {
        case RAD_ATTR_TYPE_USERPASSWORD:            // Encrypt user password: 
            // [2]:User-Password: 
            nLength = EncryptPassword((char *)pValue, arrTemp, sizeof(arrTemp));
            break;
        }

        // Change value length to attribute length: 
        nLength += 2;   // Add type and length.
        
        // Check field length: 
        switch(nType)
        {
        case RAD_ATTR_TYPE_USERNAME: 
        case RAD_ATTR_TYPE_CALLEDSTATIONID: 
        case RAD_ATTR_TYPE_CALLINGSTATIONID: 
        case RAD_ATTR_TYPE_ACCTSESSIONID: 
            // [1]:User-Name: >= 3
            // [30]:Called-Station-Id: >= 3
            // [31]:Calling-Station-Id: >= 3
            // [44]:Acct-Session-Id: >= 3
            if(nLength < 3)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): User-Name/Called-Station-Id/Calling-Station-Id/Acct-Session-Id: Length should between 3 and %d, type: [%d], value: [%s].", __FILE__, __LINE__, RAD_MAX_ATTRIBUTE_LEN, nType, pValue);
            }
            break;
        case RAD_ATTR_TYPE_USERPASSWORD: 
            // [2]:User-Password: >= 18 and <= 130
            if(nLength < 18 || nLength > 130)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): User-Password: After encryption, length should between 18 and 130, but get: [%d].", __FILE__, __LINE__, nLength);
            }
            break;
        case RAD_ATTR_TYPE_NASIPADDRESS: 
            // [4]:NAS-IP-Address: = 6
        case RAD_ATTR_TYPE_SESSIONTIMEOUT: 
            // [27]: Session-Timeout: = 6
            if(nLength != 6)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): NAS-IP-Address/Session-Timeout/Acct-Status-Type: Length should be 6, but get: [%d], type: [%d].", __FILE__, __LINE__, nLength, nType);
            }
            break;
        }
        
        // Check overflow: 
        if(GetAttributeLength() + nLength >= RAD_MAX_ATTRIBUTE_AREA_LEN)
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): Attribute structure overflowed when adding attribute: type: %d, length: %d, current length is: %d, no more than: %d.", __FILE__, __LINE__, 
                nType, nLength, GetAttributeLength(), RAD_MAX_ATTRIBUTE_AREA_LEN - 1);
        }
        
        // Copy attribute: 
        tRadiusAttribute* pAttr;
        pAttr = (tRadiusAttribute*)&(m_stPacket.arrAttribute[GetAttributeLength()]);
        // Type: 
        pAttr->nType = nType;
        // Length: 
        pAttr->nLength = (uint8_t)nLength;
        memcpy((pAttr->arrValue), arrTemp, pAttr->nLength - 2);
        
        // Increase packet length: 
        SetLength(GetLength() + nLength);
        // 
        return(nLength);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::AddAttribute(uint8_t, int, uint8_t*, char): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// -----------------------------------------------------------------------------
const uint8_t* CRadiusPacket::GetPacket() const
{
    return((uint8_t*)&m_stPacket);
}

int CRadiusPacket::GetLength() const
{
    uint16_t nLenNetOrder;
    memcpy(&nLenNetOrder, &m_stPacket.nLength, RAD_PACKET_LENGTH_LEN);
    return(ntohs(nLenNetOrder));
}

// 
uint8_t* CRadiusPacket::GetBuffer()
{
    return((uint8_t*)&m_stPacket);
}

int CRadiusPacket::GetSize() const
{
    return(sizeof(m_stPacket));
}

// -----------------------------------------------------------------------------
uint8_t CRadiusPacket::GetCode() const
{
    return(m_stPacket.nCode);
}

uint8_t CRadiusPacket::GetIdentifier() const
{
    // 
    return(m_stPacket.nIdentifier);
}

void CRadiusPacket::GetAuthenticator(uint8_t *pRequestAuthenticator, int nSize) const
{
    if(NULL == pRequestAuthenticator)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::GetAuthenticator(): NULL pointer.", __FILE__, __LINE__);
    }
    if(nSize < RAD_AUTHENTICATOR_LEN)
    {
        throw CException(-1, "%s(%ld): CRadiusPacket::GetAuthenticator(): Buffer too small, only need %d bytes, at least need: %d bytes.", __FILE__, __LINE__, 
            nSize, RAD_AUTHENTICATOR_LEN);
    }
    // 
    memcpy(pRequestAuthenticator, m_stPacket.arrAuthenticator, RAD_AUTHENTICATOR_LEN);
}

const uint8_t* CRadiusPacket::GetAuthenticator() const 
{
    return(m_stPacket.arrAuthenticator);
}

int CRadiusPacket::GetAttributeLength() const
{
    return(GetLength() - RAD_PACKET_HEADER_LEN);
}

// -----------------------------------------------------------------------------
bool CRadiusPacket::GetAttribute(uint8_t nType, tRadiusAttribute& stAttribute) const
{
    try
    {
        return(FindAttribute(nType, stAttribute));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::GetAttribute(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusPacket::GetAttributeString(uint8_t nType, std::string& sValue) const
{
    try
    {
        bool bFound = false;
        // 
        tRadiusAttribute stAttribute;
        if((bFound = FindAttribute(nType, stAttribute)))
        {
            // 
            if(RAD_ATTR_TYPE_USERPASSWORD == nType)
            {
                // Decrypt password: 
                char szValue[128];
                DecryptPassword(stAttribute.arrValue, stAttribute.nLength - 2, szValue, sizeof(szValue));
                sValue = szValue;
            }else
            {
                stAttribute.arrValue[stAttribute.nLength - 2] = '\0';
                sValue = (char *)stAttribute.arrValue;
            }
        }else sValue.clear();
        // 
        return(bFound);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::GetAttributeString(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusPacket::GetAttributeLong(uint8_t nType, unsigned long& lValue) const
{
    try
    {
        bool bFound = false;
        // 
        tRadiusAttribute stAttribute;
        if((bFound = FindAttribute(nType, stAttribute)))
        {
            if(stAttribute.nLength != 6)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::GetAttributeLong(): Invalid length: %d, should be 6, this type is not a long value.", __FILE__, __LINE__, 
                    stAttribute.nLength);
            }
            // 
            uint32_t lUint32;
            memcpy(&lUint32, stAttribute.arrValue, stAttribute.nLength - 2);
            lValue = ntohl(lUint32);
        }
        // 
        return(bFound);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::GetAttributeLong(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CRadiusPacket::GetAttributeIp(uint8_t nType, std::string& sValue) const
{
    try
    {
        bool bFound = false;
        // 
        tRadiusAttribute stAttribute;
        if((bFound = FindAttribute(nType, stAttribute)))
        {
            if(stAttribute.nLength != 6)
            {
                throw CException(-1, "%s(%ld): CRadiusPacket::GetAttributeIp(): Invalid length: %d, should be 6, this type is not a IP value.", __FILE__, __LINE__, 
                    stAttribute.nLength);
            }
            // 
            char szBuf[64];
        	if(inet_ntop(AF_INET, stAttribute.arrValue, szBuf, sizeof(szBuf)) == NULL)
        	{
                char szErr[256];
                throw CException(-1, "%s(%ld): CRadiusPacket::GetAttributeIp(): inet_ntop() failed, not an valid IP value, type: [%d], [%d]:[%s]", __FILE__, __LINE__, 
                    nType, errno, strerror_r(errno, szErr, sizeof(szErr)));
        	}else sValue = szBuf;
        }
        // 
        return(bFound);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::GetAttributeIp(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// Calculate and set authenticator by the current packet: MD5(Code+ID+Length+Authenticator+Attributes+Secret);
int CRadiusPacket::CalculateAuthenticator(uint8_t *pRequestAuthenticator, int nLength) const 
{
    try
    {
        if(nLength < RAD_AUTHENTICATOR_LEN) 
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::CalculateAuthenticator(): Buffer is too small %d, at least need %d bytes.", 
                __FILE__, __LINE__, nLength, RAD_AUTHENTICATOR_LEN);
        }
        // 
        if(!IsSecretSet()) 
        {
            throw CException(-1, "%s(%ld): CRadiusPacket::CalculateAuthenticator(): Secret is empty.", __FILE__, __LINE__);
        }
        // 
        CMd5    objMd5;
        objMd5.Start();
        // Update packet: 
        objMd5.Update((unsigned char*)&m_stPacket, GetLength());
        // Update secret: 
        objMd5.Update((unsigned char*)m_sSecret.c_str(), m_sSecret.length());
        // Set check sum: 
        objMd5.GetDigest((unsigned char*)pRequestAuthenticator, RAD_AUTHENTICATOR_LEN);
        // 
        return(sizeof(m_stPacket.arrAuthenticator));
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::SetAuthenticator(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CRadiusPacket::IsPacketValid(const uint8_t *pOriginalAuthenticator, int nLength)
{
    try
    {
        // 
        unsigned char arrAuthenticator[RAD_AUTHENTICATOR_LEN], arrAuthenticatorNew[RAD_AUTHENTICATOR_LEN];
        // 1. Save current authenticator: 
        GetAuthenticator(arrAuthenticator, sizeof(arrAuthenticator));
        // 2. Set original authenticator: 
        SetAuthenticator(pOriginalAuthenticator, nLength);
        // 3. Calculate: 
        CalculateAuthenticator(arrAuthenticatorNew, sizeof(arrAuthenticatorNew));
        // 4. Restore: 
        SetAuthenticator(arrAuthenticator, sizeof(arrAuthenticator));
        // 5. Compare: 
        if(0 == memcmp(arrAuthenticator, arrAuthenticatorNew, RAD_AUTHENTICATOR_LEN))
        {
            return(true);
        }else 
        {
            return(false);
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CRadiusPacket::IsPacketValid(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
