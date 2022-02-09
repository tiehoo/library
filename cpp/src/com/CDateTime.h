/* =========================================================================== /
* @category         DateTime
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CDATETIME_H
#define _CDATETIME_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <sys/time.h>
#include <string>
// 
#include "CTimeInterval.h"

// =========================================================================== /
// CDateTime 1.0 
// =========================================================================== /
// Constants: 
/*
const int  DATE_TIME_BASE_YEAR      = 1900;
const char DATE_TIME_ESC_CHAR       = '\\';
const int  MAX_DATE_TIME_STR_LEN    = 128;
const char DEF_DATE_TIME_FORMAT[]   = "YYYY-MM-DD HH24:MI:SS";
*/

// Used for date formatting.
//  Month table: 
static char szaMonTab[12][16] = 
{
  	"January", "February", "March", "April", "May", "June",
  	"July", "August", "September", "October", "November", "December"
};
//  Month abbreviation table: 
static char szaMonTabAbr[12][4]=
{
  	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
  	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
//  Week day abbreviation table: 
static char szaDayTabAbr[12][4]=
{
  	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
//  Week day table: 
static char szaDayTab[12][10]=
{
  	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

// 
class CDateTime
{
private: 
    // Constant: 
    const int           DATE_TIME_BASE_YEAR;    // = 1900;
    const char          DATE_TIME_ESC_CHAR;     // = '\\';
    const int           MAX_DATE_TIME_STR_LEN;  // = 128;
    // Time structure: m_stTime.tv_sec, m_stTime.tv_usec.
    struct timeval      m_stTimeVal;            // Used for date time formatting.
    char                m_cEscChar;             // Escape char: '\'.
public: 
    // 
    CDateTime();        // Initialize to NOW().
    // Don't use time_t or suseconds_t here, long is signed and good for operator + or -.
    CDateTime(long lSecond, long lMicroSecond);
    CDateTime(const CDateTime& objDateTime);
    ~CDateTime();
    // 
    void                Clear();
// 
    void                SetTimeStamp(long lSecond, long lMicroSecond);
    // Time stamp is a string like: "xxxxxxxxxx.xxxxxx".
    std::string         GetTimeStamp() const;
// 
    // second: 
    void                SetSecond(long lSecond);
    long                GetSecond() const;
    // microsecond: 
    void                SetMicroSecond(long lMicroSecond);
    long                GetMicroSecond() const;
// 
    void                SetToNow();
// 
    const CDateTime&    operator =  (const CDateTime& objDateTime);
    // 
    CDateTime           operator +  (const CTimeInterval& objTimeInterval) const;
    CDateTime           operator -  (const CTimeInterval& objTimeInterval) const;
    CTimeInterval       operator -  (const CDateTime& objDateTime) const;
    CDateTime           operator +  (long lMicrosecond) const;
    CDateTime           operator -  (long lMicrosecond) const;
    // use: using namespace std::rel_ops; to get other 4 operators. 
    bool                operator == (const CDateTime& objDateTime) const;
    bool                operator <  (const CDateTime& objDateTime) const;
// 
    // Used for select() etc. I will not return negative values.
    void                GetTimeVal(struct timeval& stTimeVal) const;
    void                GetTimeSpec(struct timespec& stTimeSpec) const;
// 
    void                SetEscChar(char cEscChar);
    void                SetDateTime(const char* szDateTime, const char* szFormat);
    std::string         ToString(const char* szFormat) const;
};

#endif
