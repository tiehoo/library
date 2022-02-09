/* =============================================================================
// Name:         TypeRadius.h
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
#ifndef _TYPERADIUS_H
#define _TYPERADIUS_H
#include <map>

//------------------------------------------------------------------------------
// Authentication and accounting atructures: 
//  RADIUS Access Request record: 
typedef struct stRadiusAccessRecord
{
    // Input: 
    std::string     sUserName;
    std::string     sPassword;
    std::string     sSipCallId;
    std::string     sSipFromTag;
    std::string     sSipToTag;
    std::string     sSipUserAgent;
    std::string     sCallingStationId;
    std::string     sCalledStationId;
    std::string     sSipInboundIp;
    int             nSipInboundPort;
    std::string     sSipOutboundIp;
    int             nSipOutboundPort;
    std::string     sNasIp; 
    int             nNasPort; 
    std::string     sSipMediaIp; 
    // Output: 
    std::string     sAccessToken;
    unsigned long   lSessionTimeout;
    unsigned long   lChannelId;
    // 
    void Clear() 
    {
        sUserName.clear();
        sPassword.clear();
        sSipUserAgent.clear();
        sSipCallId.clear();
        sSipFromTag.clear();
        sSipToTag.clear();
        sCallingStationId.clear();
        sCalledStationId.clear();
        sNasIp.clear();
        nNasPort        = 0;
        sSipMediaIp.clear();
        sSipInboundIp.clear();
        nSipInboundPort    = 0;
        sSipOutboundIp.clear();
        nSipOutboundPort   = 0;
        // 
        sAccessToken.clear();
        lSessionTimeout = 0;
        lChannelId      = 0;
    }
} tRadiusAccessRecord;

//  RADIUS Accounting record: 
typedef struct stRadiusAccountingRecord
{
    std::string     sSipCallId;
    std::string     sSipFromTag;
    std::string     sSipToTag;
    std::string     sAccessToken;
    std::string     sCodec;
    long            lTerminationCode;
    std::string     sTerminationReason;
    // 
    void Clear() 
    {
        sSipCallId.clear();
        sSipFromTag.clear();
        sSipToTag.clear();
        sAccessToken.clear();
        lTerminationCode    = 0;
        sTerminationReason.clear();
    }
} tRadiusAccountingRecord;

//------------------------------------------------------------------------------
// Client authentication information: 
typedef struct stRadiusClient
{
    std::string sIp;            // 
    char        cType;          // 'U': Authentication, 'C': Accounting.
    std::string sSecret;        // 
} tRadiusClient;

// Client list: 
typedef std::map<std::string, tRadiusClient>    tMapRadiusClient;       // <IP, tRadiusClient>

#endif
