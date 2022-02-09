#include <utility>
#include "CException.h"
#include "CCurl.h"

int main()
{
    try
    {
        // 
        std::list< std::pair<std::string, std::string> > listInput;
        std::string sOutput;
        CCurl objCurl;
    // HTTP: 
        // Get: 
        sOutput.clear();
        objCurl.HttpGet("http://192.168.0.200/echo.php?get1=value1&get2=value2", listInput, sOutput);
        printf("[%s]\n\n", sOutput.c_str());
        
        // Post: 
        sOutput.clear();
        listInput.clear();
        listInput.push_back(std::pair<std::string, std::string>("post1", "value1"));
        listInput.push_back(std::pair<std::string, std::string>("post2", "value2"));
        objCurl.HttpPost("http://192.168.0.200/echo.php", listInput, sOutput);
        printf("[%s]\n\n", sOutput.c_str());
        
    // HTTPS: 
        // Get: 
        sOutput.clear();
        objCurl.SetOptionSSLVerifyPeer(0L);
        objCurl.SetOptionSSLVerifyHost(0L);
        objCurl.HttpGet("https://192.168.0.200/echo.php?sget1=value1&sget2=value2", listInput, sOutput);
        printf("[%s]\n\n", sOutput.c_str());
        // Post: 
        sOutput.clear();
        listInput.clear();
        listInput.push_back(std::pair<std::string, std::string>("spost1", "value1"));
        listInput.push_back(std::pair<std::string, std::string>("spost2", "value2"));
        objCurl.HttpPost("https://192.168.0.200/echo.php", listInput, sOutput);
        printf("[%s]\n\n", sOutput.c_str());
    }catch(CException& eEx) 
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
