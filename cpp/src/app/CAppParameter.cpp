/* =========================================================================== /
* @category     Application tool
* @package      Buffer
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
#include "CAppParameter.h"
#include "CFileIni.h"

// --------------------------------------------------------------------------- /
// CAppParameter 1.0
// --------------------------------------------------------------------------- /
CAppParameter::CAppParameter()
{
}

CAppParameter::~CAppParameter()
{
    Clear();
}

// 
void CAppParameter::Clear()
{
    tMapParameterGroup::iterator i;
    for(i = m_mapGroup.begin(); i != m_mapGroup.end(); ++i)
    {
        if(NULL != i->second) delete i->second;
    }
}

// 
bool CAppParameter::FindGroup(std::string sGroup, tMapParameterGroup::iterator& iGroup)
{
    try
    {
        CValue objValue;
        objValue.SetValue(sGroup);
        // 
        iGroup = m_mapGroup.find(objValue.ToLower());
        if(m_mapGroup.end() == iGroup) return(false);
        else return(true);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CAppParameter::FindGroup(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CAppParameter::FindParameter(tMapParameterGroup::iterator& iGroup, std::string sName, tMapParameter::iterator& iParameter)
{
    try
    {
        if(m_mapGroup.end() == iGroup) return(false);
        // 
        if(NULL == iGroup->second) return(false);
        // 
        CValue objValue;
        objValue.SetValue(sName);
        iParameter = iGroup->second->find(objValue.ToLower());
        // 
        if(iParameter == iGroup->second->end()) return(false);
        else return(true);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CAppParameter::FindGroup(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
tMapParameterGroup::iterator CAppParameter::AddGroup(std::string sGroup)
{
    try
    {
        if(sGroup.empty()) CException(-1, "%s(%ld): CAppParameter::AddGroup(): Group is empty.", __FILE__, __LINE__);
        // 
        tMapParameter* pGroup;
        pGroup = new tMapParameter;
        if(NULL == pGroup) CException(-1, "%s(%ld): CAppParameter::AddGroup(): new failed.", __FILE__, __LINE__);
        // 
        CValue objValue;
        objValue.SetValue(sGroup);
        // 
        std::pair<tMapParameterGroup::iterator, bool> pairGroup;
        pairGroup = m_mapGroup.insert(make_pair(objValue.ToLower(), pGroup));
        if(!pairGroup.second)
        {
            // Duplicated group: 
            delete pGroup;
            CException(-1, "%s(%ld): CAppParameter::AddGroup(): Internal error, found duplicated group: %s.", __FILE__, __LINE__, sGroup.c_str());
        }
        return(pairGroup.first);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CAppParameter::AddGroup(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CAppParameter::AddParameter(tMapParameterGroup::iterator& iGroup, std::string sName, const CValue& objValue)
{
    try
    {
        if(sName.empty()) CException(-1, "%s(%ld): CAppParameter::AddParameter(): Name is empty.", __FILE__, __LINE__);
        // 
        if(NULL == iGroup->second) CException(-1, "%s(%ld): CAppParameter::AddParameter(): Internal error, parameter map is NULL pointer.", __FILE__, __LINE__);
        // 
        CValue objValueName;
        objValueName.SetValue(sName);
        // 
        std::pair<tMapParameter::iterator, bool> pairParameter;
        pairParameter = iGroup->second->insert(make_pair(objValueName.ToLower(), objValue));
        // 
        if(!pairParameter.second)
        {
            // Duplicated name: 
            CException(-1, "%s(%ld): CAppParameter::AddGroup(): Internal error, found duplicated parameter: %s.", __FILE__, __LINE__, sName.c_str());
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CAppParameter::AddParameter(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// --------------------------------------------------------------------------- /
// 
void CAppParameter::SetParameter(std::string sGroup, std::string sName, CValue objValue)
{
    try
    {
        // 
        if(sGroup.empty() || sName.empty())
        {
            CException(-1, "%s(%ld): CAppParameter::SetParameter(): Group or name is empty: [%s], [%s].", __FILE__, __LINE__, 
                sGroup.c_str(), sName.c_str());
        }
        // 
        tMapParameterGroup::iterator iGroup;
        tMapParameter::iterator iParameter;
        if(!FindGroup(sGroup, iGroup))
        {
            // Can not find group: Add a new group: 
            iGroup = AddGroup(sGroup);
            AddParameter(iGroup, sName, objValue);
        }else
        {
            if(!FindParameter(iGroup, sName, iParameter))
            {
                AddParameter(iGroup, sName, objValue);
            }else
            {
                iParameter->second = objValue;
            }
        }
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CAppParameter::SetParameter(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

bool CAppParameter::GetParameter(std::string sGroup, std::string sName, CValue& objValue)
{
    try
    {
        // 
        tMapParameterGroup::iterator iGroup;
        tMapParameter::iterator iParameter;
        if(!FindGroup(sGroup, iGroup)) return(false);
        if(!FindParameter(iGroup, sName, iParameter)) return(false);
        objValue = iParameter->second;
        return(true);
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CAppParameter::GetParameter(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CAppParameter::LoadParameterFromFile(std::string sFileName)
{
    try
    {
        // 
        std::string sSection, sName, sValue;
        CFileIni objFile(sFileName);
        // 
        while(objFile.FetchEntry(sSection, sName, sValue))
        {
            SetParameter(sSection, sName, sValue);
        }
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CAppParameter::LoadParameterFromFile(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
