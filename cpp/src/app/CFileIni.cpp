/* =========================================================================== /
* @category     Ini File
* @package      Utility
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: UAServer.cpp,v 1.0 Sean Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      12/02/2007  create it based on Stephen's CFile.h and CFile.cpp
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include "CFileIni.h"
#include "CException.h"

// ---------------------------------------------------------------------------//
// CFileIni 1.0
// ---------------------------------------------------------------------------//
CFileIni::CFileIni(std::string sFileName, long lBufferSize, int nMaxLineLen) : 
    m_objFileBuffer(lBufferSize), m_objLineBuffer(nMaxLineLen), m_objFileAnalyzer(m_objFileBuffer)
{
    std::ifstream fsFile;
    try
    {
        // Load file into file buffer: 
        // Open file: 
        fsFile.open(sFileName.c_str());
        // Read content into file: 
        if(!fsFile.fail() && !fsFile.bad())
        {
            fsFile.read((char *)m_objFileBuffer.GetBufferPointer(), m_objFileBuffer.GetSize() - 1);
            if(!fsFile.eof())
            {
                throw CException(-1, "%s(%ld): File buffer is too small, can not load file: [%s] into buffer, need more than %ld bytes.", __FILE__, __LINE__, 
                    sFileName.c_str(), m_objFileBuffer.GetSize());
            }
            m_objFileBuffer.SetHighWater(fsFile.gcount());
        }else
        {
            throw CException(-1, "%s(%ld): Failed when opening file: [%s].", __FILE__, __LINE__, sFileName.c_str());
        }
        // Close file: 
        fsFile.close();
        // 
        m_lCurrentLine = 0;
    }catch(CException& eEx) 
    {
        if(fsFile.is_open()) fsFile.close();
        // 
        eEx.PushMessage("%s(%ld): CFileIni::CFileIni(std::string): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CFileIni::~CFileIni()
{
}

// ---------------------------------------------------------------------------//
bool CFileIni::ReadLine(CBuffer& objBuffer)
{
    try
    {
        char cToken;
        bool bFound;
        long lLineStart, lLenSkip = 0;
        // 
        lLineStart  = m_objFileAnalyzer.GetOffset();
        // Find \r or \n: 
        bFound      = m_objFileAnalyzer.FindChar(INI_LINE_TERMINATOR_SET, cToken, lLenSkip);
        // 
        if(lLenSkip > objBuffer.GetSize()) 
        {
            throw CException(-1, "%s(%ld): Line buffer is too small, only %ld bytes, need at least: %ld bytes.", __FILE__, __LINE__, 
                objBuffer.GetSize(), lLenSkip);
        }
        // 
        if(bFound)
        {
            objBuffer.SetBuffer(m_objFileBuffer.GetBufferPointer(lLineStart), lLenSkip - 1);
            // If found \r see if next is \n: 
            if('\r' == cToken)
            {
                // Skip \n
                m_objFileAnalyzer.Skip("\n");
            }
        }else
        {
            objBuffer.SetBuffer(m_objFileBuffer.GetBufferPointer(lLineStart), lLenSkip);
        }
        // 
        m_lCurrentLine++;
        return(bFound);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CFileIni::ReadLine(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// ---------------------------------------------------------------------------//
void CFileIni::FetchStart()
{
    try
    {
        m_objFileAnalyzer.Rewind();
        m_lCurrentLine = 0;
    }catch(CException& eEx)
    {
        eEx.PushMessage("%s(%ld): CFileIni::FetchStart(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// Read out entry one by one: 
bool CFileIni::FetchEntry(std::string& sSection, std::string& sName, std::string& sValue)
{
    try
    {
        // 
        CBufferAnalyzer objLineAnalyzer(m_objLineBuffer);
        // 
        CValue objValue;
        
        // Read in a line: 
        char szToken[8], szTokenSecEnd[8], szTokenRemark[8], cTokenUsed;
        long lStart, lSkipLen;
        bool bFoundLine;
        // 
        szToken[0] = INI_LINE_REMARK_CHAR;
        szToken[1] = INI_SEC_BEG_CHAR;
        szToken[2] = INI_NAME_VALUE_CHAR;
        szToken[3] = '\0';
        // 
        szTokenSecEnd[0] = INI_SEC_END_CHAR;
        szTokenSecEnd[1] = '\0';
        // 
        szTokenRemark[0] = INI_LINE_REMARK_CHAR;
        szTokenRemark[1] = '\0';

        // 
        while(1)
        {
            bFoundLine = ReadLine(m_objLineBuffer);
            if(m_objLineBuffer.GetLength() > 0) 
            {
                // Get a line with content: Analyze it. 
                objLineAnalyzer.Rewind();
                if(objLineAnalyzer.Skip(" \t") < m_objLineBuffer.GetLength())
                {
                    // Found a line with content: Analyze it. 
                    lStart      = objLineAnalyzer.GetOffset();
                    lSkipLen    = 0;
                    // Find: '#', '[', '=': 
                    if(!objLineAnalyzer.FindChar(szToken, cTokenUsed, lSkipLen))
                    {
                        throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld], need at least '#', '[', '=': '%s'.", __FILE__, __LINE__, 
                            m_lCurrentLine, m_objLineBuffer.c_str());
                    }
                    // 
                    switch(cTokenUsed) 
                    {
                    case INI_LINE_REMARK_CHAR:  // '#'
                        // Remarked line: 
                        if(lSkipLen <= 1) continue;     // "# This is a remarked line."
                        else                            // "This is a wrong ini # line."
                        {
                            throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld], need at least '[', '=': '%s'.", __FILE__, __LINE__, 
                                m_lCurrentLine, m_objLineBuffer.c_str());
                        }
                        break;
                    case INI_SEC_BEG_CHAR:      // '['
                        // Found new section: 
                        if(lSkipLen <= 1)               // "[database]"
                        {
                            // Cut section: 
                            objLineAnalyzer.Skip(" \t");
                            lStart = objLineAnalyzer.GetOffset();
                            if(!objLineAnalyzer.FindChar(szTokenSecEnd, cTokenUsed, lSkipLen)) 
                            {
                                throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld], '[' without ']': '%s'.", __FILE__, __LINE__, 
                                    m_lCurrentLine, m_objLineBuffer.c_str());
                            }else
                            {
                                m_objSection.SetValue(m_objLineBuffer.GetBufferPointer(lStart), lSkipLen - 1);
                                m_objSection = m_objSection.RightTrim();
                                m_objSection = m_objSection.ToLower();
                            }
                            if(m_objSection.ToString().empty()) 
                            {
                                throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld]: '%s', no section found.", __FILE__, __LINE__, 
                                    m_lCurrentLine, m_objLineBuffer.c_str());
                            }
                        }else                           // "This is a wrong ini [ line."
                        {
                            throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld]: '%s'.", __FILE__, __LINE__, 
                                m_lCurrentLine, m_objLineBuffer.c_str());
                        }
                        break;
                    case INI_NAME_VALUE_CHAR:   // '='
                        // Set section: 
                        if(m_objSection.ToString().empty())
                        {
                            throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): No current section found, when dealing with line[%ld]: '%s'.", __FILE__, __LINE__, 
                                m_lCurrentLine, m_objLineBuffer.c_str());
                        }
                        sSection    = m_objSection.ToString();
                        
                        // Set name: 
                        objValue.SetValue(m_objLineBuffer.GetBufferPointer(lStart), lSkipLen - 1);
                        objValue = objValue.RightTrim();
                        objValue = objValue.ToLower();
                        if(objValue.ToString().empty())
                        {
                            throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Invalid line[%ld], can not find variable name: '%s'.", __FILE__, __LINE__, 
                                m_lCurrentLine, m_objLineBuffer.c_str());
                        }
                        sName = objValue.ToString();
                        
                        // Set value: 
                        objLineAnalyzer.Skip(" \t");
                        lStart = objLineAnalyzer.GetOffset();
                        // Try to find: '#'
                        if(objLineAnalyzer.FindChar(szTokenRemark, cTokenUsed, lSkipLen))
                        {
                            objValue.SetValue(m_objLineBuffer.GetBufferPointer(lStart), lSkipLen - 1);
                        }else
                        {
                            objValue.SetValue(m_objLineBuffer.GetBufferPointer(lStart), lSkipLen);
                        }
                        objValue    = objValue.RightTrim();
                        sValue      = objValue.ToString();
                        // 
                        return(true);
                    default: 
                        throw CException(-1, "%s(%ld): CFileIni::FetchEntry(): Internal error, found unknown token: %c.", __FILE__, __LINE__, cTokenUsed);
                    }
                }else
                {
                    // Get a line with all space chars: Do nothing. 
                }
            }else
            {
                // Get a blank line: Do nothing.
            }
            
            // If EOF, break: 
            if(!bFoundLine) break;
        }
        return(false);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CFileIni::FetchEntry(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// ---------------------------------------------------------------------------//
// Find and read entry, rewind every time.
bool CFileIni::ReadEntry(std::string sSection, std::string sName, std::string& sValue)
{
    try
    {
        // Standardlize parameters: 
        CValue objSection, objName;
        // 
        objSection  = sSection;
        objSection  = objSection.AllTrim();
        objSection  = objSection.ToLower();
        // 
        objName     = sName;
        objName     = objName.AllTrim();
        objName     = objName.ToLower();

        // Read in every entry: 
        std::string sSectionFile, sNameFile, sValueFile;
        // 
        FetchStart();
        while(FetchEntry(sSectionFile, sNameFile, sValueFile)) 
        {
            if(objSection.ToString() == sSectionFile && objName.ToString() == sNameFile)
            {
                // Found entry, read value: 
                sValue = sValueFile;
                return(true);
            }
        }
        return(false);
    }catch(CException& eEx) 
    {
        eEx.PushMessage("%s(%ld): CFileIni::ReadEntry(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}
