/* =========================================================================== /
* @category         RegExp
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
#include <regex.h>
// 
#include "CException.h"
#include "CRegularExpression.h"

// =========================================================================== /
// CRegularExpression 1.0 
// =========================================================================== /
CRegularExpression::CRegularExpression()
{
    Clear();
    SetExtendedSyntax();
    SetNoSubstring();
}

CRegularExpression::~CRegularExpression()
{
}

//----------------------------------------------------------------------------//
void CRegularExpression::Clear()
{
    m_nFlagCompile = 0;
    m_nFlagExecute = 0;
}

// Compile flag: 
// Use extended regular express syntax.
void CRegularExpression::SetExtendedSyntax()
{
    m_nFlagCompile = m_nFlagCompile | REG_EXTENDED;
}
// Set case insensitive.
void CRegularExpression::SetCaseInsensitive()
{
    m_nFlagCompile = m_nFlagCompile | REG_ICASE;
}
// Support for substring addressing of matches is not  required.   The  nmatch  and  pmatch  parameters  to
//  regexec() are ignored if the pattern buffer supplied was compiled with this flag set.
void CRegularExpression::SetNoSubstring()
{
    m_nFlagCompile = m_nFlagCompile | REG_NOSUB;
}
// Match-any-character operators don't match a newline.
void CRegularExpression::SetNewLineFlag()
{
    m_nFlagCompile = m_nFlagCompile | REG_NEWLINE;
}

// Execute flag: 
// The first character of the string pointed to by string is  not the beginning of the line. for '^'
void CRegularExpression::SetNotBeginOfLine()
{
    m_nFlagExecute = m_nFlagExecute | REG_NOTBOL;
}
// The last character of the string pointed to by  string is not the end of the line. for '$'
void CRegularExpression::SetNotEndOfLine()
{
    m_nFlagExecute = m_nFlagExecute | REG_NOTEOL;
}

//----------------------------------------------------------------------------//
/*
regcomp(): 
     REG_EXTENDED
           Use Extended Regular Expressions.

     REG_ICASE
           Ignore case in match.

     REG_NOSUB
           Report only success/fail in regexec().

     REG_NEWLINE
           Change  the  handling  of   NEWLINE   characters,   as
           described in the text.
regexec(): 
     REG_NOTBOL
           The first character of the string pointed to by string
           is  not the beginning of the line. Therefore, the cir-
           cumflex character (^), when taken as a special charac-
           ter, will not match the beginning of string.

     REG_NOTEOL
           The last character of the string pointed to by  string
           is not the end of the line. Therefore, the dollar sign
           ($), when taken as a special character, will not match
           the end of string.
*/
bool CRegularExpression::IsMatch(std::string sPattern, std::string sString)
{
    int nError;
    regex_t stExp;

    if((nError = regcomp(&stExp, sPattern.c_str(), m_nFlagCompile)) != 0)
    {
        char szError[128];
        if(0 == regerror(nError, &stExp, szError, sizeof(szError))) szError[0] = '\0';
        throw CException(-1, "%s(%ld): CRegularExpression::IsMatch(std::string, std::string): reccomp() failed: Expression: [%s], code: [%d], error: [%s].", __FILE__, __LINE__, sPattern.c_str(), nError, szError);
    }
    // 
    nError = regexec(&stExp, sString.c_str(), (size_t) 0, NULL, m_nFlagExecute);
    regfree(&stExp);
    
    if(0 == nError) return(true);
    else if(REG_NOMATCH == nError) return(false);
    else
    {
        char szError[128];
        if(0 == regerror(nError, &stExp, szError, sizeof(szError))) szError[0] = '\0';
        throw CException(-1, "%s(%ld): CRegularExpression::IsMatch(): regexec() failed: Expression: [%s], string: [%s], code: [%d], error: [%s].", __FILE__, __LINE__, sPattern.c_str(), 
            sString.c_str(), nError, szError);
    }
}
