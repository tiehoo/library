/* =============================================================================
// Name:         CXmlXPath.h
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
#ifndef _CXMLXPATH_H
#define _CXMLXPATH_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <list>
#include <libxml/tree.h>
#include "CValue.h"
#include "CRecord.h"

// --------------------------------------------------------------------------- /
// CXmlXPath 1.0
// --------------------------------------------------------------------------- /
class CXmlXPath 
{
private: 
    xmlDocPtr           m_pDoc;
    // 
    bool                GetValue(const xmlNodePtr pNode, CValue& objValue) const;
    bool                GetRecord(const xmlNodePtr pNode, CRecord& objRecord) const;
    /*
    bool                GetRecordList(const xmlNodePtr pNode, std::list<CRecord>& listRecord) const;
    */
public: 
    // 
    CXmlXPath(const char* szXml);
    ~CXmlXPath();
    // 
    bool                GetValue(const char* xPathExpression, CValue& objValue) const;
    bool                GetRecord(const char* xPathExpression, CRecord& objRecord) const;
    bool                GetRecordList(const char* xPathExpression, std::list<CRecord>& listRecord) const;
};

#endif
