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
#ifndef _CLOG_H
#define _CLOG_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <syslog.h>
#include <stdarg.h>

// =========================================================================== /
// CLog 1.0
// =========================================================================== /
/*
nOption: 
     LOG_CONS      If syslog() cannot pass the message to syslogd(8) it will
                   attempt to write the message to the console
                   (``/dev/console'').
     LOG_NDELAY    Open the connection to syslogd(8) immediately.  Normally
                   the open is delayed until the first message is logged.
                   Useful for programs that need to manage the order in which
                   file descriptors are allocated.
     LOG_PLOG_LEVEL_ERROR    Write the message to standard error output as well to the
                   system log.
     LOG_PID       Log the process id with each message: useful for identify-
                   ing instantiations of daemons.
*/
/*
nPriority: 
     LOG_EMERG     A panic condition.  This is normally broadcast to all
                   users.
     LOG_ALERT     A condition that should be corrected immediately, such as a
                   corrupted system database.
     LOG_CRIT      Critical conditions, e.g., hard device errors.
     LOG_ERR       Errors.
     LOG_LOG_LEVEL_WARNING   Warning messages.
     LOG_NOTICE    Conditions that are not error conditions, but should possi-
                   bly be handled specially.
     LOG_INFO      Informational messages.
     LOG_LOG_LEVEL_DEBUG     Messages that contain information normally of use only when
                   debugging a program.
*/
// Defines: 
enum emLogLevel{LOG_LEVEL_NONE = 1, LOG_LEVEL_ERROR = 10, LOG_LEVEL_WARNING = 20, LOG_LEVEL_INFOR = 30, LOG_LEVEL_DEBUG = 40};
// 
class CLog
{
private: 
    emLogLevel      m_nLevel;
    bool            m_bTimeStampOn;
    // 
    void Write(int nPriority, const char* szFormat, va_list stParameter) const;
public: 
    CLog(const char *szAppId = NULL, int nOption = (LOG_CONS | LOG_PERROR | LOG_PID), int nFacility = LOG_USER, emLogLevel nLevel = LOG_LEVEL_DEBUG, bool bTimeStampOn = false);
    ~CLog();
// 
    void SetLevel(emLogLevel nLevel);
    void SetTimeStampOn(bool bOn);
// 
    void Open(const char *szAppId, int nOption = (LOG_CONS | LOG_PERROR | LOG_PID), int nFacility = LOG_USER) const;
    void Write(int nPriority, const char* szFormat, ...) const;
    void Close() const;
// 
    void WriteError(const char *szFormat = "", ...) const;
    void WriteWarning(const char *szFormat = "", ...) const;
    void WriteInfo(const char *szFormat = "", ...) const;
    void WriteDebug(const char *szFormat = "", ...) const;
};

#endif
