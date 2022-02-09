#include <stdio.h>
#include "CException.h"
#include "CRecord.h"

int main()
{
    try
    {
        // 
        CRecord objRecord;
        CValue objValue;
        objValue = "value1";
        objRecord.Add("Name1", objValue);
        objValue = "value2";
        objRecord.Add("nAme2", objValue);
        // 
        std::string sValue;
        if(objRecord.GetValue("name2", objValue)) 
        {
            printf("Value: %s\n", objValue.ToString().c_str());
        }
        // 
        if(objRecord.GetValue("name3", objValue)) 
        {
            printf("Value: %s\n", objValue.ToString().c_str());
        }
        printf("count: %d\n", objRecord.GetSize());
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
