/* =============================================================================
// Name:         CStorageBilling.cpp
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

/* =========================================================================== /
* Place includes, constant defines here. 
/============================================================================ */
#include "CException.h"
#include "CCurl.h"

// --------------------------------------------------------------------------- /
// CCurl 1.0
// --------------------------------------------------------------------------- /
CCurl::CCurl()
{
    try
    {
        m_pCurl     = NULL;
        m_psOutput  = NULL;
        // 
        if(NULL == (m_pCurl = curl_easy_init())) 
        {
            throw CException(-1, "%s(%ld): CCurl::CCurl(): curl_easy_init() failed.", __FILE__, __LINE__);
        }
        // Error: 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_pErrorBuffer)) 
        {
            throw CException(-1, "%s(%ld): CCurl::HttpPost(): curl_easy_setopt(CURLOPT_ERRORBUFFER) failed.", __FILE__, __LINE__);
        }
        // Callback: 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &CCurl::Writer)) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_setopt() failed.", __FILE__, __LINE__);
        }
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this)) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_setopt() failed.", __FILE__, __LINE__);
        }
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CCurl::CCurl(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CCurl::~CCurl()
{
    if(NULL != m_pCurl) 
    {
        curl_easy_cleanup(m_pCurl);
        m_pCurl = NULL;
        m_psOutput = NULL;
    }
}


// --------------------------------------------------------------------------- /
// A parameter set to 1 tells the library to include the header in the body output.
void CCurl::SetOptionHeader(long lHeader)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_HEADER, lHeader)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionHeader(): curl_easy_setopt(CURLOPT_HEADER) failed.", __FILE__, __LINE__);
        }
    }
}

// Pass a long. If it is 1, libcurl will not use any functions that install signal handlers or any functions that cause signals to be sent to the process. This option is mainly here to allow multi-threaded unix applications to still set/use all timeout options etc, without risking getting signals.
// If this option is set and libcurl has been built with the standard name resolver, timeouts will not occur while the name resolve takes place. Consider building libcurl with c-ares support to enable asynchronous DNS lookups, which enables nice timeouts for name resolves without signals.
void CCurl::SetOptionNoSignal(long lNoSignal)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, lNoSignal)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionNoSignal(): curl_easy_setopt(lNoSignal) failed.", __FILE__, __LINE__);
        }
    }
}

// --------------------------------------------------------------------------- /
// Proxy related: 
//  Pass a long with this option to set type of the proxy. Available options for this are CURLPROXY_HTTP, CURLPROXY_HTTP_1_0 (added in 7.19.4), CURLPROXY_SOCKS4 (added in 7.15.2), CURLPROXY_SOCKS5, CURLPROXY_SOCKS4A (added in 7.18.0) and CURLPROXY_SOCKS5_HOSTNAME (added in 7.18.0). The HTTP type is default. (Added in 7.10).
void CCurl::SetOptionProxyType(long lProxyType)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_PROXYTYPE, lProxyType)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionProxy(): curl_easy_setopt(CURLOPT_PROXYTYPE) failed.", __FILE__, __LINE__);
        }
    }
}

//  You can specify port in this string like: proxy.com:80
void CCurl::SetOptionProxy(const std::string sHost)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_PROXY, sHost.c_str())) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionProxy(): curl_easy_setopt(CURLOPT_PROXY) failed.", __FILE__, __LINE__);
        }
    }
}

void CCurl::SetOptionProxyPort(long lPort)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_PROXYPORT, lPort)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionProxy(): curl_easy_setopt(CURLOPT_PROXYPORT) failed.", __FILE__, __LINE__);
        }
    }
}

// --------------------------------------------------------------------------- /
void CCurl::SetOptionUserAgent(const std::string sUserAgent) 
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, sUserAgent.c_str())) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionUserAgent(): curl_easy_setopt(CURLOPT_USERAGENT) failed.", __FILE__, __LINE__);
        }
    }
}

void CCurl::SetOptionFollowLocation(long lFollowLocation)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, lFollowLocation)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionFollowLocation(): curl_easy_setopt(CURLOPT_FOLLOWLOCATION) failed.", __FILE__, __LINE__);
        }
    }
}

void CCurl::SetOptionSSLVerifyPeer(long lSSLVerifyPeer)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, lSSLVerifyPeer)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionSSLVerifyPeer(): curl_easy_setopt(CURLOPT_SSL_VERIFYPEER) failed.", __FILE__, __LINE__);
        }
    }
}

void CCurl::SetOptionSSLVerifyHost(long lSSLVerifyHost)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, lSSLVerifyHost)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionSSLVerifyHost(): curl_easy_setopt(CURLOPT_SSL_VERIFYHOST) failed.", __FILE__, __LINE__);
        }
    }
}

// CONNECTION OPTIONS
void CCurl::SetOptionTimeOut(long lTimeOutSeconds)
{
    if(NULL != m_pCurl)
    {
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, lTimeOutSeconds)) 
        {
            throw CException(-1, "%s(%ld): CCurl::SetOptionTimeOut(): curl_easy_setopt(CURLOPT_TIMEOUT) failed.", __FILE__, __LINE__);
        }
    }
}

// --------------------------------------------------------------------------- /
size_t CCurl::Writer(char *pBuffer, size_t size, size_t nmemb, void* pObject)
{
    size_t lSize = size * nmemb;
    if(lSize > 0 && NULL != pBuffer && NULL != pObject) 
    {
        // Call non-static member function: 
        if(NULL != static_cast<CCurl*>(pObject)->m_psOutput)
        {
            static_cast<CCurl*>(pObject)->m_psOutput->append(pBuffer, lSize);
        }else 
        {
            throw CException(-1, "%s(%ld): CCurl::Writer(): pObject)->m_psOutput is NULL.", __FILE__, __LINE__);
        }
    }
    return(lSize);
}

void CCurl::HttpGet(const char* szUrlBase, const std::list< std::pair<std::string, std::string> >& listInput, std::string& sOutput) 
{
    try
    {
        // 
        if(NULL == szUrlBase) 
        {
            throw CException(-1, "%s(%ld): szUrlBase is NULL.", __FILE__, __LINE__);
        }
        // Prepare get parameters: 

        // Set member pointer for call back: 
        m_psOutput = &sOutput;

        // 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_pErrorBuffer)) 
        {
            throw CException(-1, "%s(%ld): CCurl::HttpPost(): curl_easy_setopt(CURLOPT_ERRORBUFFER) failed.", __FILE__, __LINE__);
        }
        
        // CURLOPT_HTTPGET: Pass a long. If the long is 1, this forces the HTTP request to get back to GET. Usable if a POST, HEAD, PUT, or a custom request has been used previously using the same curl handle.
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 1L)) 
        {
            throw CException(-1, "%s(%ld): CCurl::HttpGet(): curl_easy_setopt(CURLOPT_HTTPGET) failed.", __FILE__, __LINE__);
        }
        // All important: 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_URL, szUrlBase)) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_setopt(CURLOPT_URL) failed.", __FILE__, __LINE__);
        }
        
        // 
        CURLcode result;
        if(CURLE_OK != (result = curl_easy_perform(m_pCurl))) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpGet(): curl_easy_perform() failed: [%d][%s].", __FILE__, __LINE__, 
                result, m_pErrorBuffer);
        }
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CCurl::HttpGet(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CCurl::HttpPost(const char* szUrlBase, const std::list< std::pair<std::string, std::string> >& listInput, std::string& sOutput)
{
    try
    {
        // 
        if(NULL == szUrlBase) 
        {
            throw CException(-1, "%s(%ld): szUrlBase is NULL.", __FILE__, __LINE__);
        }
        
        // Prepare post data: 
        int nLen;
        std::string sPostData;
        std::list< std::pair<std::string, std::string> >::const_iterator it;
        // 
        sPostData.clear();
        for(it = listInput.begin(); it != listInput.end(); ++it) 
        {
            sPostData += it->first + "=" + it->second + "&";
        }
        if(!sPostData.empty())
        {
            sPostData = sPostData.substr(0, sPostData.size()-1);
        }

        // Set member pointer for call back: 
        m_psOutput = &sOutput;

        //      All important: 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_URL, szUrlBase)) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_setopt(CURLOPT_URL) failed.", __FILE__, __LINE__);
        }
        // 
        if(0 != curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, sPostData.c_str())) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_setopt(CURLOPT_POSTFIELDS) failed.", __FILE__, __LINE__);
        }
        
        // 
        CURLcode result;
        if(CURLE_OK != (result = curl_easy_perform(m_pCurl))) 
        {
            throw CException(-1, "%s(%ld): CApiHttp::HttpPost(): curl_easy_perform() failed: [%d][%s].", __FILE__, __LINE__, 
                result, m_pErrorBuffer);
        }
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CCurl::HttpPost(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
