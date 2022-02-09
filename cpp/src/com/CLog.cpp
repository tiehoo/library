/* =========================================================================== /
* @category     System logging
* @package      Logger
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
#include "CDateTime.h"
#include "CLog.h"

// =========================================================================== /
// CLog 1.0
//============================================================================*/
void CLog::Write(int nPriority, const char* szFormat, va_list stParameter) const
{
    if(NULL == szFormat)
    {
        throw CException(-1, "%s(%ld): CLog::Write(int, const char*, va_list): NULL pointer.", __FILE__, __LINE__);
    }
    // 
    if(m_bTimeStampOn)
    {
        CDateTime objDateTime;
        syslog(nPriority, "(%ld,%ld)", objDateTime.GetSecond(), objDateTime.GetMicroSecond());
    }
    // 
    vsyslog(nPriority, szFormat, stParameter);
}

//----------------------------------------------------------------------------//
CLog::CLog(const char *szAppId, int nOption, int nFacility, emLogLevel nLevel, bool bTimeStampOn)
{
    SetLevel(nLevel);
    SetTimeStampOn(bTimeStampOn);
    if(NULL != szAppId) Open(szAppId, nOption, nFacility);
}

CLog::~CLog()
{
    Close();
}

//----------------------------------------------------------------------------//
void CLog::SetLevel(emLogLevel nLevel)
{
    m_nLevel = nLevel;
}

void CLog::SetTimeStampOn(bool bOn)
{
    m_bTimeStampOn = bOn;
}

//----------------------------------------------------------------------------//
void CLog::Open(const char *szAppId, int nOption, int nFacility) const
{
    if(NULL == szAppId)
    {
        throw CException(-1, "%s(%ld): CLog::Open(const char *, int, int): NULL pointer.", __FILE__, __LINE__);
    }
    openlog(szAppId, nOption, nFacility);
}

// 
void CLog::Write(int nPriority, const char* szFormat, ...) const
{
    if(NULL == szFormat) throw CException(-1, "%s(%ld): CLog::Write(int, const char*, ...): NULL pointer.", __FILE__, __LINE__);
    // 
    va_list stParameter;
    va_start(stParameter, szFormat);
    Write(nPriority, szFormat, stParameter);
    va_end(stParameter);
}

// 
void CLog::Close() const
{
    closelog();
}

//----------------------------------------------------------------------------//
// 
void CLog::WriteError(const char* szFormat, ...) const
{
    if(m_nLevel < LOG_LEVEL_ERROR) return;
    if(NULL == szFormat) throw CException(-1, "%s(%ld): CLog::WriteInfo(const char*): NULL pointer.", __FILE__, __LINE__);
    // 
    va_list stParameter;
    va_start(stParameter, szFormat);
    Write(LOG_ERR, szFormat, stParameter);
    va_end(stParameter);
}

void CLog::WriteWarning(const char* szFormat, ...) const
{
    if(m_nLevel < LOG_LEVEL_WARNING) return;
    if(NULL == szFormat) throw CException(-1, "%s(%ld): CLog::WriteInfo(const char*): NULL pointer.", __FILE__, __LINE__);
    // 
    va_list stParameter;
    va_start(stParameter, szFormat);
    Write(LOG_WARNING, szFormat, stParameter);
    va_end(stParameter);
}

void CLog::WriteInfo(const char* szFormat, ...) const
{
    if(m_nLevel < LOG_LEVEL_INFOR) return;
    if(NULL == szFormat) throw CException(-1, "%s(%ld): CLog::WriteInfo(const char*): NULL pointer.", __FILE__, __LINE__);
    //    
    va_list stParameter;
    va_start(stParameter, szFormat);
    Write(LOG_INFO, szFormat, stParameter);
    va_end(stParameter);
}

void CLog::WriteDebug(const char* szFormat, ...) const
{
    if(m_nLevel < LOG_LEVEL_DEBUG) return;
    if(NULL == szFormat) throw CException(-1, "%s(%ld): CLog::WriteInfo(const char*): NULL pointer.", __FILE__, __LINE__);
    // 
    va_list stParameter;
    va_start(stParameter, szFormat);
    Write(LOG_DEBUG, szFormat, stParameter);
    va_end(stParameter);
}
