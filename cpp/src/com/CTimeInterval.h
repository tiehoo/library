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
#ifndef _CTIMEINTERVAL_H
#define _CTIMEINTERVAL_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <sys/time.h>
#include <string>

// =========================================================================== /
// CTimeInterval 1.0 
// =========================================================================== /
// Constants: 
//  Time stamp: Signed.
typedef struct stTimeInterval
{
    long    lSecond;
    long    lMicroSecond;
    // 
    void    Clear()
    {
        lSecond         = 0;
        lMicroSecond    = 0;
    }
    // 
    stTimeInterval()
    {
        Clear();
    }
} tTimeInterval;

// 
class CTimeInterval
{
private: 
    // Time structure: m_stTime.tv_sec, m_stTime.tv_usec.
    tTimeInterval           m_stTimeInterval;   // Used for time operation and calculation.
public: 
    CTimeInterval();                            // Initialize to 0.
    // Don't use time_t or suseconds_t here, long is signed and good for operator + or -.
    CTimeInterval(long lSecond, long lMicroSecond);
    CTimeInterval(const CTimeInterval& objTimeInterval);
    ~CTimeInterval();
// 
    void                    Clear();
// Set and auto adjust: 
    void                    SetTimeInterval(long lSecond, long lMicroSecond);
    void                    GetTimeInterval(tTimeInterval& stTimeInterval) const;
    tTimeInterval           GetTimeInterval();
// 
    // second: 
    void                    SetSecond(long lSecond);
    long                    GetSecond() const;
    // microsecond: 
    void                    SetMicroSecond(long lMicroSecond);
    long                    GetMicroSecond() const;
    // 0: if both are 0; 1: if positive, -1: if negative.
    int                     Sign() const;
// 
    const CTimeInterval&    operator =  (const CTimeInterval& objTimeInterval);
    // 
    CTimeInterval           operator +  (const CTimeInterval& objTimeInterval) const;
    CTimeInterval           operator -  (const CTimeInterval& objTimeInterval) const;
    CTimeInterval           operator +  (long  lMicrosecond) const;
    CTimeInterval           operator -  (long  lMicrosecond) const;
    // 
    CTimeInterval           operator *  (float fPercentage) const;    // Add x% up as time interval.
    // 
    bool                    operator == (const CTimeInterval& objTimeInterval) const;
    bool                    operator <  (const CTimeInterval& objTimeInterval) const;
// 
    // Used for select() etc. I will not return negative values.
    CTimeInterval           Abs() const;
    void                    GetTimeVal(struct timeval& stTimeVal) const;
// 
    friend class CDateTime;
};

#endif
