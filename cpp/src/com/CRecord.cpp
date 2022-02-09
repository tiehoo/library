/* =============================================================================
// Name:         CRecord.cpp
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
#include <stdio.h>
#include <algorithm>
#include "CException.h"
#include "CRecord.h"

// --------------------------------------------------------------------------- /
// CRecord 1.0
// --------------------------------------------------------------------------- /
CRecord::CRecord()
{
    Clear();
}

CRecord::~CRecord()
{
}

// --------------------------------------------------------------------------- /
void CRecord::Clear()
{
    m_mapRecord.clear();
}

// --------------------------------------------------------------------------- /
void CRecord::Add(const std::string sName, const CValue& objValue) 
{
    if(sName.empty())
    {
        throw CException(-1, "%s(%ld): CRecord::Add(): sName is empty.", __FILE__, __LINE__);
    }
    std::string sNameLower(sName);
    // 
    std::transform(sNameLower.begin(), sNameLower.end(), sNameLower.begin(), ::tolower);
    // 
    m_mapRecord.insert(std::pair< std::string, CValue >(sNameLower, objValue));
}

// 
bool CRecord::GetValue(const std::string sName, CValue& objValue) const
{
    if(sName.empty())
    {
        return(false);
    }
    // 
    std::string sNameLower(sName);
    std::transform(sNameLower.begin(), sNameLower.end(), sNameLower.begin(), ::tolower);
    // 
    std::map<std::string, CValue>::const_iterator it;
    it = m_mapRecord.find(sNameLower);
    if(it != m_mapRecord.end())
    {
        objValue = it->second;
        return(true);
    }else 
    {
        return(false);
    }
}

// 
int CRecord::GetSize() const
{
    return(m_mapRecord.size());
}

// 
void  CRecord::Print() const 
{
    std::map<std::string, CValue>::const_iterator it;
    for(it = m_mapRecord.begin(); it != m_mapRecord.end(); ++it)
    {
        printf("[%s]: [%s]\n", it->first.c_str(), it->second.ToString().c_str());
    }
}
