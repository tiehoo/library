#include <stdlib.h>
#include "CException.h"
#include "CMysql.h"
#include <iostream>
using namespace std;

/*
drop table test.test;
create table test.test(f_int INT(10), f_float DECIMAL(12, 6), f_string VARCHAR(32), f_char CHAR(1) default '?', 
    f_datetime DATETIME, f_null CHAR(1));
alter table test.test ENGINE = INNODB;
alter table test.test ADD(PRIMARY KEY(f_int));
*/
int main()
{
    // 
/*
    while(1)
    {
        CConnectionMysql    objCon1;
    }
exit(0);
*/
    int nRet;
    CConnectionMysql    objCon;
    try
    {
        // Thread safe: Before
        CBeforeThreadMysql objBeforeThread;
        cout << "Thread safe: " << objBeforeThread.IsThreadSafe() << endl;
        // Thread safe: In
        CInThreadMysql objInThread;
        objInThread.BeforePthreadExit();
        
        // 
        objCon.SetAutoReconnect(true);
        // 
        objCon.Connect("74.13.233.137", "admin", "routesoft05bill", "test");
        cout << "Database connected." << endl;
        // Connect more than once. Will disconnect and reconnect.
        objCon.Connect("74.13.233.137", "ser", "routesoft05bill", "bill");
        cout << "Database connected again." << endl;
        // 
        //sleep(10);
        // Kill connection here: 
        objCon.Connect();
        cout << "Database reconnected." << endl;
        // 
        //sleep(10);
        // Kill connection: 
        objCon.IsAlive();
        cout << "Reconnected: " << endl;
        //sleep(10);

        // Select databse: 
        objCon.SelectDatabase("test");
        cout << "Database switched to test." << endl;

        // Insert: 
        try
        {
            nRet = objCon.Execute("INSERT INTO test VALUE(10, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
            cout << nRet << " line(s) inserted." << endl;
        }catch(CException &eEx)
        {
            std::cout << eEx.GetMessageStack() << std::endl;
            //eEx.Print();
        }
//        sleep(10);
        // I will kill the connection: ERROR 2013(HY000): Lost connection to MySQL server during query
        nRet = objCon.Execute("INSERT INTO test VALUE(11, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
        cout << nRet << " line(s) inserted." << endl;
        
        // Alive: 
//        sleep(10);
        // I will kill the connection: 
        if(objCon.IsAlive()) cout << "Database is still alive." << endl;
        else cout << "Database is not alive." << endl;
        
        // Transaction: 
        //  rollback: 
        objCon.StartTransaction();
        nRet = objCon.Execute("INSERT INTO test VALUE(12, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
        cout << nRet << " line(s) inserted." << endl;
        objCon.Rollback();
        cout << "Rolled back." << endl;
        
        //  rollback: 
        objCon.StartTransaction();
        nRet = objCon.Execute("INSERT INTO test VALUE(13, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
        cout << nRet << " line(s) inserted." << endl;
        objCon.Commit();
        cout << "Commited." << endl;

        // Here you will found only 3 line deleted.
        nRet = objCon.Execute("DELETE FROM test");
        cout << nRet << " line(s) deleted." << endl;

        // Query: 
        // Row: 
        CRowMysql objRow;
        
        // Query: 
        CQueryMysql objQuery(objCon);
        // 
        objCon.StartTransaction();
        nRet = objCon.Execute("INSERT INTO test VALUE(14, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
        nRet = objCon.Execute("INSERT INTO test VALUE(15, 10.12345678, 'ABCDEFGHIJK', 'C', now(), NULL)");
        cout << nRet << " line(s) inserted." << endl;
        // 
        nRet = objQuery.Query("SELECT * FROM test");
        cout << nRet << " rows selected, that's right." << endl;
        nRet = objQuery.StoreResult();
        cout << nRet << " rows retrieved: " << endl;
        // Title: 
        for(int i = 0; i < objQuery.GetFieldCount(); i++)
        {
            cout << objQuery.GetFieldName(i) << "\t";
        }
        cout << endl;
        // 
        bool bIsTruncated;
        // Row: 
        while(objQuery.Fetch(objRow))
        {
            // Field: 
            for(int i = 0; i < objRow.GetFieldCount(); i++)
            {
                cout << objRow[i].ToString("NULL") << "\t";
            }
            cout << endl;
            // Test ToChar:
            cout << objRow[0].ToChar() << "," << objRow[1].ToChar() << "," << objRow[2].ToChar() << "," << objRow[3].ToChar() << "," << objRow[4].ToChar() << "," << objRow[5].ToChar('?') << endl;
        }
        
        // Print again: 
        cout << "Field information: " << endl;
        objQuery.Rewind();
        CFieldMysql objField;
        // Row: 
        while(objQuery.Fetch(objRow))
        {
            // Field: 
            for(int i = 0; i < objRow.GetFieldCount(); i++)
            {
                if(!objRow.GetField(i, objField)) throw CException(-1, "Internal error, can not find field: %d.", i);
                // Field information: 
                cout << "*Name: " << objField.Name() << ",Origin: " << objField.OriginName() << endl;
                cout << "Table: " << objField.Table() << ", Origin: " << objField.OriginTable() << endl;
                cout << "Database: " << objField.Database() << ", Catalog: " << objField.Catalog() << endl;
                cout << "Type: " << objField.Type() << ", Numeric: " << objField.IsNumeric() << ", Binary: " << objField.IsBinary() << endl;
                cout << "Defined: Not NULL: " << objField.IsDefinedNotNull() << ", Primary key: " << objField.IsPrimaryKey() << 
                    ", Decimals: " << objField.NumberOfDecimal() << endl;
            }
            cout << endl;
            //  Test type conversion: 
            cout << "Long: " << objRow[0].ToLong() << endl;
            cout << "Double: " << objRow[1].ToDouble() << endl;
        }

        // 
        objCon.Rollback();
        // 
        objCon.Disconnect();
        cout << "Database disconnected." << endl;
        if(objCon.IsAlive()) cout << "Database is still alive." << endl;
        else cout << "Database is not alive." << endl;
    }catch(CException &eEx)
    {
        std::cout << eEx.GetMessageStack() << std::endl;
        //eEx.Print();
        try
        {
            objCon.Rollback();
        }catch(CException& eEx1)
        {
            std::cout << eEx1.GetMessageStack() << std::endl;
            //eEx1.Print();
        }
        exit(-1);
    }
}
