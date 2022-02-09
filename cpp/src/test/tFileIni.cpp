#include <iostream>
#include "CException.h"
#include "CFileIni.h"
//  
using namespace std;

// 
int main()
{
    try
    {
        // 
        /* File buffer too small: 
        CFileIni objFileIni("./tFileIni.ini", 10, 5);
        */
        /* Line buffer too small: 
        CFileIni objFileIni("./tFileIni.ini", 5*1024, 5);
        */
        CFileIni objFileIni("./tFileIni.ini");
        // Fetch each entry: 
        std::string sSection, sName, sValue;
        objFileIni.FetchStart();
        while(objFileIni.FetchEntry(sSection, sName, sValue))
        {
            printf("[%s] [%s=%s].\n", sSection.c_str(), sName.c_str(), sValue.c_str());
        }
        
        // Read one: 
        bool bFound;
        bFound = objFileIni.ReadEntry("datAbase", "hOst", sValue);
        if(bFound) printf("Entry: [%s].\n", sValue.c_str());
        else printf("Can not find entry.\n");
        // 
        return(0);
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
