#include <string.h>
#include <stdio.h>
#include "CException.h"
#include "CBuffer.h"
#include "CNameValuePair.h"

int main()
{
    try
    {
        char szBuffer[64];
        CNameValuePair objNameValuePair(64);
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "     ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, " \tname ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, " name  =");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, " name  =  ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        strcpy(szBuffer, "  name   =   value");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        strcpy(szBuffer, "  name   =   value   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();

        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "  name   =   a\"value\"   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=", "\"'");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "  name   =   \"value'   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=", "\"'");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "  name   =   \"value\"ab   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=", "\"'");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "  name   =   \"value   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=", "\"'");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();
        // 
        objNameValuePair.Clear();
        strcpy(szBuffer, "  name   =   \" value \"   ");
        objNameValuePair.SetBuffer(szBuffer);
        objNameValuePair.Parse("=", "\"'");
        printf("\n");
        objNameValuePair.Print();
        printf("\n");
        objNameValuePair.PrintStructure();

        // 
        std::string sName, sValue;
        char cSeparator, cQuotation;
        if(objNameValuePair.GetName(sName)) 
        {
            printf("Name: [%s].\n", sName.c_str());
        }
        if(objNameValuePair.GetSeparator(cSeparator)) 
        {
            printf("Separator: [%c].\n", cSeparator);
        }
        if(objNameValuePair.GetValue(sValue)) 
        {
            printf("Value: [%s].\n", sValue.c_str());
        }
        if(objNameValuePair.GetQuotation(cQuotation)) 
        {
            printf("Separator: [%c].\n", cQuotation);
        }
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
