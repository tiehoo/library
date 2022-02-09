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
#include <stdlib.h>
#include "CException.h"
#include "CTimeInterval.h"

//---------------------------------------------------------------------------------------------//
// CTimeInterval: 
//---------------------------------------------------------------------------------------------//
CTimeInterval::CTimeInterval()
{
}

CTimeInterval::CTimeInterval(long lSecond, long lMicroSecond)
{
    try
    {
        SetTimeInterval(lSecond, lMicroSecond);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::CTimeInterval(long, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval::CTimeInterval(const CTimeInterval& objTimeInterval)
{
    try
    {
        operator = (objTimeInterval);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::CTimeInterval(const CTimeInterval&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval::~CTimeInterval()
{
}

void CTimeInterval::Clear()
{
    m_stTimeInterval.Clear();
}

//---------------------------------------------------------------------------------------------//
void CTimeInterval::SetTimeInterval(long lSecond, long lMicroSecond)
{
    long lSecondRound;
    lSecondRound = lMicroSecond/(1000*1000);
    // Time interval: 
    m_stTimeInterval.lSecond       = lSecond + lSecondRound;
    m_stTimeInterval.lMicroSecond  = lMicroSecond%(1000*1000);
    
    // If two parts has the different sign, try to make them the same sign: 
    if(m_stTimeInterval.lSecond > 0 && m_stTimeInterval.lMicroSecond < 0)
    {
        m_stTimeInterval.lSecond--;
        m_stTimeInterval.lMicroSecond += (1000*1000);
    }else if(m_stTimeInterval.lSecond < 0 && m_stTimeInterval.lMicroSecond > 0)
    {
        m_stTimeInterval.lSecond++;
        m_stTimeInterval.lMicroSecond -= (1000*1000);
    }else
    {
        // If one is 0, do nothing.
    }
}

void CTimeInterval::GetTimeInterval(tTimeInterval& stTimeInterval) const
{
    stTimeInterval.lSecond        = m_stTimeInterval.lSecond;
    stTimeInterval.lMicroSecond   = m_stTimeInterval.lMicroSecond;
}

tTimeInterval CTimeInterval::GetTimeInterval()
{
    return(m_stTimeInterval);
}

//---------------------------------------------------------------------------------------------//
void CTimeInterval::SetSecond(long lSecond)
{
    try
    {
        SetTimeInterval(lSecond, GetMicroSecond());
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::SetSecond(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
long CTimeInterval::GetSecond() const
{
    return(m_stTimeInterval.lSecond);
}

// 
void CTimeInterval::SetMicroSecond(long lMicroSecond)
{
    try
    {
        SetTimeInterval(GetSecond(), lMicroSecond);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::SetMicroSecond(long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
long CTimeInterval::GetMicroSecond() const
{
    return(m_stTimeInterval.lMicroSecond);
}

// 
int CTimeInterval::Sign() const 
{
    if(m_stTimeInterval.lSecond > 0) return(1);
    else if(m_stTimeInterval.lSecond < 0) return(-1);
    else
    {
        if(m_stTimeInterval.lMicroSecond > 0) return(1);
        else if(m_stTimeInterval.lMicroSecond < 0) return(-1);
        else return(0);
    }
}

//---------------------------------------------------------------------------------------------//
const CTimeInterval& CTimeInterval::operator = (const CTimeInterval& objTimeInterval)
{
    if(this != &objTimeInterval)
    {
        m_stTimeInterval.lSecond       = objTimeInterval.m_stTimeInterval.lSecond;
        m_stTimeInterval.lMicroSecond  = objTimeInterval.m_stTimeInterval.lMicroSecond;
    }
    return(*this);
}

CTimeInterval CTimeInterval::operator + (const CTimeInterval& objTimeInterval) const
{
    try
    {
        // 
        return(CTimeInterval((long)(m_stTimeInterval.lSecond + objTimeInterval.m_stTimeInterval.lSecond), 
            (long)(m_stTimeInterval.lMicroSecond + objTimeInterval.m_stTimeInterval.lMicroSecond)));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::operator + (const CTimeInterval&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval CTimeInterval::operator - (const CTimeInterval& objTimeInterval) const
{
    try
    {
        return(CTimeInterval(m_stTimeInterval.lSecond - objTimeInterval.m_stTimeInterval.lSecond, 
            m_stTimeInterval.lMicroSecond - objTimeInterval.m_stTimeInterval.lMicroSecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::operator - (const CTimeInterval&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval CTimeInterval::operator + (long lMicrosecond) const
{
    try
    {
        // Add usec first: 
        return(CTimeInterval(m_stTimeInterval.lSecond, m_stTimeInterval.lMicroSecond + lMicrosecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::operator + (long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval CTimeInterval::operator - (long lMicrosecond) const
{
    try
    {
        return(CTimeInterval(m_stTimeInterval.lSecond, m_stTimeInterval.lMicroSecond - lMicrosecond));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::operator - (long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CTimeInterval CTimeInterval::operator * (float fPercentage) const
{
    try
    {
        return(CTimeInterval((long)(m_stTimeInterval.lSecond * fPercentage), (long)(m_stTimeInterval.lMicroSecond * fPercentage)));
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CTimeInterval::operator * (float): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CTimeInterval::operator == (const CTimeInterval& objTimeInterval) const
{
    return(m_stTimeInterval.lSecond == objTimeInterval.m_stTimeInterval.lSecond && m_stTimeInterval.lMicroSecond == objTimeInterval.m_stTimeInterval.lMicroSecond);
}

bool CTimeInterval::operator < (const CTimeInterval& objTimeInterval) const 
{
    if(m_stTimeInterval.lSecond < objTimeInterval.m_stTimeInterval.lSecond) return(true);
    else if(m_stTimeInterval.lSecond < objTimeInterval.m_stTimeInterval.lSecond) return(false);
    else
    {
        if(m_stTimeInterval.lMicroSecond < objTimeInterval.m_stTimeInterval.lMicroSecond) return(true);
        else return(false);
    }
}

// 
CTimeInterval CTimeInterval::Abs() const 
{
    return(CTimeInterval(labs(m_stTimeInterval.lSecond), labs(m_stTimeInterval.lMicroSecond)));
}

void CTimeInterval::GetTimeVal(struct timeval& stTimeVal) const 
{
    stTimeVal.tv_sec    = labs(m_stTimeInterval.lSecond);
    stTimeVal.tv_usec   = labs(m_stTimeInterval.lMicroSecond);
}
