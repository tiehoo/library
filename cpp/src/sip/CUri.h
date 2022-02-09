/* =========================================================================== /
* @category         SIP Message
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CURI_H
#define _CURI_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CMessageBase.h"
#include "CParameterList.h"

//Constants: 
//  Message Length: 
const long  DEF_URI_LEN = 512;

// =========================================================================== /
// CSipUri 1.0
// =========================================================================== /
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
    mailto:watson@bell-telephone.com
*/
/* examples:
   sip:j.doe@big.com;maddr=239.255.255.1;ttl=15
   sip:j.doe@big.com
   sip:j.doe:secret@big.com;transport=tcp
   sip:j.doe@big.com?subject=project
   sip:+1-212-555-1212:1234@gateway.com;user=phone
   sip:1212@gateway.com
   sip:alice@10.1.2.3
   sip:alice@example.com
   sip:alice@registrar.com;method=REGISTER

   NOT EQUIVALENT:
   SIP:JUSER@ExAmPlE.CoM;Transport=udp
   sip:juser@ExAmPlE.CoM;Transport=UDP
   
   // 
   sip:bigbox3.site3.atlanta.com;lr
   // 
   sip:74.13.233.143:80
*/
/*
Here we will implement: 
scheme:[<user>][:<password>][@]<host>[:<port>][;<uri-parameters>][?<headers>]
*/

class CUri : public CMessageBase 
{
private: 
    std::pair<int, int>     m_pairScheme;
    std::pair<int, int>     m_pairUser;
    std::pair<int, int>     m_pairPassword;
    std::pair<int, int>     m_pairHost;
    std::pair<int, int>     m_pairPort;
    std::pair<int, int>     m_pairParameterList;
    std::pair<int, int>     m_pairHeaderList;
public: 
    explicit CUri(size_t nSize = DEF_URI_LEN);
    explicit CUri(const CUri& objUri);
    ~CUri();
    // 
    virtual void        Clear();
    virtual bool        Parse();
    virtual void        PrintStructure() const;
// 
    const CUri&         operator =  (const CUri& objUri);
    bool                operator == (const CUri& objUri) const;
// 
    bool                GetScheme(std::string& sScheme) const;
    bool                GetUser(std::string& sUser) const;
    bool                GetPassword(std::string& sPassword) const;
    bool                GetHost(std::string& sHost) const;
    bool                GetPort(std::string& sPort) const;
    bool                GetPort(int& nPort) const;
    bool                GetParameterList(std::string& sParameterList) const;
    bool                GetParameterList(CParameterList& objParameterList) const;
    bool                GetHeaderList(std::string& sHeaderList) const;
    bool                GetHeaderList(CParameterList& objHeaderList) const;
};

#endif
