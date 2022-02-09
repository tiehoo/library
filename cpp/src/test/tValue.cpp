#include <iostream>
#include <stdio.h>
#include "CException.h"
#include "CValue.h"

using namespace std;

// make !=, >, etc. available. 
#include <utility>
using namespace std::rel_ops;

int main()
{
    try
    {
        // 
        CValue objValue;
        bool   bIsTrancated = false;
        // NULL: 
        cout << "String: " << objValue.ToString("NULL") << endl;
        cout << "Char: " << objValue.ToChar('?', &bIsTrancated) << endl;
        cout << "Char truncated: " << bIsTrancated << endl;
        cout << "Long: " << objValue.ToLong(-1024) << endl;
        cout << "Double: " << objValue.ToDouble(-0.1024) << endl;
        // String: 
        cout << endl;
        objValue.SetValue("+001.001");
        objValue.SetValue("+001.001", 5);
        cout << "String: " << objValue.ToString("NULL") << endl;
        cout << "Char: " << objValue.ToChar('?', &bIsTrancated) << endl;
        cout << "Char truncated: " << bIsTrancated << endl;
        cout << "Long: " << objValue.ToLong(-1024) << endl;
        cout << "Double: " << objValue.ToDouble(-0.1024) << endl;
        // Float: 0. 
        cout << endl;
        objValue.SetValue("+0.0");
        cout << "Zero Double: " << objValue.ToDouble(-0.1024) << endl;
        objValue.SetValue("-.0");
        cout << "Zero Double: " << objValue.ToDouble(-0.1024) << endl;
        objValue.SetValue(".0");
        cout << "Zero Double: " << objValue.ToDouble(-0.1024) << endl;
        // Char: 
        cout << endl;
        objValue.SetValue('A');
        cout << "String: " << objValue.ToString("NULL") << endl;
        cout << "Char: " << objValue.ToChar('?', &bIsTrancated) << endl;
        cout << "Char truncated: " << bIsTrancated << endl;
        cout << "Long: " << objValue.ToLong(-1024) << endl;
        cout << "Double: " << objValue.ToDouble(-0.1024) << endl;
        // Long: 
        cout << endl;
        objValue.SetValue((long)65);
        cout << "String: " << objValue.ToString("NULL") << endl;
        cout << "Char: " << objValue.ToChar('?', &bIsTrancated) << endl;
        cout << "Char truncated: " << bIsTrancated << endl;
        cout << "Long: " << objValue.ToLong(-1024) << endl;
        cout << "Double: " << objValue.ToDouble(-0.1024) << endl;
        // Double: 
        cout << endl;
        objValue.SetValue(65.1234);
        cout << "String: " << objValue.ToString("NULL") << endl;
        cout << "Char: " << objValue.ToChar('?', &bIsTrancated) << endl;
        cout << "Char truncated: " << bIsTrancated << endl;
        cout << "Long: " << objValue.ToLong(-1024) << endl;
        cout << "Double: " << objValue.ToDouble(-0.1024) << endl;
    // Operate: 
        CValue objValue1;
        objValue1 = objValue = "\t String. \t";
        printf("Value1: [%s]\n", objValue1.ToString().c_str());
        // 
        printf("Value: [%s]\n", objValue.ToString().c_str());
        printf("Lower: [%s]\n", objValue.ToLower().c_str());
        printf("Upper: [%s]\n", objValue.ToUpper().c_str());
        printf("RightTrim: [%s]\n", objValue.RightTrim().c_str());
        printf("LeftTrim: [%s]\n", objValue.LeftTrim().c_str());
        printf("AllTrim: [%s]\n", objValue.AllTrim().c_str());
    // Type: 
        cout << "Type: " << objValue.GetType() << endl;
        objValue.SetNull();
        cout << "Type: " << objValue.GetType() << endl;
    // Operator: 
    	// NULL: 
    	objValue.SetNull();
    	objValue1.SetNull();
    	bool bResult;
    	bResult = (objValue == objValue1);
    	cout << "NULL == NULL: " << bResult << endl; 
    	bResult = (objValue < objValue1);
    	cout << "NULL < NULL: " << bResult << endl;
    	// 
    	objValue = (long)1;
    	objValue1 = "1.01";
    	bResult = (objValue == objValue1);
    	cout << "1 == \"1.01\": " << bResult << endl; 
    	// 
    	objValue    = (double)1.101;
    	objValue1   = (double)1.100;
    	bResult = (objValue < objValue1);
    	cout << "1.101 < 1.100: " << bResult << endl; 
    	bResult = (objValue > objValue1);
    	cout << "1.101 > 1.100: " << bResult << endl; 
    	
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
