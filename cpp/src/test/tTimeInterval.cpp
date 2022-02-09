// 
#include <stdio.h>
#include <iostream>
// 
#include "CException.h"
#include "CTimeInterval.h"

int main()
{
    try
    {
        //  Initialization: 
        CTimeInterval objInterval1(1, 1);
        printf("Interval1: %ld, %ld\n", objInterval1.GetSecond(), objInterval1.GetMicroSecond());
        CTimeInterval objInterval2(objInterval1);
        printf("Interval2: %ld, %ld\n", objInterval2.GetSecond(), objInterval2.GetMicroSecond());
        // Set values: 
        objInterval1.SetTimeInterval(0, 1000001);
        printf("Interval1: %ld, %ld\n", objInterval1.GetSecond(), objInterval1.GetMicroSecond());
        objInterval1.SetTimeInterval(-1, 1000001);
        printf("Interval1: %ld, %ld\n", objInterval1.GetSecond(), objInterval1.GetMicroSecond());
        objInterval1.SetTimeInterval(-1, -1000001);
        printf("Interval1: %ld, %ld\n", objInterval1.GetSecond(), objInterval1.GetMicroSecond());
        // Operators: 
        objInterval2 = objInterval1;
        printf("Interval2: %ld, %ld\n", objInterval2.GetSecond(), objInterval2.GetMicroSecond());
        // 
        CTimeInterval objInterval3;
        objInterval1.SetTimeInterval(1, 1);
        objInterval2.SetTimeInterval(0, 1000000);
        // 
        objInterval3 = objInterval1 + objInterval2;
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
        objInterval3 = objInterval1 + 1;
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
        objInterval3 = objInterval2 - objInterval1;
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
        objInterval3 = objInterval1 - 1000001;
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
        objInterval3 = objInterval1 * 2;
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
        objInterval3 = objInterval1;
        if(objInterval1 == objInterval3) printf("Interval1 == Interval3\n");
        else printf("Interval1 != Interval3\n");
        // 
        objInterval3 = objInterval3 - 1;
        if(objInterval3 < objInterval1) printf("Interval3 < Interval1\n");
        else printf("Interval3 !< Interval1\n");
        // 
        objInterval1.SetTimeInterval(-1, 1);
        printf("Interval1's sign: %d\n", objInterval1.Sign());
        objInterval3 = objInterval1.Abs();
        printf("Interval3: %ld, %ld\n", objInterval3.GetSecond(), objInterval3.GetMicroSecond());
        // 
    }catch(CException &eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
