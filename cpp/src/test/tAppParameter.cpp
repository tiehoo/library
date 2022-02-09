#include <stdio.h>
#include <iostream>
#include "CException.h"
#include "CAppParameter.h"
using namespace std;

int main()
{
    try
    {
    // CAppParameter: 
        CAppParameter objPar;
        CValue objValue, objValue1;

        objValue.SetValue("1972");
        // 
        objPar.SetParameter("datAbase", "pOrt", objValue);
        if(!objPar.GetParameter("database", "port", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[datebase]:port: %ld.\n", objValue1.ToLong());
        }
        // 
        objValue.SetValue("209.47.1.1");
        objPar.SetParameter("database", "ip", objValue);
        if(!objPar.GetParameter("database", "ip", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[datebase]:ip: %s.\n", objValue1.ToString().c_str());
        }
        // Add new group: 
        objValue.SetValue("209.47.1.2");
        objPar.SetParameter("b2bua", "ip", objValue);
        if(!objPar.GetParameter("b2buA", "ip", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[b2bua]:ip: %s.\n", objValue1.ToString().c_str());
        }
        // Update middle one: 
        objValue.SetValue("209.47.1.102");
        objPar.SetParameter("database", "ip", objValue);
        
        // All read out: 
        if(!objPar.GetParameter("database", "port", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[datebase]:port: %ld.\n", objValue1.ToLong());
        }
        if(!objPar.GetParameter("database", "ip", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[datebase]:ip: %s.\n", objValue1.ToString().c_str());
        }
        if(!objPar.GetParameter("b2buA", "ip", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[b2bua]:ip: %s.\n", objValue1.ToString().c_str());
        }
        
        // Load from file: 
        objPar.LoadParameterFromFile("./tFileIni.ini");
        
        // 
        if(!objPar.GetParameter("dataBase", "hOst", objValue1))
        {
            printf("Can not find parameter.\n");
        }else
        {
            printf("[database]:host: %s.\n", objValue1.ToString().c_str());
        }
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
