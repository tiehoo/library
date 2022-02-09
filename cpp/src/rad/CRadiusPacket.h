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
#ifndef _CRADIUSPACKET_H
#define _CRADIUSPACKET_H

// RFC2865: http://www.ietf.org/rfc/rfc2865.txt
/*
A summary of the RADIUS data format is shown below.  The fields are
   transmitted from left to right.
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Code      |  Identifier   |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                         Authenticator                         |
   |                                                               |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Attributes ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-
# 
      RADIUS Codes (decimal) are assigned as follows:

        1       Access-Request
        2       Access-Accept
        3       Access-Reject
        4       Accounting-Request
        5       Accounting-Response
       11       Access-Challenge
       12       Status-Server (experimental)
       13       Status-Client (experimental)
      255       Reserved
*/
/*
   A summary of the Attribute format is shown below.  The fields are
   transmitted from left to right.
    0                   1                   2
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
   |     Type      |    Length     |  Value ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
# 
      This specification concerns the following values:

          1      User-Name
          2      User-Password
          3      CHAP-Password
          4      NAS-IP-Address
          5      NAS-Port
          6      Service-Type
          7      Framed-Protocol
          8      Framed-IP-Address
          9      Framed-IP-Netmask
         10      Framed-Routing
         11      Filter-Id
         12      Framed-MTU
         13      Framed-Compression
         14      Login-IP-Host
         15      Login-Service
         16      Login-TCP-Port
         17      (unassigned)
         18      Reply-Message
         19      Callback-Number
         20      Callback-Id
         21      (unassigned)
         22      Framed-Route
         23      Framed-IPX-Network
         24      State
         25      Class
         26      Vendor-Specific
         27      Session-Timeout
         28      Idle-Timeout
         29      Termination-Action
         30      Called-Station-Id
         31      Calling-Station-Id
         32      NAS-Identifier
         33      Proxy-State
         34      Login-LAT-Service
         35      Login-LAT-Node
         36      Login-LAT-Group
         37      Framed-AppleTalk-Link
         38      Framed-AppleTalk-Network
         39      Framed-AppleTalk-Zone
         40-59   (reserved for accounting)
         60      CHAP-Challenge
         61      NAS-Port-Type
         62      Port-Limit
         63      Login-LAT-Port

      Request Authenticator

         In Access-Request Packets, the Authenticator value is a 16
         octet random number, called the Request Authenticator.  The
         value SHOULD be unpredictable and unique over the lifetime of a
         secret (the password shared between the client and the RADIUS
         server), since repetition of a request value in conjunction
         with the same secret would permit an attacker to reply with a
         previously intercepted response.  Since it is expected that the
         same secret MAY be used to authenticate with servers in
         disparate geographic regions, the Request Authenticator field
         SHOULD exhibit global and temporal uniqueness.

         The Request Authenticator value in an Access-Request packet
         SHOULD also be unpredictable, lest an attacker trick a server
         into responding to a predicted future request, and then use the
         response to masquerade as that server to a future Access-
         Request.

         Although protocols such as RADIUS are incapable of protecting
         against theft of an authenticated session via realtime active
         wiretapping attacks, generation of unique unpredictable
         requests can protect against a wide range of active attacks
         against authentication.
         
      Response Authenticator

         The value of the Authenticator field in Access-Accept, Access-
         Reject, and Access-Challenge packets is called the Response
         Authenticator, and contains a one-way MD5 hash calculated over
         a stream of octets consisting of: the RADIUS packet, beginning
         with the Code field, including the Identifier, the Length, the
         Request Authenticator field from the Access-Request packet, and
         the response Attributes, followed by the shared secret.  That
         is, ResponseAuth =
         MD5(Code+ID+Length+RequestAuth+Attributes+Secret) where +
         denotes concatenation.
*/

// RFC 8266: http://www.ietf.org/rfc/rfc2865.txt
/* RADIUS Accounting: 
It is recommended that
   the client continue attempting to send the Accounting-Request packet
   until it receives an acknowledgement, using some form of backoff.  If
   no response is returned within a length of time, the request is re-
   sent a number of times. The client can also forward requests to an
   alternate server or servers in the event that the primary server is
   down or unreachable.  An alternate server can be used either after a
   number of tries to the primary server fail, or in a round-robin
   fashion.

If the RADIUS accounting server is unable to successfully record the
   accounting packet it MUST NOT send an Accounting-Response
   acknowledgment to the client.
*/
/*
3.  Packet Format

   Exactly one RADIUS Accounting packet is encapsulated in the UDP Data
   field [4], where the UDP Destination Port field indicates 1813
   (decimal).

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Code      |  Identifier   |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                         Authenticator                         |
   |                                                               |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Attributes ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-

   Code

      The Code field is one octet, and identifies the type of RADIUS
      packet.  When a packet is received with an invalid Code field, it
      is silently discarded.

      RADIUS Accounting Codes (decimal) are assigned as follows:

           4       Accounting-Request
           5       Accounting-Response

   Length

      The Length field is two octets.  It indicates the length of the
      packet including the Code, Identifier, Length, Authenticator and
      Attribute fields.  Octets outside the range of the Length field
      MUST be treated as padding and ignored on reception.  If the
      packet is shorter than the Length field indicates, it MUST be
      silently discarded.  The minimum length is 20 and maximum length
      is 4095.

   Request Authenticator

      In Accounting-Request Packets, the Authenticator value is a 16
      octet MD5 [5] checksum, called the Request Authenticator.

      The NAS and RADIUS accounting server share a secret.  The Request
      Authenticator field in Accounting-Request packets contains a one-
      way MD5 hash calculated over a stream of octets consisting of the
      Code + Identifier + Length + 16 zero octets + request attributes +
      shared secret (where + indicates concatenation).  The 16 octet MD5
      hash value is stored in the Authenticator field of the
      Accounting-Request packet.

      Note that the Request Authenticator of an Accounting-Request can
      not be done the same way as the Request Authenticator of a RADIUS
      Access-Request, because there is no User-Password attribute in an
      Accounting-Request.

   Response Authenticator

      The Authenticator field in an Accounting-Response packet is called
      the Response Authenticator, and contains a one-way MD5 hash
      calculated over a stream of octets consisting of the Accounting-
      Response Code, Identifier, Length, the Request Authenticator field
      from the Accounting-Request packet being replied to, and the
      response attributes if any, followed by the shared secret.  The
      resulting 16 octet MD5 hash value is stored in the Authenticator
      field of the Accounting-Response packet.

4.1.  Accounting-Request
      Accounting-Request packets are sent from a client (typically a
      Network Access Server or its proxy) to a RADIUS accounting server,
      and convey information used to provide accounting for a service
      provided to a user.  The client transmits a RADIUS packet with the
      Code field set to 4 (Accounting-Request).

      Upon receipt of an Accounting-Request, the server MUST transmit an
      Accounting-Response reply if it successfully records the
      accounting packet, and MUST NOT transmit any reply if it fails to
      record the accounting packet.

      Any attribute valid in a RADIUS Access-Request or Access-Accept
      packet is valid in a RADIUS Accounting-Request packet, except that
      the following attributes MUST NOT be present in an Accounting-
      Request:  User-Password, CHAP-Password, Reply-Message, State.
      Either NAS-IP-Address or NAS-Identifier MUST be present in a
      RADIUS Accounting-Request.  It SHOULD contain a NAS-Port or NAS-
      Port-Type attribute or both unless the service does not involve a
      port or the NAS does not distinguish among its ports.

   A summary of the Accounting-Request packet format is shown below.

   The fields are transmitted from left to right.

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Code      |  Identifier   |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                     Request Authenticator                     |
   |                                                               |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Attributes ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-

   Code

      4 for Accounting-Request.

   Identifier

      The Identifier field MUST be changed whenever the content of the
      Attributes field changes, and whenever a valid reply has been
      received for a previous request.  For retransmissions where the
      contents are identical, the Identifier MUST remain unchanged.

      Note that if Acct-Delay-Time is included in the attributes of an
      Accounting-Request then the Acct-Delay-Time value will be updated
      when the packet is retransmitted, changing the content of the
      Attributes field and requiring a new Identifier and Request
      Authenticator.
5.  Attributes
          1-39    (refer to RADIUS document [2])
          40      Acct-Status-Type
          41      Acct-Delay-Time
          42      Acct-Input-Octets
          43      Acct-Output-Octets
          44      Acct-Session-Id
          45      Acct-Authentic
          46      Acct-Session-Time
          47      Acct-Input-Packets
          48      Acct-Output-Packets
          49      Acct-Terminate-Cause
          50      Acct-Multi-Session-Id
          51      Acct-Link-Count
          60+     (refer to RADIUS document [2])

      The format of the value field is one of five data types.  Note
      that type "text" is a subset of type "string."

      text     1-253 octets containing UTF-8 encoded 10646 [7]
               characters.  Text of length zero (0) MUST NOT be sent;
               omit the entire attribute instead.

      string   1-253 octets containing binary data (values 0 through 255
               decimal, inclusive).  Strings of length zero (0) MUST NOT
               be sent; omit the entire attribute instead.

      address  32 bit value, most significant octet first.

      integer  32 bit unsigned value, most significant octet first.

      time     32 bit unsigned value, most significant octet first --
               seconds since 00:00:00 UTC, January 1, 1970.  The
               standard Attributes do not use this data type but it is
               presented here for possible use in future attributes.
5.1.  Acct-Status-Type

   A summary of the Acct-Status-Type attribute format is shown below.
   The fields are transmitted from left to right.

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |    Length     |             Value
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
              Value (cont)         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Type

      40 for Acct-Status-Type.

   Length

      6

   Value

      The Value field is four octets.

       1      Start
       2      Stop
       3      Interim-Update
       7      Accounting-On
       8      Accounting-Off
       9-14   Reserved for Tunnel Accounting
      15      Reserved for Failed

5.5.  Acct-Session-Id

   Description

      This attribute is a unique Accounting ID to make it easy to match
      start and stop records in a log file.  The start and stop records
      for a given session MUST have the same Acct-Session-Id.  An
      Accounting-Request packet MUST have an Acct-Session-Id.  An
      Access-Request packet MAY have an Acct-Session-Id; if it does,
      then the NAS MUST use the same Acct-Session-Id in the Accounting-
      Request packets for that session.

      The Acct-Session-Id SHOULD contain UTF-8 encoded 10646 [7]
      characters.

   A summary of the Acct-Session-Id attribute format is shown below.
   The fields are transmitted from left to right.

    0                   1                   2
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |    Length     |  Text ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Type

      44 for Acct-Session-Id.

   Length

      >= 3

   String

      The String field SHOULD be a string of UTF-8 encoded 10646 [7]
      characters.
*/

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdint.h>     // Network byte types
#include "TypeRadius.h"

// --------------------------------------------------------------------------- /
// CRadiusPacket 1.0
// --------------------------------------------------------------------------- /
// Constants: 
//  Packet element length: 
const int   RAD_CODE_LEN                                = 1;
const int   RAD_IDENTIFIER_LEN                          = 1;
const int   RAD_PACKET_LENGTH_LEN                       = 2;
const int   RAD_AUTHENTICATOR_LEN                       = 16;
//  Header length: code+ID+length+vector
const int   RAD_PACKET_HEADER_LEN                       = RAD_CODE_LEN + RAD_IDENTIFIER_LEN + 
                                                            RAD_PACKET_LENGTH_LEN + RAD_AUTHENTICATOR_LEN;

//  Atributes area length: No use to set it too high, full size should <= MTU which is around 1600
const int   RAD_MAX_ATTRIBUTE_LEN                       = 253;  // One attribute, text, string: 1 ~ 253.
const int   RAD_MAX_ATTRIBUTE_AREA_LEN                  = 1300; // No more than popular MTU.

// --------------------------------------------------------------------------- /
// Type define: 
//  RADIUS attribute: 
typedef struct stRadiusAttribute
{
    uint8_t     nType;
    uint8_t     nLength;
    uint8_t     arrValue[RAD_MAX_ATTRIBUTE_LEN];
} tRadiusAttribute;

//  RADIUS Packet: 
typedef struct stRadiusPacket
{
    uint8_t     nCode;                                      // RADIUS Codes
    uint8_t     nIdentifier;                                // Identifier
    uint8_t     nLength[RAD_PACKET_LENGTH_LEN];             // Packet length
    uint8_t     arrAuthenticator[RAD_AUTHENTICATOR_LEN];    // Authenticator
    uint8_t     arrAttribute[RAD_MAX_ATTRIBUTE_AREA_LEN];   // All attributes
} tRadiusPacket;

// --------------------------------------------------------------------------- /
//  Packet code: 
const uint8_t   RAD_ACCESS_REQUEST                      = 1;
const uint8_t   RAD_ACCESS_ACCEPT                       = 2;
const uint8_t   RAD_ACCESS_REJECT                       = 3;
const uint8_t   RAD_ACCOUNTING_REQUEST                  = 4;
const uint8_t   RAD_ACCOUNTING_RESPONSE                 = 5;
const uint8_t   RAD_ACCESS_CHALLENGE                    = 11;
const uint8_t   RAD_STATUS_SERVER                       = 12;
const uint8_t   RAD_STATUS_CLIENT                       = 13;
const uint8_t   RAD_RESERVED                            = 255;

// Attribute: Kind
const char      RAD_ATTR_KIND_STRING                    = 'S';
const char      RAD_ATTR_KIND_IP                        = 'I';
const char      RAD_ATTR_KIND_ULONG                     = 'L';

// Attribute: Type
//  Authentication: 
const uint8_t   RAD_ATTR_TYPE_USERNAME                  = 1;    // User-Name
const uint8_t   RAD_ATTR_TYPE_USERPASSWORD              = 2;    // User-Password
const uint8_t   RAD_ATTR_TYPE_NASIPADDRESS              = 4;    // NAS-IP-Address
const uint8_t   RAD_ATTR_TYPE_NASPORT                   = 5;    // NAS-Port
const uint8_t   RAD_ATTR_TYPE_SERVICETYPE               = 6;    // Service-Type
const uint8_t   RAD_ATTR_TYPE_SESSIONTIMEOUT            = 27;   // Session-Timeout
//  Extended authentication attribute: 
// 
const uint8_t   RAD_ATTR_TYPE_SIPMETHOD                 = 101;  // Sip-Method
const uint8_t   RAD_ATTR_TYPE_SIPRESPONSECODE           = 102;  // Sip-Response-Code
const uint8_t   RAD_ATTR_TYPE_SIPCSEQ                   = 103;  // Sip-Cseq
const uint8_t   RAD_ATTR_TYPE_SIPTOTAG                  = 104;  // Sip-From-Tag
const uint8_t   RAD_ATTR_TYPE_SIPFROMTAG                = 105;  // Sip-To-Tag
// 
const uint8_t   RAD_ATTR_TYPE_SIPCALLID                 = 111;  // Sip-Call-Id
// 
const uint8_t   RAD_ATTR_TYPE_SIPUSERAGENT              = 115;  // Sip-User-Agent
// 
const uint8_t   RAD_ATTR_TYPE_SIPINBOUNDIPADDRESS       = 120;  // Sip-Inbound-Ip-Address
const uint8_t   RAD_ATTR_TYPE_SIPINBOUNDPORT            = 121;  // Sip-Inbound-Port
const uint8_t   RAD_ATTR_TYPE_SIPOUTBOUNDIPADDRESS      = 122;  // Sip-Outbound-Ip-Address
const uint8_t   RAD_ATTR_TYPE_SIPOUTBOUNDPORT           = 123;  // Sip-Outbound-Port
// 
const uint8_t   RAD_ATTR_TYPE_SIPMEDIAIPADDRESS         = 124;  // Sip-Media-Ip-Address media server IP address. 
// 
const uint8_t   RAD_ATTR_TYPE_PROVIDERID                = 130;  // Provider-Id
// 
const uint8_t   RAD_ATTR_TYPE_ACCESSTOKEN               = 140;  // Access-Token. String: Authentication token returned by access request. 
const uint8_t   RAD_ATTR_TYPE_CHANNELID                 = 141;  // Channel-Id. String: Channel ID returned by access request. 

//  Accounting: 
const uint8_t   RAD_ATTR_TYPE_CALLEDSTATIONID           = 30;   // Called-Station-Id
const uint8_t   RAD_ATTR_TYPE_CALLINGSTATIONID          = 31;   // Calling-Station-Id
const uint8_t   RAD_ATTR_TYPE_ACCTSESSIONID             = 44;
//  Extended accounting attribute: 
// 
const uint8_t   RAD_ATTR_TYPE_TERMINATIONCODE           = 150;   // Long: Why the session is terminated. 
const uint8_t   RAD_ATTR_TYPE_TERMINATIONREASON         = 151;   // String: Why the session is terminated. 

// Attribute: Value
const uint32_t  RAD_ATTR_VALUE_ACCTSTATUSTYPE_START     = 1;
const uint32_t  RAD_ATTR_VALUE_ACCTSTATUSTYPE_STOP      = 2;

// Authenticator: All zero initializer.
const uint8_t   RAD_ACCT_REQUEST_AUTHENTICATOR[]        = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

// 
class CRadiusPacket
{
private: 
// 
    // 
    std::string             m_sSecret;                      // Secret shared between client and server. 
    tRadiusPacket           m_stPacket;                     // Packet. 
// 
    void                    SetLength(int nLength);
// 
    // Return the pointer of the attribute: 
    bool                    FindAttribute(uint8_t nType, tRadiusAttribute& stAttribute) const;
public: 
    CRadiusPacket();
    CRadiusPacket(std::string sSecret);
    ~CRadiusPacket();
// 
    // Clear header and attribute: 
    void                    ClearPacket();
    // Clear attribute: 
    void                    ClearAttribute();
    // 
    const CRadiusPacket&    operator = (const CRadiusPacket& objRadiusPacket);

// Secret: 
    void                    SetSecret(std::string sSecret);
    bool                    IsSecretSet() const;

// Passowrd encription/decryption: 
    int                     EncryptPassword(const char *szPassword, uint8_t *pPassEncrypted, int nSize) const;
    void                    DecryptPassword(const uint8_t *pPassEncrypted, int nPassLen, char* szPassword, int nSize) const;

// Set a packet got from network: 
    void                    SetPacket(const uint8_t *pPacket, int nLength);

// Generate a packet to send: 
    // 
    void                    SetCode(uint8_t nCode);
    // If retransmit, keep identifier and authenticator the same.
    void                    SetIdentifier(uint8_t nIdentifier);
    void                    SetAuthenticator(const uint8_t *pAuthenticator, int nLength);
/*
    // Must call SetIdentifier() first to generate a random authenticator.
    void                    SetRandomAuthenticator();
*/
    // Note: If type = RAD_ATTR_TYPE_USERPASSWORD, it will encrypt password inside.
    int                     AddAttribute(uint8_t nType, int nLength, const uint8_t* pValue, char cKind = RAD_ATTR_KIND_STRING);

// Get packet information and attributes: 
    const uint8_t*          GetPacket() const;
    int                     GetLength() const;
    // Used to receive: 
    uint8_t*                GetBuffer();
    int                     GetSize() const;
    // 
    uint8_t                 GetCode() const;
    uint8_t                 GetIdentifier() const;
// 
    // Generate 16 bytes random vector(By current identifier and time). Return the length of the vector.
/*
    int                     GenerateRandomAuthenticator(uint8_t nIdentifier, uint8_t* pAuthenticator, int nLength) const;
*/
    void                    GenerateRandomAuthenticator();
    void                    GetAuthenticator(uint8_t *pRequestAuthenticator, int nSize) const;
    const uint8_t*          GetAuthenticator() const;
    int                     GetAttributeLength() const;
// 
    // If return false, can not find: 
    bool                    GetAttribute(uint8_t nType, tRadiusAttribute& stAttribute) const;
    // Note: If type = RAD_ATTR_TYPE_USERPASSWORD, it will decrypt password inside.
    bool                    GetAttributeString(uint8_t nType, std::string& sValue) const;
    bool                    GetAttributeIp(uint8_t nType, std::string& sValue) const;
    bool                    GetAttributeLong(uint8_t nType, unsigned long& lValue) const;
// Generate and verify check sum: 
    // Calculate authenticator by the current packet: MD5(Code+ID+Length+Authenticator+Attributes+Secret);
    int                     CalculateAuthenticator(uint8_t *pRequestAuthenticator, int nLength) const;
    // Check if the packet is valid by original authenticator: 
    bool                    IsPacketValid(const uint8_t *pOriginalAuthenticator, int nLength);
};

#endif
