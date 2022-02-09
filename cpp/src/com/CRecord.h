/* =============================================================================
// Name:         CRecord.h
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
#ifndef _CRECORD_H
#define _CRECORD_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <map>
#include "CValue.h"

// --------------------------------------------------------------------------- /
// CRecord 1.0
// --------------------------------------------------------------------------- /
class CRecord
{
private: 
    std::map<std::string, CValue>  m_mapRecord;
public: 
    // 
    CRecord();
    ~CRecord();
    // 
    void            Clear();
    // 
    void            Add(const std::string sName, const CValue& objValue);
    // 
    bool            GetValue(const std::string sName, CValue& objValue) const;
    // 
    int             GetSize() const;
    // 
    void            Print() const;
};

#endif
