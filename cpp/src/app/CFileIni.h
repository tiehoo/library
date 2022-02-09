/* =========================================================================== /
* @category     Ini File
* @package      Utility
* @author       Stephen (Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: UAServer.cpp,v 1.0 Sean Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      12/02/2007  create it based on Stephen's CFile.h and CFile.cpp
/============================================================================ */
#ifndef _CFILEINI_H_
#define _CFILEINI_H_

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <fstream>
#include "CValue.h"
#include "CBuffer.h"
#include "CBufferAnalyzer.h"

// ---------------------------------------------------------------------------//
// CFileIni 1.0
// ---------------------------------------------------------------------------//
// 
const long  INI_MAX_BUFFER_SIZE         = 5*1024;
const int   INI_MAX_LINE_LEN            = 1*1024;
// 
const char  INI_LINE_CONTINUE           = '\\';
const char  INI_LINE_REMARK_CHAR        = '#';
const char  INI_NAME_VALUE_CHAR         = '=';
const char  INI_SEC_BEG_CHAR            = '[';
const char  INI_SEC_END_CHAR            = ']';
// 
const char  INI_LINE_TERMINATOR_SET[]   = "\r\n";

class CFileIni
{
private: 
    // Buffer: 
    CBuffer                         m_objFileBuffer;
    CBuffer                         m_objLineBuffer;
    // File buffer analyzer: 
    CBufferAnalyzer                 m_objFileAnalyzer;
    // Last section found: 
    CValue                          m_objSection;
    // Current line number: 
    long                            m_lCurrentLine;
// 
    // Return a logical line, that means merge all continued lines.
    // If return false means EOF.
    bool    ReadLine(CBuffer& objBuffer);
public: 
    CFileIni(std::string sFileName, long lBufferSize = INI_MAX_BUFFER_SIZE, int nMaxLineLen = INI_MAX_LINE_LEN);
    ~CFileIni();
// 
    // Read out entry one by one: 
    void    FetchStart();
    bool    FetchEntry(std::string& sSection, std::string& sName, std::string& sValue);
    // Find and read entry, rewind every time.
    bool    ReadEntry(std::string sSection, std::string sName, std::string& sValue);
};

#endif
