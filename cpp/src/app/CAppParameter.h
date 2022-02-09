/* =========================================================================== /
* @category     Application
* @package      Application
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CAPPPARAMETER_H
#define _CAPPPARAMETER_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <map>
#include "CValue.h"

//------------------------------------------------------------------------------
// CAppParameter 1.0
//------------------------------------------------------------------------------
// Constants: 
typedef std::map<std::string, CValue>           tMapParameter;      // <name, value>
typedef std::map<std::string, tMapParameter*>   tMapParameterGroup; // <group_name, par_map>

// 
class CAppParameter
{
private: 
    tMapParameterGroup  m_mapGroup;
// 
    bool                            FindGroup(std::string sGroup, tMapParameterGroup::iterator& iGroup);
    bool                            FindParameter(tMapParameterGroup::iterator& iGroup, std::string sName, 
                                        tMapParameter::iterator& iParameter);
// 
    tMapParameterGroup::iterator    AddGroup(std::string sGroup);
    void                            AddParameter(tMapParameterGroup::iterator& iGroup, std::string sName, 
                                        const CValue& objValue);
public: 
// 
    CAppParameter();
    ~CAppParameter();
// 
    void                            Clear();
// 
    void                            SetParameter(std::string sGroup, std::string sName, CValue objValue);
    bool                            GetParameter(std::string sGroup, std::string sName, CValue& objValue);
// 
    // Load parameters from ini file: 
    void                            LoadParameterFromFile(std::string sFileName);
};

#endif
