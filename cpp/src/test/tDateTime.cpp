#include <iostream>
using namespace std;
// 
#include <utility>
using namespace std::rel_ops;
#include "CException.h"
#include "CDateTime.h"

int main()
{
    try
    {
    // CTimeInterval: 
    		cout << "Time Interval: " << endl;
        CTimeInterval   objTimeInterval;
        objTimeInterval.SetTimeInterval(1, 1000001);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        // 
        objTimeInterval.SetTimeInterval(0, -1000001);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        // 
        objTimeInterval.SetTimeInterval(1, -1000001);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        // +: (+) + (+) = (+)
        CTimeInterval objTimeInterval1, objTimeInterval2, objTimeInterval3;
        objTimeInterval1.SetTimeInterval(1, 999999);
        objTimeInterval2.SetTimeInterval(1, 1);
        objTimeInterval3 = objTimeInterval1 + objTimeInterval2;
        cout << "Time interval: (" << objTimeInterval3.Sign() << ") " << objTimeInterval3.GetSecond() << ":" << objTimeInterval3.GetMicroSecond() << endl;
        //    (-) + (+) = (+)
        objTimeInterval1.SetTimeInterval(-1, -999999);
        objTimeInterval2.SetTimeInterval(2, 1);
        objTimeInterval3 = objTimeInterval1 + objTimeInterval2;
        cout << "Time interval: (" << objTimeInterval3.Sign() << ") " << objTimeInterval3.GetSecond() << ":" << objTimeInterval3.GetMicroSecond() << endl;
        //    (-) + (+) = (-)
        objTimeInterval1.SetTimeInterval(-3, 999998);
        objTimeInterval2.SetTimeInterval(1, 1);
        objTimeInterval3 = objTimeInterval1 + objTimeInterval2;
        cout << "Time interval: (" << objTimeInterval3.Sign() << ") " << objTimeInterval3.GetSecond() << ":" << objTimeInterval3.GetMicroSecond() << endl;

        // -: (+) - (+) = (+)
        objTimeInterval1.SetTimeInterval(1, 999999);
        objTimeInterval2.SetTimeInterval(1, 1);
        objTimeInterval3 = objTimeInterval1 - objTimeInterval2;
        cout << "Time interval: (" << objTimeInterval3.Sign() << ") " << objTimeInterval3.GetSecond() << ":" << objTimeInterval3.GetMicroSecond() << endl;
        //    (+) - (-) = (+)
        objTimeInterval1.SetTimeInterval(1, 999999);
        objTimeInterval2.SetTimeInterval(-2, 1);
        objTimeInterval3 = objTimeInterval1 - objTimeInterval2;
        cout << "Time interval: (" << objTimeInterval3.Sign() << ") " << objTimeInterval3.GetSecond() << ":" << objTimeInterval3.GetMicroSecond() << endl;

        // *
        cout << "Testing * operator..." << endl;
        objTimeInterval.Clear();
        objTimeInterval.SetMicroSecond(1000011);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        // *
        objTimeInterval = objTimeInterval * 2.5;
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        
        // + long
        cout << "Testing + long operator..." << endl;
        objTimeInterval.Clear();
        objTimeInterval.SetMicroSecond(1000011);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        objTimeInterval = objTimeInterval + (1000000 - 1);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;

        // ABS: 
        cout << "Testing ABS..." << endl;
        CTimeInterval objIntervalAbs;
        objTimeInterval.SetTimeInterval(-1, 999999);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        objIntervalAbs = objTimeInterval.Abs();
        cout << "Time interval ABS: (" << objIntervalAbs.Sign() << ") " << objIntervalAbs.GetSecond() << ":" << objIntervalAbs.GetMicroSecond() << endl;
        objTimeInterval.SetTimeInterval(-1, -999999);
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        objIntervalAbs = objTimeInterval.Abs();
        cout << "Time interval ABS: (" << objIntervalAbs.Sign() << ") " << objIntervalAbs.GetSecond() << ":" << objIntervalAbs.GetMicroSecond() << endl;
        
        // Get timeval: 
        struct timeval stTimeVal;
        objTimeInterval.GetTimeVal(stTimeVal);
        cout << "Time value: " << stTimeVal.tv_sec << ", " << stTimeVal.tv_usec << endl;

    // CDateTime: 
    		cout << endl << "Date and Time: " << endl;
        CDateTime objDateTime, objDateTime1, objDateTime2;
        for(int i = 0; i < 10; ++i)
        {
            objDateTime.SetToNow();
            cout << "Time: " << objDateTime.GetSecond() << ":" << objDateTime.GetMicroSecond() << endl;
        }
        // 
        objDateTime.SetTimeStamp(123, 456);
        cout << "Time stamp: " << objDateTime.GetTimeStamp() << endl;
        
        // operators: 
        cout << endl << "Operators: " << endl;
        // -: 
        objDateTime.SetToNow();
        usleep(500*1000);
        objDateTime1.SetToNow();
        objTimeInterval = objDateTime1 - objDateTime;
        cout << "Time: " << objDateTime.GetSecond() << ":" << objDateTime.GetMicroSecond() << endl;
        cout << "Time1: " << objDateTime1.GetSecond() << ":" << objDateTime1.GetMicroSecond() << endl;
        cout << "Time interval: (" << objTimeInterval.Sign() << ") " << objTimeInterval.GetSecond() << ":" << objTimeInterval.GetMicroSecond() << endl;
        // <: 
        bool bT;
        bT = objDateTime < objDateTime1;
        cout << "Time < Time1? " << bT << endl;
        // >: 
        bT = objDateTime > objDateTime1;
        cout << "Time > Time1? " << bT << endl;
        // -: 
        objDateTime2 = objDateTime1 - objTimeInterval;
        cout << "Time2: " << objDateTime2.GetSecond() << ":" << objDateTime2.GetMicroSecond() << endl;
        // ==: 
        bT = objDateTime2 == objDateTime;
        cout << "Time == Time2? " << bT << endl;
        // +: 
        objDateTime2 = objDateTime + objTimeInterval;
        cout << "Time2: " << objDateTime2.GetSecond() << ":" << objDateTime2.GetMicroSecond() << endl;
        // 
        objDateTime2 = objDateTime - 1000001;
        cout << "Time2: " << objDateTime2.GetSecond() << ":" << objDateTime2.GetMicroSecond() << endl;
        
        // Set time: 
        cout << endl;
        objDateTime.SetDateTime("19700101000000", "YYYYMMDDHH24MISS");
        cout << "Date Time: " << objDateTime.GetSecond() << ":" << objDateTime.GetMicroSecond() << endl;
        // 
        objDateTime.SetDateTime("1970-01-01 01:01:01", "YYYY-MM-DD HH24:MI:SS");
        cout << "Date Time: " << objDateTime.GetSecond() << ":" << objDateTime.GetMicroSecond() << endl;
        
        // ToString: 
        cout << endl;
        cout << "String: " << objDateTime.ToString("YYYY-MM-DD HH24:MI:SS") << endl;
        objDateTime.SetToNow();
        cout << "String: " << objDateTime.ToString("YYYY-MM-DD HH24:MI:SS") << endl;
    }catch(CException& eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        return(-1);
    }
}
