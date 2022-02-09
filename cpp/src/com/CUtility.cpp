/* =========================================================================== /
* @category         Random
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
#include <stack>
// 
#include "CException.h"
#include "CUtility.h"

// =========================================================================== /
// CUtility 1.0 
// =========================================================================== /
CUtility::CUtility()
{
}

CUtility::~CUtility()
{
}

//----------------------------------------------------------------------------//
long CUtility::GetRandom(long lMax) 
{
    return((long)((double)rand() / ((double)RAND_MAX + 1) * lMax));
}

//----------------------------------------------------------------------------//
void CUtility::Xor(const uint8_t* pOper1, const uint8_t* pOper2, uint8_t* pResult, int nLen) 
{
    if(NULL == pResult || NULL == pOper1 || NULL == pOper2)
    {
        throw CException(-1, "%s(%ld): CUtility::Xor(): NULL pointer.", __FILE__, __LINE__);
    }
    for(int i = 0; i < nLen; i++)
    {
        pResult[i] = pOper1[i] ^ pOper2[i];
    }
}

//----------------------------------------------------------------------------//
/*
double CUtility::Evaluate(std::string sExp) 
{
    // 
    std::stack<char>    stackOperator;
    std::stack<double>  stackOperand;
    // 
    size_t nBeg, nEnd;
    std::string sOperand;
    // 
    stackOperator.push('@');
    // 
    double fValue, fLeft, fRight;
    char   cOperator;
    // Skip space: 
    while(1) 
    {
        nBeg = sExp.find_first_of("1234567890/+-*");
        if(nBeg == std::string::npos) 
        {
            break;
        }
        if(sExp[nBeg] == '+' || sExp[nBeg] == '-' || sExp[nBeg] == '*' || sExp[nBeg] == '/') 
        {
            // Found a operator: Compare to the top of the stack: 
            if(comp_operator(sExp[nBeg], stackOperator.top()) > 0) 
            {
                // Current is higher: *, /, calculate
                cOperator   = stackOperator.pop();
                fRight      = stackOperand.pop();
                fLeft       = stackOperand.pop();
                if(cOperator = '+') 
                {
                    fValue = fLeft + fRight;
                }else if(cOperator = '-') 
                {
                    fValue = fLeft - fRight;
                }
                stackOperand.push(fValue);
            }else 
            {
                stackOperator.push(sExp[nBeg]);
            }
        }else 
        {
            // Begin of and operand: Find the end. 
            nEnd = sExp.find_first_no_of("1234567890");
            if(nEnd = std::string::npos) 
            {
                sOperand = sExp.substr(nBeg, sExp.length() - nBeg);
            }else 
            {
                sOperand = sExp.substr(nBeg, nEnd - nBeg);
            }
            stackOperand.push(strtod(sOperand.c_str()));
        }
    }
    // Calculate: 
    
}
*/
