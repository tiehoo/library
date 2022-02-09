/* =============================================================================
// Name:         CXmlXPath.cpp
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
#include <string.h>
#include "CException.h"
#include "CXmlXPath.h"
#include <libxml/xpath.h>

// --------------------------------------------------------------------------- /
// CXmlXPath 1.0
// --------------------------------------------------------------------------- /
CXmlXPath::CXmlXPath(const char* szXml)
{
    m_pDoc = NULL;
    m_pDoc = xmlParseMemory(szXml, strlen(szXml));
    // 
    if(NULL == m_pDoc) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::CXmlXPath(): Parse failed.", __FILE__, __LINE__);
    }
}

CXmlXPath::~CXmlXPath()
{
    if(NULL != m_pDoc) 
    {
        xmlFreeDoc(m_pDoc);
        m_pDoc = NULL;
    }
}

// --------------------------------------------------------------------------- /
bool CXmlXPath::GetValue(const xmlNodePtr pNode, CValue& objValue) const
{
    // 
    if(NULL == pNode) 
    {
        return(false);
    }
    if(NULL == pNode->children)
    {
        return(false);
    }
    if(XML_TEXT_NODE != pNode->children->type) 
    {
        // I only return the content of the leaf node. 
        return(false);
    }
    // 
    xmlChar* pValue = NULL;
    pValue = xmlNodeGetContent(pNode);
    objValue.SetValue((char *)pValue);
    xmlFree(pValue);
    // 
    return(true);
}

bool CXmlXPath::GetRecord(const xmlNodePtr pNode, CRecord& objRecord) const
{
    // 
    if(NULL == pNode) 
    {
        return(false);
    }
    if(NULL == pNode->children) 
    {
        return(false);
    }
    if(XML_ELEMENT_NODE != pNode->children->type && XML_TEXT_NODE != pNode->children->type) 
    {
        // 
        return(false);
    }
    // Traval into child node: 
    bool bDidFind = false;
    CValue objValue;
    xmlNodePtr pNext;
    pNext = pNode->children;
    while(1) 
    {
        if(strlen((char *)pNext->name) > 0 && XML_ELEMENT_NODE == pNext->type) 
        {
            if(!GetValue(pNext, objValue)) 
            {
                objValue.SetNull();
            }
            objRecord.Add((char *)pNext->name, objValue);
            bDidFind = true;
        }
        // 
        if(NULL == pNext->next) 
        {
            break;
        }else 
        {
            pNext = pNext->next;
        }
    }
    // 
    return(bDidFind);
}

/*
bool CXmlXPath::GetRecordList(const xmlNodePtr pNode, std::list<CRecord>& listRecord) const
{
}
*/
// --------------------------------------------------------------------------- /
bool CXmlXPath::GetValue(const char* xPathExpression, CValue& objValue) const 
{
    if(NULL == xPathExpression) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xPathExpression is NULL.", __FILE__, __LINE__);
    }
    // 
    if(NULL == m_pDoc) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): m_pDoc is NULL.", __FILE__, __LINE__);
    }

    // 
    xmlXPathContextPtr xpathCtx = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    // 
    try
    {
        // 
        xpathCtx = xmlXPathNewContext(m_pDoc);
        if(NULL == xpathCtx) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathNewContext() failed.", __FILE__, __LINE__);
        }
        xpathObj = xmlXPathEvalExpression((xmlChar*)xPathExpression, xpathCtx);
        if(NULL == xpathObj) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathEvalExpression() failed.", __FILE__, __LINE__);
        }
        // Get value: 
        if(NULL == xpathObj->nodesetval) 
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }else if(xpathObj->nodesetval->nodeNr <= 0)
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }
        // 
        bool bDidFind = false;
        xmlNodePtr pNode;
        for(int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) 
        {
            // 
            if(NULL != xpathObj->nodesetval->nodeTab[i]) 
            {
                if(XML_ELEMENT_NODE == xpathObj->nodesetval->nodeTab[i]->type)
                {
                    pNode = xpathObj->nodesetval->nodeTab[i];
        		    bDidFind = GetValue(pNode, objValue);
        		    break;
                }
            }
        }
        // 
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return(bDidFind);
    }catch(CException& eEx)
    {
        if(NULL != xpathCtx) 
        {
            xmlXPathFreeContext(xpathCtx);
        }
        if(NULL != xpathObj) 
        {
            xmlXPathFreeObject(xpathObj);
        }
        // 
        eEx.PushMessage("%s(%ld): CXmlXPath::GetValue(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CXmlXPath::GetRecord(const char* xPathExpression, CRecord& objRecord) const 
{
    if(NULL == xPathExpression) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xPathExpression is NULL.", __FILE__, __LINE__);
    }
    // 
    if(NULL == m_pDoc) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): m_pDoc is NULL.", __FILE__, __LINE__);
    }

    // 
    xmlXPathContextPtr xpathCtx = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    // 
    try
    {
        // 
        xpathCtx = xmlXPathNewContext(m_pDoc);
        if(NULL == xpathCtx) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathNewContext() failed.", __FILE__, __LINE__);
        }
        xpathObj = xmlXPathEvalExpression((xmlChar*)xPathExpression, xpathCtx);
        if(NULL == xpathObj) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathEvalExpression() failed.", __FILE__, __LINE__);
        }
        // Get value: 
        if(NULL == xpathObj->nodesetval) 
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }else if(xpathObj->nodesetval->nodeNr <= 0)
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }
        // 
        bool bDidFind = false;
        xmlNodePtr pNode;
        for(int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) 
        {
            // 
            if(NULL != xpathObj->nodesetval->nodeTab[i]) 
            {
                if(XML_ELEMENT_NODE == xpathObj->nodesetval->nodeTab[i]->type)
                {
                    pNode = xpathObj->nodesetval->nodeTab[i];
        		    bDidFind = GetRecord(pNode, objRecord);
        		    break;
                }
            }
        }
        // 
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return(bDidFind);
    }catch(CException& eEx)
    {
        if(NULL != xpathCtx) 
        {
            xmlXPathFreeContext(xpathCtx);
        }
        if(NULL != xpathObj) 
        {
            xmlXPathFreeObject(xpathObj);
        }
        // 
        eEx.PushMessage("%s(%ld): CXmlXPath::GetValue(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CXmlXPath::GetRecordList(const char* xPathExpression, std::list<CRecord>& listRecord) const
{
    if(NULL == xPathExpression) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xPathExpression is NULL.", __FILE__, __LINE__);
    }
    // 
    if(NULL == m_pDoc) 
    {
        throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): m_pDoc is NULL.", __FILE__, __LINE__);
    }

    // 
    xmlXPathContextPtr xpathCtx = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    // 
    try
    {
        // 
        xpathCtx = xmlXPathNewContext(m_pDoc);
        if(NULL == xpathCtx) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathNewContext() failed.", __FILE__, __LINE__);
        }
        xpathObj = xmlXPathEvalExpression((xmlChar*)xPathExpression, xpathCtx);
        if(NULL == xpathObj) 
        {
            throw CException(-1, "%s(%ld): CXmlXPath::GetValue(): xmlXPathEvalExpression() failed.", __FILE__, __LINE__);
        }
        // Get value: 
        if(NULL == xpathObj->nodesetval) 
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }else if(xpathObj->nodesetval->nodeNr <= 0)
        {
            xmlXPathFreeObject(xpathObj);
            xmlXPathFreeContext(xpathCtx);
            return(false);
        }
        // 
        bool bDidFind = false;
        xmlNodePtr pNode;
        CRecord objRecord;
        for(int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) 
        {
            // 
            if(NULL != xpathObj->nodesetval->nodeTab[i]) 
            {
                if(XML_ELEMENT_NODE == xpathObj->nodesetval->nodeTab[i]->type)
                {
                    pNode = xpathObj->nodesetval->nodeTab[i];
                    objRecord.Clear();
        		    bDidFind = GetRecord(pNode, objRecord);
        		    listRecord.push_back(objRecord);
                }
            }
        }
        // 
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return(bDidFind);
    }catch(CException& eEx)
    {
        if(NULL != xpathCtx) 
        {
            xmlXPathFreeContext(xpathCtx);
        }
        if(NULL != xpathObj) 
        {
            xmlXPathFreeObject(xpathObj);
        }
        // 
        eEx.PushMessage("%s(%ld): CXmlXPath::GetValue(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
