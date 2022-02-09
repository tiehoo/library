#include <iostream>
#include "CException.h"
#include "CRegularExpression.h"

using namespace std;
int main()
{
    try
    {
        // 
        CRegularExpression objRegExp;
        cout << "IsMatch: " << objRegExp.IsMatch("[0-9]A[0-9]", "9A9") << endl;
        cout << "IsMatch: " << objRegExp.IsMatch("[0-9]A[0-9]", "9AB9") << endl;
        objRegExp.SetExtendedSyntax();
        // IP address: 
        cout << "IsMatch: " << objRegExp.IsMatch("^[1-9][0-9]{0,2}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}", "029.47.41.25") << endl;
        // A zero number: 
        cout << "IsMatch: " << objRegExp.IsMatch("^[+-]?0*\\.?0{1,}$", "0") << endl;
        
        cout << "IsMatch: " << objRegExp.IsMatch("[0-0]", "") << endl;
    }catch(CException& eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
