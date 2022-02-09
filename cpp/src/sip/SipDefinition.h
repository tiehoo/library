/* =========================================================================== /
* @category         Name Value Pair
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CNameValuePair.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _SIPDEFINITION_H
#define _SIPDEFINITION_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */

// Space: Just added \r\n into to make life easier.
const char  SIP_DEF_CHARSET_SPACE[] = "\r\n\t ";  // 

//  Line terminator: 
const char SIP_MESSAGE_LINE_TERMINATOR[]    = "\r\n";   // 

//----------------------------------------------------------------------------//
//  SIP method: 
const char SIP_METHOD_REGISTER[]        = "REGISTER";
const char SIP_METHOD_INVITE[]          = "INVITE";
const char SIP_METHOD_ACK[]             = "ACK";
const char SIP_METHOD_CANCEL[]          = "CANCEL";
const char SIP_METHOD_BYE[]             = "BYE";
const char SIP_METHOD_OPTIONS[]         = "OPTIONS";

//----------------------------------------------------------------------------//
// Header: 
const char  SIP_HEADER_FROM[]                   = "From";
const char  SIP_HEADER_FROM_SHORT[]             = "f";
const char  SIP_HEADER_FROM_STANDARD[]          = "FROM";
// 
const char  SIP_HEADER_TO[]                     = "To";
const char  SIP_HEADER_TO_SHORT[]               = "t";
const char  SIP_HEADER_TO_STANDARD[]            = "TO";
// 
const char  SIP_HEADER_CALL_ID[]                = "Call-ID";
const char  SIP_HEADER_CALL_ID_SHORT[]          = "i";
const char  SIP_HEADER_CALL_ID_STANDARD[]       = "CALL-ID";
// 
const char  SIP_HEADER_VIA[]                    = "Via";
const char  SIP_HEADER_VIA_SHORT[]              = "v";
const char  SIP_HEADER_VIA_STANDARD[]           = "VIA";
// 
const char  SIP_HEADER_CONTENT_TYPE[]           = "Content-Type";
const char  SIP_HEADER_CONTENT_TYPE_SHORT[]     = "c";
const char  SIP_HEADER_CONTENT_TYPE_STANDARD[]  = "CONTENT-TYPE";
// 
const char  SIP_HEADER_CONTENT_LENGTH[]         = "Content-Length";
const char  SIP_HEADER_CONTENT_LENGTH_SHORT[]   = "l";
const char  SIP_HEADER_CONTENT_LENGTH_STANDARD[]= "CONTENT-LENGTH";
// 
const char  SIP_HEADER_CONTACT[]                = "Contact";
const char  SIP_HEADER_CONTACT_SHORT[]          = "m";
const char  SIP_HEADER_CONTACT_STANDARD[]       = "CONTACT";

// 
const char  SIP_HEADER_CSEQ[]                   = "CSeq";
// 
const char  SIP_HEADER_ROUTE[]                  = "Route";
// 
const char  SIP_HEADER_RECORD_ROUTE[]           = "Record-Route";
// 
const char  SIP_HEADER_MAX_FORWARDS[]           = "Max-Forwards";
// 
const char  SIP_HEADER_EXPIRES[]                = "Expires";
// 
const char  SIP_HEADER_AUTHORIZATION[]          = "Authorization";
// 
const char  SIP_HEADER_WWW_AUTHENTICATE[]       = "WWW-Authenticate";
// 

#endif
