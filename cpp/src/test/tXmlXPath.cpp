#include "CException.h"
#include "CXmlXPath.h"

int main()
{
    try
    {
        // Test record: 
        char szXml[] = "<api><status><code>200</code><message>OK</message></status><data>\
        <ip>74.13.233.143</ip>\
        <secret>1w3S5x7O9k2N4t6G8v0</secret>\
        <text>I love you</text>\
        <txt></txt>\
        </data></api>";
        // Test list: 
        /*
        char szXml[] = "<api><status><code>200</code><message>OK</message></status><data><client><ip>74.13.233.143</ip><secret>1w3S5x7O9k2N4t6G8v0</secret></client><client><ip>69.13.233.144</ip><secret>1w3S5x7O9k2N4t6G8v0</secret></client><client><ip>79.13.233.145</ip><secret>1w3S5x7O9k2N4t6G8v0</secret></client></data></api>";
        */
        printf("Input: %s\n", szXml);
        CXmlXPath objXmlXPath(szXml);
        // 
        CValue objValue;
        /*
        if(objXmlXPath.GetValue("/api/status/message", objValue)) 
        {
            printf("message: %s\n", objValue.ToString().c_str());
        }else 
        {
            printf("Not found.\n");
        }
        if(objXmlXPath.GetValue("/api/status/code", objValue)) 
        {
            printf("code: %s\n", objValue.ToString().c_str());
        }else 
        {
            printf("Not found.\n");
        }
        if(objXmlXPath.GetValue("/api/data/client", objValue)) 
        {
            printf("client: %s\n", objValue.ToString().c_str());
        }else 
        {
            printf("Not found.\n");
        }
        printf("\n\n");
        */
        // 
        CRecord objRecord;
        if(objXmlXPath.GetRecord("/api/data", objRecord)) 
        {
            printf("client fields: %d\n", objRecord.GetSize());
            objRecord.Print();
        }else 
        {
            printf("Not found.\n");
        }
        printf("\n\n");

        // 
        /*
        std::list<CRecord> listRecord;
        if(objXmlXPath.GetRecordList("/api/data/client", listRecord)) 
        {
            printf("client records: %d\n", listRecord.size());
            std::list<CRecord>::const_iterator it;
            for(it = listRecord.begin(); it != listRecord.end(); ++it) 
            {
                it->Print();
            }
        }else 
        {
            printf("Not found.\n");
        }
        */
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
