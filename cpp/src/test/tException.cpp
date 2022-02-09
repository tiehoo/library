// 
#include <iostream>
// 
#include "CException.h"

int main()
{
    try
    {
        try
        {
            throw CException(-1304, "%s(%ld): This a test exception: %s, %c, %%, %d, %ld.", __FILE__, __LINE__, "test1", 'c', 65535, 65539);
        }catch(CException &eEx)
        {
            eEx.PushMessage("%s(%ld): Exception caught here.", __FILE__, __LINE__);
            throw eEx;
        }
    }catch(CException &eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
