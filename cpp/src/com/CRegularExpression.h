/* =========================================================================== /
* @category         Regular Expression
* @package          
* @author           Stephen(Tie) Hu <stephen@softroute.net>
* @copyright        2004-2007 The Softroute Co. Ltd.
* @license          
* @version          CVS: $Id: CDeviceBuffer.h,v 1.121 2006/08/29 21:25:17 stephen Exp $
* @link             
* @modification     (MM/DD/YY)  (Purpose)
    Stephen Hu      02/01/2007  First Generate
/============================================================================ */
#ifndef _CREGULAREXPRESSION_H
#define _CREGULAREXPRESSION_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <string>

// =========================================================================== /
// CRegularExpression 1.0 
// =========================================================================== /
/*
http://www.regular-expressions.info/reference.html
*/

class CRegularExpression
{
private: 
    int     m_nFlagCompile;
    int     m_nFlagExecute;
public: 
    CRegularExpression();
    ~CRegularExpression();
// 
    void    Clear();                // Clear all flags.
    // Compile flag: 
    void    SetExtendedSyntax();    // Use extended regular express syntax.
    void    SetCaseInsensitive();   // Set case insensitive.
    void    SetNoSubstring();       // Support for substring addressing of matches is not  required.   The  nmatch  and  pmatch  parameters  to
                                    //  regexec() are ignored if the pattern buffer supplied was compiled with this flag set.
    void    SetNewLineFlag();       // Match-any-character operators don't match a newline.
    // Execute flag: 
    void    SetNotBeginOfLine();    // The first character of the string pointed to by string is  not the beginning of the line. for '^'
    void    SetNotEndOfLine();      // The last character of the string pointed to by  string is not the end of the line. for '$'
// 
    bool    IsMatch(std::string sPattern, std::string sString);
};

#endif
