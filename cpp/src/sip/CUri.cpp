/* =========================================================================== /
* @category     SIP Message
* @package      Analyzer, Buffer
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
#include "SipDefinition.h"
#include "CUri.h"

/* --------------------------------------------------------------------------- /
// CUri 1.0
/ --------------------------------------------------------------------------- */
CUri::CUri(size_t nSize):CMessageBase(nSize)
{
    try
    {
        // 
        Clear();
    }catch(CException &eEx)
    {
        eEx.SetMessage("CUri::CUri(): Failed.");
        throw eEx;
    }
}

CUri::CUri(const CUri& objUri):CMessageBase(objUri.GetSize())
{
    try
    {
        operator = (objUri);
    }catch(CException &eEx)
    {
        eEx.SetMessage("CUri::CUri(): Failed.");
        throw eEx;
    }
}

CUri::~CUri()
{
}

//----------------------------------------------------------------------------//
void CUri::Clear()
{
    try
    {
        CMessageBase::Clear();
        // 
        m_pairScheme.first          = 0;
        m_pairScheme.second         = 0;
        // 
        m_pairUser.first            = 0;
        m_pairUser.second           = 0;
        // 
        m_pairPassword.first        = 0;
        m_pairPassword.second       = 0;
        // 
        m_pairHost.first            = 0;
        m_pairHost.second           = 0;
        // 
        m_pairPort.first            = 0;
        m_pairPort.second           = 0;
        // 
        m_pairParameterList.first   = 0;
        m_pairParameterList.second  = 0;
        // 
        m_pairHeaderList.first      = 0;
        m_pairHeaderList.second     = 0;
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CUri::Clear(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CNameValuePair::PrintStructure() const 
{
    CMessageBase::PrintStructure();
    // 
    printf("Scheme(%d, %d)", m_pairScheme.first, m_pairScheme.second);
    Print(m_pairScheme.first, m_pairScheme.second);
    printf("\n");
    // 
    printf("User(%d, %d)", m_pairUser.first, m_pairUser.second);
    Print(m_pairUser.first, m_pairUser.second);
    printf("\n");
    // 
    printf("Password(%d, %d)", m_pairPassword.first, m_pairPassword.second);
    Print(m_pairPassword.first, m_pairPassword.second);
    printf("\n");
    // 
    printf("Host(%d, %d)", m_pairHost.first, m_pairHost.second);
    Print(m_pairHost.first, m_pairHost.second);
    printf("\n");
    // 
    printf("Port(%d, %d)", m_pairPort.first, m_pairPort.second);
    Print(m_pairPort.first, m_pairPort.second);
    printf("\n");
    // 
    printf("Password(%d, %d)", m_pairPassword.first, m_pairPassword.second);
    Print(m_pairPassword.first, m_pairPassword.second);
    printf("\n");
    // 
    printf("ParameterList(%d, %d)", m_pairParameterList.first, m_pairParameterList.second);
    Print(m_pairParameterList.first, m_pairParameterList.second);
    printf("\n");
    // 
    printf("HeaderList(%d, %d)", m_pairHeaderList.first, m_pairHeaderList.second);
    Print(m_pairHeaderList.first, m_pairHeaderList.second);
    printf("\n");
    // 
    printf("\n");
}

//----------------------------------------------------------------------------//
const CUri& CUri::operator = (const CUri& objUri)
{
    try
    {
        // 
        CMessageBase::operator = (objUri);
        m_pairScheme            = objUri.m_pairScheme;
        m_pairUser              = objUri.m_pairUser;
        m_pairPassword          = objUri.m_pairPassword;
        m_pairHost              = objUri.m_pairHost;
        m_pairPort              = objUri.m_pairPort;
        m_pairParameterList     = objUri.m_pairParameterList;
        m_pairHeaderList        = objUri.m_pairHeaderList;
        // 
        return(*this);
    }catch(CException &eEx) 
    {
        eEx.SetMessage("CUri::operator = (): Failed.");
        throw eEx;
    }
}

/*
    Scheme, user, host, port all matches, all parameters/headers matches. 
*/
bool CUri::operator == (const CUri& objUri) const 
{
    try
    {
        using namespace std::rel_ops;
        // 
        if(!IsValid() || !objUri.IsValid()) return(false);
        // Scheme: 
        std::string sObject, sSubject;
        if(GetScheme(sObject) && objUri.GetScheme(sSubject)) 
        {
            if(strcasecmp(sObject.c_str(), sSubject.c_str()) != 0) return(false);
        }
        if(strcasecmp(m_strScheme.c_str(), objUri.m_strScheme.c_str()) != 0) return(false);
        // User: 
        if(strcasecmp(m_strUser.c_str(), objUri.m_strUser.c_str()) != 0) return(false);
        // Host: 
        if(strcasecmp(m_strHost.c_str(), objUri.m_strHost.c_str()) != 0) return(false);
        // Port: 
        if(m_nPort != objUri.m_nPort) return(false);
        // Parameter list: 
        if(m_objParameterList != objUri.m_objParameterList) return(false);
        // Header list: 
        if(m_objHeaderList != objUri.m_objHeaderList) return(false);
        // 
        return(true);
    }catch(CException &eEx)
    {
        eEx.SetMessage("CUri::operator == (): Failed.");
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
/*
The generic URI syntax consists of a hierarchical sequence of components referred to as the scheme, authority, path, query, and fragment.
URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
authority   = [ userinfo "@" ] host [ ":" port ]

  foo://username:password@example.com:8042/over/there/index.dtb?type=animal;name=narwhal#nose
  \_/   \_______________/ \_________/ \__/            \___/ \_/ \______________________/ \__/
   |           |               |       |                |    |            |                |
   |       userinfo         hostname  port              |    |          query          fragment
   |    \________________________________/\_____________|____|/
scheme                  |                          |    |    |
   |                authority                    path   |    |
   |                                                    |    |
   |            path                       interpretable as filename
   |   ___________|____________                              |
  / \ /                        \                             |
  urn:example:animal:ferret:nose               interpretable as extension
// SIP/SIPS
sip/sips:<user>[:<password>]@<host>[:<port>][;<uri-parameters>][?<headers>]
examples:
sip:alice@atlanta.com?subject=project%20x&priority=urgent
sip:+1-212-555-1212:1234@gateway.com;user=phone
*/
/* 
http://www.faqs.org/rfcs/rfc3261.html
8.1.1.1
19.1
25
    sip:user:password@host:port;uri-parameters?headers
    uri-parameters  = name1=value1;name2=value2
    headers         = name1=value1&name2=value2
#
    sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15
# Another kind of URI: 
    <mailto:watson@bell-telephone.com>
*/
/*
Here we will implement: 
scheme:[<user>][:<password>][@]<host>[:<port>][;<uri-parameters>][?<headers>]
*/
bool CUri::Parse() 
{
    try
    {
        // 
        long lOffset, lSkipLen, lSkipBackLen, lOffsetUserHost, lLenUserHost, lOffsetPasswordPort, lLenPasswordPort;
        char cTokenUsed, cTokenUsed1;
        // 
        CBufferAnalyzer objAnalyzer(*this);
        objAnalyzer.Rewind();
        
        // 
        lLenUserHost        = 0;
        lLenPasswordPort    = 0;
        cTokenUsed          = '\0';
        cTokenUsed1         = '\0';
        // 
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        if(objAnalyzer.IsEnd()) 
        {
            // All space: [     ]
            SetInvalid("CUri: All space.");
            return(false);
        }
        lOffset = objAnalyzer.GetOffset();
        // 1. Get scheme: 
        if(!objAnalyzer.FindChar(":", cTokenUsed, lSkipLen))
        {
            SetInvalid("CUri: No scheme separator[:] found.");
            return(false);
        }
        // Here cTokenUsed may be: 
        /* {WAITTING to be read}
        ':': scheme[:]...
        */
        // {READ scheme}
        m_pairScheme.first  = lOffset;
        lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairScheme.first + lSkipLen - 1);
        m_pairScheme.second = lSkipLen - 1 - lSkipBackLen;
       
        // 2. Analyze user:password@host:port; REGISTER doesn't have user and password.
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        lOffset = objAnalyzer.GetOffset();
        if(!objAnalyzer.FindChar(":@;?", cTokenUsed, lSkipLen))
        {
            // Can not find token: 
            cTokenUsed = '\0';
        }
        // Here cTokenUsed may be: 
        /* {WAITTING to be read}
        ':': sip:user[:]password... or sip:host[:]port...
        '@': sip:user[@]host...
        ';': sip:host[;]parameter...
        '?': sip:host[?]header...
        '\0': sip:host[]
        */
        if(':' == cTokenUsed)
        {
            // sip:user[:]password... or sip:host[:]port...
            lOffsetUserHost = lOffset;
            lSkipBackLen    = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, lOffsetUserHost + lSkipLen - 1);
            lLenUserHost    = lSkipLen - 1 - lSkipBackLen;
            if(lLenUserHost <= 0)
            {
                SetInvalid("CUri: user/host is empty.");
                return(false);
            }
            // 
            objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
            lOffset = objAnalyzer.GetOffset();
            if(!objAnalyzer.FindChar("@;?", cTokenUsed, lSkipLen)) 
            {
                // Should be: scheme:host[:]port...
                cTokenUsed = '\0';
            }
            lOffsetPasswordPort = lOffset;
            lSkipBackLen        = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, lOffsetPasswordPort + lSkipLen - 1);
            lLenPasswordPort    = lSkipLen - 1 - lSkipBackLen;
            if(lLenPasswordPort <= 0) 
            {
                SetInvalid("CUri: password/port is empty.");
                return(false);
            }
        }
        // Here cTokenUsed may be: 
        /* {WAITTING to be read}
        '@': sip:user[@]host... or sip:user:password[@]host...
        ';': sip:host[;]parameter... or sip:host:port[;]parameter...
        '?': sip:host[?]header... or sip:host:port[?]header...
        '\0': sip:host[] or sip:host:port[]
        */
        // 
        if('@' == cTokenUsed)
        {
            // {READ user}
            m_pairUser.first    = lOffsetUserHost;
            m_pairUser.second   = lLenUserHost;
            // {READ password}
            if(lLenPasswordPort > 0) 
            {
                m_pairUser.first    = lOffsetPasswordPort;
                m_pairUser.second   = lLenPasswordPort;
            }
        }else 
        {
            // {READ host}
            m_pairHost.first    = lOffsetUserHost;
            m_pairHost.second   = lLenUserHost;
            // {READ port}
            if(lLenPasswordPort > 0) 
            {
                m_pairPort.first    = lOffsetPasswordPort;
                m_pairPort.second   = lLenPasswordPort;
            }
        }
        if('\0' == cTokenUsed) 
        {
            SetValid();
            return(true);
        }
        // Here cTokenUsed may be: 
        /* {WAITTING to go}
        ';': [;]parameter or [;]parameter?header
        '?': [?]header or [?]header;parameter
        */
        // 
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        if(objAnalyzer.IsEnd())
        {
            SetInvalid("CUri: parameter/header is empty.");
            return(false);
        }
        lOffset = objAnalyzer.GetOffset();
        if(!objAnalyzer.FindChar(";?", cTokenUsed1, lSkipLen)) 
        {
            cTokenUsed1 = '\0';
        }
        /* {WAITTING to be read}
        ';': [;]parameter
        '?': [?]header
        '\0': parameter[] or header[]
        */
        // 
        if(';' == cTokenUsed) 
        {
            m_pairParameterList.first   = lOffset;
            if('\0' != cTokenUsed1) 
            {
                lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairParameterList.first + lSkipLen - 1);
                m_pairParameterList.second  = lSkipLen - 1 - lSkipBackLen;
            }else 
            {
                lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairParameterList.first + lSkipLen);
                m_pairParameterList.second  = lSkipLen - lSkipBackLen;
            }
        }else if('?' == cTokenUsed) 
        {
            m_pairHeaderList.first      = lOffset;
            if('\0' != cTokenUsed1) 
            {
                lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairHeaderList.first + lSkipLen - 1);
                m_pairHeaderList.second     = lSkipLen - 1 - lSkipBackLen;
            }else 
            {
                lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, m_pairHeaderList.first + lSkipLen - 1);
                m_pairHeaderList.second     = lSkipLen - lSkipBackLen;
            }
        }else 
        {
            throw CException(-1, "%s(%ld): Internal error, unexpected char: %c.", __FILE__, __LINE__, cTokenUsed);
        }
        // 
        if('\0' == cTokenUsed1)
        {
            SetValid();
            return(true);
        }
        
        // Get possible following parameter and header: 
        objAnalyzer.Skip(SIP_DEF_CHARSET_SPACE);
        if(objAnalyzer.IsEnd())
        {
            SetInvalid("CUri: parameter/header(2) is empty.");
            return(false);
        }
        // 
        if(';' == cTokenUsed1) 
        {
            m_pairParameterList.first   = objAnalyzer.GetOffset();
            lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, GetLength() - 1);
            m_pairParameterList.second  = GetLength() - m_pairParameterList.first - lSkipBackLen;
        }else if('?' == cTokenUsed1) 
        {
            m_pairHeaderList.first      = objAnalyzer.GetOffset();
            lSkipBackLen                = objAnalyzer.SkipBackFrom(SIP_DEF_CHARSET_SPACE, GetLength() - 1);
            m_pairHeaderList.second     = GetLength() - m_pairHeaderList.first - lSkipBackLen;
        }else 
        {
            throw CException(-1, "%s(%ld): Internal error, unexpected char: %c.", __FILE__, __LINE__, cTokenUsed);
        }
        // 
        SetValid();
        return(true);
    }catch(CException& eEx)
    {
        eEx.SetMessage("CUri::Parsse(): Failed.");
        throw eEx;
    }
}

size_t CUri::GetString(char* szString, size_t nSize) const
{
    try
    {
        // 
        if(NULL == szString) throw CException(-1, "CUri::GetString(): NULL pointer.");
        // 
        int nLen = 0;
        if(!IsDirty())
        {
            // Return the buffer: 
            nLen = GetLength();
            if(nLen >= nSize)
            {
                throw CException(-1, "CUri::GetString(): Buffer too small, need %d bytes, but only has %d bytes.",
                    nLen, nSize);
            }
            strncpy(szString, GetBufferPointer(), nLen);
        }else 
        {
            // Scheme: 
            if(nLen + m_strScheme.length() >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                nLen + m_strScheme.length(), nSize);
            strcpy(szString + nLen, m_strScheme.c_str());
            nLen += m_strScheme.length();
            // : 
            if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                nLen + 1, nSize);
            szString[nLen] = ':';
            nLen += 1;
            // User: 
            if(m_strUser.length() > 0)
            {
                // User: 
                if(nLen + m_strUser.length() >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + m_strUser.length(), nSize);
                strcpy(szString + nLen, m_strUser.c_str());
                nLen += m_strUser.length();
            }
            // Password: Optinal
            if(m_strPassword.length() > 0) 
            {
                //if(m_strUser.empty()) throw CException(-1, "CUri::GetString(): No user but has password.");
                // :
                if(!m_strUser.empty())
                {
                    if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                        nLen + 1, nSize);
                    szString[nLen] = ':';
                    nLen += 1;
                }
                // Password: 
                if(nLen + m_strPassword.length() >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + m_strPassword.length(), nSize);
                strcpy(szString + nLen, m_strPassword.c_str());
                nLen += m_strPassword.length();
            }
            // @
            if(!m_strPassword.empty() || !m_strUser.empty())
            {
                if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + 1, nSize);
                szString[nLen] = '@';
                nLen += 1;
            }
            if(m_strHost.empty()) throw CException(-1, "CUri::GetString(): No host.");
            // Host: 
            if(nLen + m_strHost.length() >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                nLen + m_strHost.length(), nSize);
            strcpy(szString + nLen, m_strHost.c_str());
            nLen += m_strHost.length();
            // Port: Optional
            if(m_strPort.length() > 0 && !((IsSchemeSip() || IsSchemeSips()) && m_nPort == DEF_SIP_PORT))
            {
                // : 
                if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + 1, nSize);
                szString[nLen] = ':';
                nLen += 1;
                // Port: 
                if(nLen + m_strPort.length() >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + m_strPort.length(), nSize);
                strcpy(szString + nLen, m_strPort.c_str());
                nLen += m_strPort.length();
            }
            // Parameter list: 
            //if(m_objParameterList.GetLength() > 0 || m_objParameterList.IsDirty())
            if(m_objParameterList.GetCount() > 0)
            {
                // ';'
                if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + 1, nSize);
                szString[nLen] = ';';
                nLen += 1;
                // Don't have to check length before, it will check inside GetString()
                nLen += m_objParameterList.GetString(szString + nLen, nSize - nLen);
            }
            // Header list: 
            //if(m_objHeaderList.GetLength() > 0 || m_objHeaderList.IsDirty())
            if(m_objHeaderList.GetCount() > 0)
            {
                // '?'
                if(nLen + 1 >= nSize) throw CException(-1, "CUri::GetString(): Buffer too small, trying to copy %d bytes, but size is only %d bytes.",
                    nLen + 1, nSize);
                szString[nLen] = '?';
                nLen += 1;
                // 
                nLen += m_objHeaderList.GetString(szString + nLen, nSize - nLen);
            }
        }
        // 
        return(nLen);
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CUri::GetString(): Failed.");
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
const std::string& CUri::GetScheme() const
{
    return(m_strScheme);
}

void CUri::SetScheme(std::string strScheme)
{
    m_strScheme = strScheme;
    SetDirty();
}

// 
const std::string& CUri::GetUser() const
{
    return(m_strUser);
}

void CUri::SetUser(std::string strUser)
{
    m_strUser = strUser;
    SetDirty();
}

// 
const std::string& CUri::GetPassword() const
{
    return(m_strPassword);
}

void CUri::SetPassword(std::string strPassword)
{
    m_strPassword = strPassword;
    SetDirty();
}

// 
const std::string& CUri::GetHost() const
{
    return(m_strHost);
}

void CUri::SetHost(std::string strHost)
{
    m_strHost = strHost;
    SetDirty();
}

// 
int CUri::GetPort() const
{
    return(m_nPort);
}

void CUri::SetPort(int nPort)
{
    if(nPort <= 0) throw CException(-1, "CUri::SetPort(int): Invalid port: %d.", nPort);
    m_nPort = nPort;
    m_strPort = m_objAnalyzer.ToString(m_nPort);
    SetDirty();
}

void CUri::SetPort(std::string strPort)
{
    try
    {
        SetPort(m_objAnalyzer.ToLong(m_strPort));
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CUri::SetPort(string&): Failed.");
        throw eEx;
    }
}

std::string CUri::GetUserHostPort() const
{
    std::string sUserHostPort = m_strUser + "@" + m_strHost;
    if(IsPortSet()) sUserHostPort += ":" + m_strPort;
    return(sUserHostPort);
}

// --------------------------------------------------------------------------- /
bool CUri::IsPortSet() const
{
    if(m_nPort < 0) return(false);
    else return(true);
}

// 
bool CUri::IsSchemeSip() const
{
    try
    {
        if(strcasecmp(GetScheme().c_str(), URI_SCHEME_SIP) == 0) return(true);
        else return(false);
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CUri::IsSchemeSip(): Failed.");
        throw eEx;
    }
}

// 
bool CUri::IsSchemeSips() const
{
    try
    {
        if(strcasecmp(GetScheme().c_str(), URI_SCHEME_SIPS) == 0) return(true);
        else return(false);
    }catch(CException& eEx) 
    {
        eEx.SetMessage("CUri::IsSchemeSips(): Failed.");
        throw eEx;
    }
}

// 
bool CUri::IsDirty() const
{
    return(CMessageBase::IsDirty() || m_objParameterList.IsDirty() || m_objHeaderList.IsDirty());
}
