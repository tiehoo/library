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
#ifndef _CUTILITY_H
#define _CUTILITY_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
#include <stdlib.h>
#include <stdint.h>     // Network byte types

// =========================================================================== /
// CUtility 1.0 
// =========================================================================== /
class CUtility
{
public: 
    CUtility();
    ~CUtility();
// 
    // Return a random number between 0 and lMax - 1;
    static long GetRandom(long lMax = RAND_MAX);
    // 
    static void Xor(const uint8_t* pOper1, const uint8_t* pOper2, uint8_t* pResult, int nLen);
    /*
    // Evaluate a simple expression: 
    static double Evaluate(std::string sExp);
    */
};

#endif
