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

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdio.h>
#include <string.h>
#include "CException.h"
#include "CValue.h"
#include "CDateTime.h"
#include <errno.h>
extern int errno;

//---------------------------------------------------------------------------------------------//
//CDateTime: 
//---------------------------------------------------------------------------------------------//
CDateTime::CDateTime() : DATE_TIME_BASE_YEAR(1900), DATE_TIME_ESC_CHAR('\\'), MAX_DATE_TIME_STR_LEN(128)
{
    try
    {
        Clear();
        SetToNow();
        SetEscChar(DATE_TIME_ESC_CHAR);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::CDateTime(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CDateTime::CDateTime(long lSecond, long lMicroSecond) : DATE_TIME_BASE_YEAR(1900), DATE_TIME_ESC_CHAR('\\'), MAX_DATE_TIME_STR_LEN(128)
{
    try
    {
        SetTimeStamp(lSecond, lMicroSecond);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::CDateTime(long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CDateTime::CDateTime(const CDateTime& objDateTime) : DATE_TIME_BASE_YEAR(1900), DATE_TIME_ESC_CHAR('\\'), MAX_DATE_TIME_STR_LEN(128)
{
    operator = (objDateTime);
}

CDateTime::~CDateTime()
{
}

// 
void CDateTime::Clear()
{
    // 
    m_stTimeVal.tv_sec          = 0;
    m_stTimeVal.tv_usec         = 0;
}

//---------------------------------------------------------------------------------------------//
const CDateTime& CDateTime::operator = (const CDateTime& objDateTime)
{
    if(this != &objDateTime)
    {
        // 
        m_stTimeVal.tv_sec     = objDateTime.m_stTimeVal.tv_sec;
        m_stTimeVal.tv_usec    = objDateTime.m_stTimeVal.tv_usec;
    }
    return(*this);
}

// 
CDateTime CDateTime::operator + (const CTimeInterval& objTimeInterval) const
{
    try
    {
        return(CDateTime((long)m_stTimeVal.tv_sec + objTimeInterval.m_stTimeInterval.lSecond, 
            (long)m_stTimeVal.tv_usec + objTimeInterval.m_stTimeInterval.lMicroSecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::operator + (const CTimeInterval&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CDateTime CDateTime::operator - (const CTimeInterval& objTimeInterval) const
{
    try
    {
        return(CDateTime((long)m_stTimeVal.tv_sec - objTimeInterval.m_stTimeInterval.lSecond, 
            (long)m_stTimeVal.tv_usec - objTimeInterval.m_stTimeInterval.lMicroSecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::operator - (const CTimeInterval&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CTimeInterval CDateTime::operator - (const CDateTime& objDateTime) const
{
    try
    {
        return(CTimeInterval((long)m_stTimeVal.tv_sec - (long)objDateTime.m_stTimeVal.tv_sec, 
            (long)m_stTimeVal.tv_usec - (long)objDateTime.m_stTimeVal.tv_usec));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::operator - (const CDateTime&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CDateTime CDateTime::operator + (long lMicrosecond) const
{
    try
    {
        return(CDateTime((long)m_stTimeVal.tv_sec, (long)m_stTimeVal.tv_usec + lMicrosecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::operator + (long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CDateTime CDateTime::operator - (long lMicrosecond) const
{
    try
    {
        return(CDateTime((long)m_stTimeVal.tv_sec, (long)m_stTimeVal.tv_usec - lMicrosecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::operator - (long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CDateTime::operator == (const CDateTime& objDateTime) const 
{
    return(m_stTimeVal.tv_sec == objDateTime.m_stTimeVal.tv_sec && m_stTimeVal.tv_usec == objDateTime.m_stTimeVal.tv_usec);
}

bool CDateTime::operator < (const CDateTime& objDateTime) const 
{
    if(m_stTimeVal.tv_sec < objDateTime.m_stTimeVal.tv_sec) return(true);
    else if(m_stTimeVal.tv_sec < objDateTime.m_stTimeVal.tv_sec) return(false);
    else
    {
        if(m_stTimeVal.tv_usec < objDateTime.m_stTimeVal.tv_usec) return(true);
        else return(false);
    }
}

//---------------------------------------------------------------------------------------------//
void CDateTime::SetTimeStamp(long lSecond, long lMicroSecond)
{
    long lSecondFinal, lMicroSecondFinal, lSecondRound;
    lSecondRound = lMicroSecond/(1000*1000);
    // Time value: 
    lSecondFinal        = lSecond + lSecondRound;
    lMicroSecondFinal    = lMicroSecond%(1000*1000);
    
    // If two parts has the different sign, try to make them the same sign: 
    if(lSecondFinal > 0 && lMicroSecondFinal < 0)
    {
        lSecondFinal--;
        lMicroSecondFinal += (1000*1000);
    }else if(lSecondFinal < 0 && lMicroSecondFinal > 0)
    {
        lSecondFinal++;
        lMicroSecondFinal -= (1000*1000);
    }else
    {
        // If one is 0, do nothing.
    }
    
    // 
    if(lSecondFinal < 0 || lMicroSecondFinal < 0)
    {
        throw CException(-1, "%s(%ld): CDateTime::SetTimeStamp(): Invalid value, all final values need to be positive: Input %ld, %ld, final: %ld, %ld.", __FILE__, __LINE__, 
            lSecond, lMicroSecond, lSecondFinal, lMicroSecondFinal);
    }
    // Time value: 
    m_stTimeVal.tv_sec          = lSecondFinal;
    m_stTimeVal.tv_usec         = lMicroSecondFinal;
}

// 
std::string CDateTime::GetTimeStamp() const
{
    char szDateTimeStamp[32];
    int nLen = snprintf(szDateTimeStamp, sizeof(szDateTimeStamp), "%010ld.%06ld", m_stTimeVal.tv_sec, m_stTimeVal.tv_usec);
    if(nLen >= sizeof(szDateTimeStamp))
    {
        throw CException(-1, "%s(%ld): CDateTime::GetTimeStamp(): Trancated, buffer too small, only %d, need %d bytes.", __FILE__, __LINE__, 
            sizeof(szDateTimeStamp), nLen);
    }
    return(std::string(szDateTimeStamp));
}

//---------------------------------------------------------------------------------------------//
void CDateTime::SetSecond(long lSecond)
{
    SetTimeStamp(lSecond, GetMicroSecond());
}

long CDateTime::GetSecond() const
{
    return(m_stTimeVal.tv_sec);
}


void CDateTime::SetMicroSecond(long lMicroSecond)
{
    SetTimeStamp(GetSecond(), lMicroSecond);
}

long CDateTime::GetMicroSecond() const
{
    return(m_stTimeVal.tv_usec);
}

//---------------------------------------------------------------------------------------------//
void CDateTime::SetToNow()
{
    if(gettimeofday(&m_stTimeVal, NULL) != 0)
    {
        char szErr[256];
        throw CException(-1, "%s(%ld): CDateTime::SetToNow(): gettimeofday() failed: %d:[%s]", __FILE__, __LINE__, errno, strerror_r(errno, szErr, sizeof(szErr)));
    }
}

//---------------------------------------------------------------------------------------------//
void CDateTime::GetTimeVal(struct timeval& stTimeVal) const
{
    stTimeVal.tv_sec    = m_stTimeVal.tv_sec;
    stTimeVal.tv_usec   = m_stTimeVal.tv_usec;
}

void CDateTime::GetTimeSpec(struct timespec& stTimeSpec) const
{
    stTimeSpec.tv_sec   = m_stTimeVal.tv_sec;
    stTimeSpec.tv_nsec  = m_stTimeVal.tv_usec * 1000;   // Convert to nanosecond.
}

//---------------------------------------------------------------------------------------------//
void CDateTime::SetEscChar(char cEscChar)
{
    if('\0' == cEscChar) throw CException(-1, "%s(%ld): CDateTime::SetEscChar(): Invalid escape character: [%c],[%02x]", __FILE__, __LINE__, cEscChar, cEscChar);
    m_cEscChar = cEscChar;
}

// 
void CDateTime::SetDateTime(const char* szDateTime, const char* szFormat)
{
    try
    {
        if(NULL == szDateTime || NULL == szFormat) throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): NULL pointer.", __FILE__, __LINE__);
        // 
        CValue objValue;
        int nCnt1, nCnt2, nCnt3;
        struct tm stTime;
        
        //2.If make a date time: 
        // Set default time field values. 
        stTime.tm_sec=0;
        stTime.tm_min=0;
        stTime.tm_hour=0;
        stTime.tm_mday=1;
        stTime.tm_mon=0;
        stTime.tm_year=0;   // The number of years since 1900.
        stTime.tm_wday=0;
        stTime.tm_yday=0;
        stTime.tm_isdst=0;
        
        // Analyze szDateTime with szFormat:
        for(nCnt1=0, nCnt2=0; szFormat[nCnt1] != '\0' && szDateTime[nCnt2] != '\0';)
        {
            if(szFormat[nCnt1] == m_cEscChar) nCnt1 += 2, nCnt2 += 2;
            else if(toupper(szFormat[nCnt1]) == 'D')
            {
                if(toupper(szFormat[nCnt1+1]) == 'D')
                {
                    if(toupper(szFormat[nCnt1+2]) == 'D')
                    {   // 'DDD' day of year(1-366):(0-365).
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'DDD' yet, [%s][%s].", __FILE__, __LINE__,
                            szFormat, szDateTime);
                    }else
                    {   // 'DD' day of month(1-31):(0-11).
                        objValue.SetValue(&szDateTime[nCnt2], 2);
                        stTime.tm_mday = objValue.ToLong(1);
                        if(stTime.tm_mday < 1 || stTime.tm_mday > 31)
                        {
                            throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid month: %s(1-31), [%s][%s].", __FILE__, __LINE__, 
                                objValue.ToString().c_str(), szFormat, szDateTime);
                        }
                        nCnt1 += 2; nCnt2 += 2;
                    }
                }else if(toupper(szFormat[nCnt1+1]) == 'A' && toupper(szFormat[nCnt1+2]) == 'Y')
                {   // 'DAY' name of day.
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'DAY' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }else if(toupper(szFormat[nCnt1+1]) == 'Y')
                {   // 'DY' abbreviated name of day.
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'DY' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }else
                {   // 'D' day of week(1-7).
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'D' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }
            }else if(toupper(szFormat[nCnt1]) == 'H' && toupper(szFormat[nCnt1+1]) == 'H')
            {
                if(toupper(szFormat[nCnt1+2]) == '1' && toupper(szFormat[nCnt1+3]) == '2')
                {   // 'HH12' hour of day(1-12).
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'HH12' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }else if(toupper(szFormat[nCnt1+2]) == '2' && toupper(szFormat[nCnt1+3]) == '4')
                {
                    // 'HH24' hour of day(0-23).
                    objValue.SetValue(&szDateTime[nCnt2], 2);
                    stTime.tm_hour = objValue.ToLong(0);
                    if(stTime.tm_hour < 0 || stTime.tm_hour > 23)
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid hour(0-23): %s, [%s][%s].", __FILE__, __LINE__, 
                            objValue.ToString().c_str(), szFormat, szDateTime);
                    }
                    nCnt1 += 4;   nCnt2 += 2;
                }else
                {   // 'HH' hour of day(1-12)
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'HH' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }
            }else if(toupper(szFormat[nCnt1]) == 'M')
            {
                if(toupper(szFormat[nCnt1+1]) == 'I')
                {    // 'MI' minute(0-59).
                    objValue.SetValue(&szDateTime[nCnt2], 2);
                    stTime.tm_min = objValue.ToLong(0);
                    if(stTime.tm_min < 0 || stTime.tm_min > 59)
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid minute(0-59): %s, [%s][%s].", __FILE__, __LINE__, 
                            objValue.ToString().c_str(), szFormat, szDateTime);
                    }
                    nCnt1 += 2;   nCnt2 += 2;
                }else if(toupper(szFormat[nCnt1+1])=='M')
                {   // 'MM' month(01-12).
                    objValue.SetValue(&szDateTime[nCnt2], 2);
                    stTime.tm_mon = objValue.ToLong(1);
                    stTime.tm_mon--;
                    if(stTime.tm_mon < 0 || stTime.tm_mon > 11)
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid month(01-12): %s, [%s][%s].", __FILE__, __LINE__, 
                            objValue.ToString().c_str(), szFormat, szDateTime);
                    }
                    nCnt1+=2;   nCnt2+=2;
                }else if(toupper(szFormat[nCnt1+1]) == 'O' && toupper(szFormat[nCnt1+2]) == 'N' && 
                    toupper(szFormat[nCnt1+3]) == 'T' && toupper(szFormat[nCnt1+4])=='H')
                {
                    // 'MONTH' name of mounth.
                    for(nCnt3 = 0; nCnt3 < 12; nCnt3++)
                    {
                        if(strncasecmp(&szDateTime[nCnt2], szaMonTab[nCnt3], strlen(szaMonTab[nCnt3])) == 0) break;
                    }
                    if(nCnt3 < 12)
                    {
                        stTime.tm_mon = nCnt3;
                        nCnt1 += 5;   nCnt2 += strlen(szaMonTab[nCnt3]);
                    }else
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid month name: [%s][%s].", __FILE__, __LINE__, 
                            szFormat, szDateTime);
                    }
                }else if(toupper(szFormat[nCnt1+1]) == 'O' && toupper(szFormat[nCnt1+2]) == 'N')
                {  
                    // 'MONTH' abrivation name of mounth 
                    for(nCnt3 = 0; nCnt3 < 12; nCnt3++)
                    {
                        if(strncasecmp(&szDateTime[nCnt2], szaMonTabAbr[nCnt3], strlen(szaMonTab[nCnt3])) == 0) break;
                    }
                    if(nCnt3 < 12)
                    {
                        stTime.tm_mon = nCnt3;
                        nCnt1 += 3;   nCnt2 += strlen(szaMonTabAbr[nCnt3]);
                    }else
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid month abbriviation: [%s][%s].", __FILE__, __LINE__, 
                            szFormat, szDateTime);
                    }
                }else
                {
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1]) == 'W')
            {
                if(toupper(szFormat[nCnt1+1]) == 'W')
                {
                    // 'WW' week of year(1-53) where week 1
                    // starts on the first day of the year and continues to the
                    // seventh day of the year.
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'WW' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }else
                {
                    // 'W' week of month(1-5) where week 1 starts on the
                    // first day of the month and ends on the seventh.
                    throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Doesn't support format: 'W' yet, [%s][%s].", __FILE__, __LINE__,
                        szFormat, szDateTime);
                }
            }else if(toupper(szFormat[nCnt1]) == 'S')
            {
                if(toupper(szFormat[nCnt1+1]) == 'S')
                {   // 'SS' second(0-59).
                    objValue.SetValue(&szDateTime[nCnt2], 2);
                    stTime.tm_sec = objValue.ToLong(0);
                    if(stTime.tm_mon < 0 || stTime.tm_mon > 59)
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid second(01-59): %s, [%s][%s].", __FILE__, __LINE__, 
                            objValue.ToString().c_str(), szFormat, szDateTime);
                    }
                    nCnt1 += 2;   nCnt2 += 2;
                }else
                {
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1]) == 'Y')
            {
                if(toupper(szFormat[nCnt1+1]) == 'Y')
                {
                    if(toupper(szFormat[nCnt1+2]) == 'Y')
                    {
                        if(toupper(szFormat[nCnt1+3]) == 'Y')
                        {   // 'YYYY'
                            objValue.SetValue(&szDateTime[nCnt2], 4);
                            stTime.tm_year = objValue.ToLong(0) - DATE_TIME_BASE_YEAR;
                            if(stTime.tm_year < 0)
                            {
                                throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid year(1900-): %s, [%s][%s].", __FILE__, __LINE__, 
                                    objValue.ToString().c_str(), szFormat, szDateTime);
                            }
                            nCnt1 += 4;   nCnt2 += 4;
                        }else
                        {   // 'YYY' 
                            objValue.SetValue(&szDateTime[nCnt2], 3);
                            stTime.tm_year = objValue.ToLong(0) - (DATE_TIME_BASE_YEAR - 1000);
                            if(stTime.tm_year < 0)
                            {
                                throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid year(1900-): %s, [%s][%s].", __FILE__, __LINE__, 
                                    objValue.ToString().c_str(), szFormat, szDateTime);
                            }
                            nCnt1 += 3;   nCnt2 += 3;
                        }
                    }else
                    {   // 'YY' 
                        objValue.SetValue(&szDateTime[nCnt2], 2);
                        if(objValue.ToLong(0) > 90)
                        {
                            stTime.tm_year = 1900 + objValue.ToLong(0) - DATE_TIME_BASE_YEAR;
                        }else
                        {
                            stTime.tm_year = 2000 + objValue.ToLong(0) - DATE_TIME_BASE_YEAR;
                        }
                        if(stTime.tm_year < 0)
                        {
                            throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid year(1900-): %s, [%s][%s].", __FILE__, __LINE__, 
                                objValue.ToString().c_str(), szFormat, szDateTime);
                        }
                        nCnt1 += 2;   nCnt2 += 2;
                    }
                }else
                {   // 'Y'
                    time_t lTmpTime;
                    char *spTmpTime;
                    //Get system year first:
                    lTmpTime    = time(NULL);
                    spTmpTime   = ctime(&lTmpTime);
                    objValue.SetValue(spTmpTime + strlen(spTmpTime) - 5 , 3);
                    long lYear3     = objValue.ToLong();
                    objValue.SetValue(&szDateTime[nCnt2], 1);
                    stTime.tm_year  = lYear3 * 10 + objValue.ToLong(0) - DATE_TIME_BASE_YEAR;
                    if(stTime.tm_year < 0)
                    {
                        throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): Invalid year(1900-): %s, [%s][%s].", __FILE__, __LINE__, 
                            objValue.ToString().c_str(), szFormat, szDateTime);
                    }
                    nCnt1++;    nCnt2++;
                }
            }else
            {
                nCnt1++;    nCnt2++;
            }
        }

        if((m_stTimeVal.tv_sec = mktime(&stTime)) == -1)
        {
            throw CException(-1, "%s(%ld): CDateTime::SetDateTime(): mktime() failed. [%s], [%s]", __FILE__, __LINE__, szDateTime, szFormat);
        }
        m_stTimeVal.tv_usec = 0;
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::SetDateTime(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
std::string CDateTime::ToString(const char* szFormat) const
{
    try
    {
        char szDateTime[MAX_DATE_TIME_STR_LEN], szTmp[32];
        int  nCnt1, nCnt2;
        struct tm *stpTime;
        // 
        stpTime = localtime(&m_stTimeVal.tv_sec);
        // 
        for(nCnt1 = 0, nCnt2 = 0; szFormat[nCnt1] !='\0' && nCnt2 < MAX_DATE_TIME_STR_LEN - 1;)
        {
            if(szFormat[nCnt1] == m_cEscChar)
            {
                szDateTime[nCnt2] = szFormat[nCnt1+1];
                nCnt1 += 2;   nCnt2++;
            }else if(toupper(szFormat[nCnt1])=='D')
            {
                if(toupper(szFormat[nCnt1+1])=='D')
                {
                    if(toupper(szFormat[nCnt1+2])=='D')
                    {   // 'DDD' day of year(1-366).
                        sprintf(szTmp, "%03d", stpTime->tm_yday + 1);
                        strncpy(&szDateTime[nCnt2], szTmp, 3);
                        nCnt1 += 3;   nCnt2 += 3;
                    }else
                    {   // 'DD' day of month(1-31).
                        sprintf(szTmp, "%02d", stpTime->tm_mday);
                        strncpy(&szDateTime[nCnt2], szTmp, 2);
                        nCnt1 += 2;   nCnt2 += 2;
                    }
                }else if(toupper(szFormat[nCnt1+1]) == 'A' && toupper(szFormat[nCnt1+2]) == 'Y')
                {   // 'DAY' name of day.
                    sprintf(szTmp, "%s", szaDayTab[stpTime->tm_wday]);
                    strncpy(&szDateTime[nCnt2], szTmp, strlen(szTmp));
                    nCnt1 += 3;   nCnt2 += strlen(szTmp);
                }else if(toupper(szFormat[nCnt1+1]) == 'Y')
                {   // 'DY' abbreviated name of day.
                    sprintf(szTmp, "%s", szaDayTabAbr[stpTime->tm_wday]);
                    strncpy(&szDateTime[nCnt2], szTmp, strlen(szTmp));
                    nCnt1 += 2;   nCnt2 += strlen(szTmp);
                }else
                {   // 'D' day of week(1-7).
                    szDateTime[nCnt2] = stpTime->tm_wday + 1 + '0';
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1]) == 'H' && toupper(szFormat[nCnt1+1]) == 'H')
            {
                if(toupper(szFormat[nCnt1+2]) == '1' && toupper(szFormat[nCnt1+3]) == '2')
                {   // 'HH12' hour of day(1-12).
                    sprintf(szTmp, "%02d", (stpTime->tm_hour % 12 == 0 ? 12 : stpTime->tm_hour % 12));
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 4;   nCnt2 += 2;
                }else if(toupper(szFormat[nCnt1+2]) == '2' && toupper(szFormat[nCnt1+3]) == '4')
                {   // 'HH24' hour of day(0-23).
                    sprintf(szTmp, "%02d", stpTime->tm_hour);
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 4;   nCnt2 += 2;
                }else
                {   // 'HH' hour of day(1-12)
                    sprintf(szTmp,"%02d", (stpTime->tm_hour % 12 == 0 ? 12 : stpTime->tm_hour % 12));
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 2;   nCnt2 += 2;
                }
            }else if(toupper(szFormat[nCnt1]) == 'M')
            {
                if(toupper(szFormat[nCnt1+1]) == 'I')
                {   // 'MI' minute(0-59).
                    sprintf(szTmp, "%02d", stpTime->tm_min);
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 2;   nCnt2 += 2;
                }else if(toupper(szFormat[nCnt1+1]) == 'M')
                {   // 'MM' month(01-12).
                    sprintf(szTmp, "%02d", stpTime->tm_mon + 1);
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 2;   nCnt2 += 2;
                }else if(toupper(szFormat[nCnt1+1]) == 'O' && toupper(szFormat[nCnt1+2]) == 'N' && 
                    toupper(szFormat[nCnt1+3]) == 'T' && toupper(szFormat[nCnt1+4])=='H')
                {   // 'MONTH' name of mounth 
                    sprintf(szTmp, "%s", szaMonTab[stpTime->tm_mon]);
                    strncpy(&szDateTime[nCnt2], szTmp, strlen(szTmp));
                    nCnt1 += 5;   nCnt2 += strlen(szTmp);
                }else if(toupper(szFormat[nCnt1+1]) == 'O' && toupper(szFormat[nCnt1+2]) == 'N')
                {   // 'MONTH' abbrivation name of mounth 
                    sprintf(szTmp, "%s", szaMonTabAbr[stpTime->tm_mon]);
                    strncpy(&szDateTime[nCnt2], szTmp, strlen(szTmp));
                    nCnt1 += 3;   nCnt2 += strlen(szTmp);
                }else
                {
                    szDateTime[nCnt2] = szFormat[nCnt1];
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1])=='W')
            {
                if(toupper(szFormat[nCnt1+1])=='W')
                {   // 'WW' week of year(1-53) where week 1
                    // starts on the first day of the year and continues to the
                    // seventh day of the year.
                    sprintf(szTmp, "%02d", (stpTime->tm_yday + 1) / 7 + 1);
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 2;   nCnt2 += 2;
                }else
                {   // 'W' week of month(1-5) where week 1 starts on the
                    //first day of the month and ends on the seventh.
                    //sprintf(szTmp,"%d",stpTime->tm_mday/7+1);
                    szDateTime[nCnt2] = szTmp[0];
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1])=='S')
            {
                if(toupper(szFormat[nCnt1+1])=='S')
                {   // 'SS' second(0-59).
                    sprintf(szTmp,"%02d", stpTime->tm_sec);
                    strncpy(&szDateTime[nCnt2], szTmp, 2);
                    nCnt1 += 2;   nCnt2 += 2;
                }else
                {
                    szDateTime[nCnt2] = szFormat[nCnt1];
                    nCnt1++;    nCnt2++;
                }
            }else if(toupper(szFormat[nCnt1]) == 'Y')
            {
                sprintf(szTmp,"%04d", stpTime->tm_year + DATE_TIME_BASE_YEAR);
                if(toupper(szFormat[nCnt1+1]) == 'Y')
                {
                    if(toupper(szFormat[nCnt1+2]) == 'Y')
                    {
                        if(toupper(szFormat[nCnt1+3])=='Y')
                        {   // 'YYYY' 
                            strncpy(&szDateTime[nCnt2], szTmp, 4);
                            nCnt1 += 4;   nCnt2 += 4;
                        }else
                        {   // 'YYY' 
                            strncpy(&szDateTime[nCnt2], &szTmp[1], 3);
                            nCnt1 += 3;   nCnt2 += 3;
                        }
                    }else
                    {   // 'YY' 
                        strncpy(&szDateTime[nCnt2], &szTmp[2], 2);
                        nCnt1 += 2;   nCnt2 += 2;
                    }
                }else
                {   // 'Y' 
                    szDateTime[nCnt2] = szTmp[3];
                    nCnt1++;    nCnt2++;
                }
            }else
            {
                szDateTime[nCnt2] = szFormat[nCnt1];
                nCnt1++;    nCnt2++;
            }
        }
        if(nCnt2 == (MAX_DATE_TIME_STR_LEN-1))
        {
            throw CException(-1, "%s(%ld): CDateTime::ToString(const char*): Buffer overflowed, no more than: %d, [%s].", __FILE__, __LINE__, 
                MAX_DATE_TIME_STR_LEN, szFormat);
        }
        szDateTime[nCnt2]='\0';
        return(std::string(szDateTime));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CDateTime::ToString(const char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
