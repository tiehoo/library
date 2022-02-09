/* =============================================================================
// Name:         CCurl.h
// Description:  
// Function:     
// Vertion£º     (0.1)
// Auther:       Stephen Hu(Tie Hu)
// Corporation:  Softroute
// Copyright (c) 
// Compilation:  
// Modified      (MM/DD/YY)  (Version) (Previours version) (Purpose)
   Stephen Hu    24/07/2005  0.1                           First Generate
============================================================================= */
#ifndef _CCurl_H
#define _CCurl_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <list>
#include <curl/curl.h>

/*
http://curl.haxx.se/libcurl/c/example.html
*/
// --------------------------------------------------------------------------- /
// CCurl 1.0
// --------------------------------------------------------------------------- /
class CCurl
{
private: 
    CURL*           m_pCurl;
    // Error Buffer: 
    char            m_pErrorBuffer[CURL_ERROR_SIZE];
    // Call back: 
    std::string*    m_psOutput; // Private callback variable. 
    // Static call back function: 
    static 
    size_t          Writer(char *pBuffer, size_t size, size_t nmemb, void* pObject);
public: 
    // 
    CCurl();
    ~CCurl();
// BEHAVIOR OPTIONS: 
    // A parameter set to 1 tells the library to include the header in the body output.
    void            SetOptionHeader(long lHeader);
    // Pass a long. If it is 1, libcurl will not use any functions that install signal handlers or any functions that cause signals to be sent to the process. This option is mainly here to allow multi-threaded unix applications to still set/use all timeout options etc, without risking getting signals.
    // If this option is set and libcurl has been built with the standard name resolver, timeouts will not occur while the name resolve takes place. Consider building libcurl with c-ares support to enable asynchronous DNS lookups, which enables nice timeouts for name resolves without signals.
    void            SetOptionNoSignal(long lNoSignal);
// NETWORK OPTIONS: 
    // Proxy related: 
    //  Pass a long with this option to set type of the proxy. Available options for this are CURLPROXY_HTTP, CURLPROXY_HTTP_1_0 (added in 7.19.4), CURLPROXY_SOCKS4 (added in 7.15.2), CURLPROXY_SOCKS5, CURLPROXY_SOCKS4A (added in 7.18.0) and CURLPROXY_SOCKS5_HOSTNAME (added in 7.18.0). The HTTP type is default. (Added in 7.10).
    void            SetOptionProxyType(long lProxyType);
    //  You can specify port in this string like: proxy.com:80
    void            SetOptionProxy(const std::string sHost);
    void            SetOptionProxyPort(long lPort);
// HTTP OPTIONS: 
    void            SetOptionUserAgent(const std::string sUserAgent);
    void            SetOptionFollowLocation(long lFollowLocation);
    // Some times when you use SSL you may need to skip verification: 
    void            SetOptionSSLVerifyPeer(long lSSLVerifyPeer);
    void            SetOptionSSLVerifyHost(long lSSLVerifyHost);
// CONNECTION OPTIONS
    void            SetOptionTimeOut(long lTimeOutSeconds);
    
    // 
    void            HttpPost(const char* szUrlBase, const std::list< std::pair<std::string, std::string> >& listInput, std::string& sOutput);
    void            HttpGet(const char* szUrlBase, const std::list< std::pair<std::string, std::string> >& listInput, std::string& sOutput);
};

#endif
