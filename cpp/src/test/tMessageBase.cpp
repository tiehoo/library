#include <stdio.h>
#include "CException.h"
#include "CMessageBase.h"

int main()
{
    try
    {
        CMessageBase objMessageBase;
        objMessageBase.SetInvalid("%s, %d.", "Ok", 21);
        objMessageBase.SetInvalid("%s, %d.", "Failed", 22);
        objMessageBase.PrintStructure();
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
