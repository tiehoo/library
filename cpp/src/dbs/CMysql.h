/* =============================================================================
// Name:         CMysql.h
// Description:  
// Function:     
// Vertion£º     (0.1)
// Auther:       Stephen Hu(Tie Hu)
// Corporation:  Softroute
// Copyright (c) 
// Compilation:  
// Modified      (MM/DD/YY)  (Version) (Previours version) (Purpose)
   Stephen Hu    24/07/2005  0.1                           First Generate
============================================================================= */
#ifndef _CMYSQL_H
#define _CMYSQL_H

/* =========================================================================== /
* Place includes, constant defines here.
/============================================================================ */
// 
#include <mysql.h>
#include <string>

/*
//  Make a thread safe client: 
./configure --enable-thread-safe-client
*/
//----------------------------------------------------------------------------//
// CBeforeThreadMysql: 
//----------------------------------------------------------------------------//
// 1. Define this object before spawning any thread.
class CBeforeThreadMysql
{
public: 
    CBeforeThreadMysql(int argc = -1, char **argv = NULL, char **groups = NULL);
    ~CBeforeThreadMysql();
    // Whether the client library is compiled as thread-safe.
    bool    IsThreadSafe() const;
};

//----------------------------------------------------------------------------//
// CInThreadMysql: 
//----------------------------------------------------------------------------//
// If you want to share connections between threads, new this object in each thread 
// before calling any other MYSQL functions.
// 1. Define this object in the thread before any MYSQL call.
// 2. Call BeforePthreadExit() before pthread_exit().
class CInThreadMysql
{
public: 
    CInThreadMysql();
    ~CInThreadMysql();
// 
    void BeforePthreadExit();
};

//----------------------------------------------------------------------------//
// CConnectionMysql: 
//----------------------------------------------------------------------------//
// Constants: 
const int   DEF_MYSQL_PORT    = 3060;

// 
class CConnectionMysql
{
private: 
// 
    MYSQL           m_stMysql;      // Database connection
// 
    std::string     m_sHost;
    std::string     m_sUser;
    std::string     m_sPass;
    std::string     m_sDatabase;
    std::string     m_sUnixSocket;
    unsigned int    m_nPort;
    unsigned long   m_lClientFlag;
// 
    bool            m_bIsConnected;
// 
    void            Clear();        // Clear members.
//
    void            Init();         // Initialize MYSQL structure.
    void            Close();        // Close connection.
    void            CleanUp();      // Clean memory, shutdown embedded server.
public: 
    // nPort = 0 means use default port. set sHost to "localhost" to connect to local server.
    explicit CConnectionMysql(std::string sHost = "", std::string sUser = "", std::string sPass = "", std::string sDatabase = "", 
        std::string sUnixSocket = "", int nPort = 0);
    ~CConnectionMysql();
// 
    void            SetHost(std::string sHost);
    void            SetUser(std::string sUser);
    void            SetPass(std::string sPass);
    void            SetDatabase(std::string sDatabase);
    void            SetUnixSocket(std::string sUnixSocket);
    void            SetPort(int nPort);
// 
    unsigned int    GetErrorCode();     // 1146
    std::string     GetSqlState();      // 42S02
    std::string     GetErrorMessage();  // Table 'test.no_such_table' doesn't exist
    std::string     GetErrorString();   // ERROR 1146 (42S02): Table 'test.no_such_table' doesn't exist
// 
    // mysql_options() should be called after mysql_init() and before mysql_connect() or mysql_real_connect().
    void            SetAutoReconnect(bool bAutoReconnect = false);
// Connection: 
    // Used to connect/reconnect db.
    void            Connect();
    // nPort = 0 means use default port. If want to connect to local host: Set sHost = "localhost"
    void            Connect(std::string sHost, std::string sUser = "", std::string sPass = "", 
                        std::string sDatabase = "", std::string sUnixSocket = "", int nPort = 0);
    // You can only call Reconnect() after you called Connect().
    void            Reconnect(bool bCheckConnection = true);
    // Disconnect: 
    void            Disconnect();
    // Ping and try to reconnect.
    bool            IsAlive();
    // 
    bool            IsConnected() const;
// Switch database: 
    // 
    void            SelectDatabase(std::string sDatabase);
// Execute SQL: 
    // Returns the number of rows changed (for UPDATE), deleted (for DELETE), or inserted (for INSERT). 
    // Note: for UPDATE if 1 row matched but 0 row changed, this will return 0;
    // For SELECT statements, mysql_affected_rows() works like mysql_num_rows(). 
    unsigned long   Execute(const char* szSql);
    unsigned long   Execute(const std::string& sSql);
// Transaction: 
    void            StartTransaction();
    void            SetAutoCommitOn();
    void            SetAutoCommitOff();
    // 
    void            Commit();
    void            Rollback();
//
    friend class    CQueryMysql;
};

/*
Note: CFieldMysql, CRowMysql is not memory safe, don't access them after you destroyed CQueryMysql!
*/
//----------------------------------------------------------------------------//
// CFieldMysql: This class represent a mysql field from a row.
//----------------------------------------------------------------------------//
// Constants: 
#define MYSQL_FIELD_TYPE_T  enum enum_field_types

// 
class CFieldMysql
{
private: 
    MYSQL_FIELD*            m_pField;               // This is pointer to the structure. m_pField[0].
    char*                   m_pValue;               // Field value.
    unsigned long           m_lFieldLength;         // Field length.
public: 
    CFieldMysql();
    ~CFieldMysql();
// 
    void                    Clear();
    const CFieldMysql&      operator = (const CFieldMysql& refFieldMysql);
// Field definiton: 
    MYSQL_FIELD_TYPE_T      Type();                 // 
    bool                    IsNumeric();            // Whether a field has a numeric type.
    bool                    IsBinary();             // To distinguish between binary and non-binary data for string data types.
    // 
    bool                    IsDefinedNotNull();     // 
    bool                    IsPrimaryKey();
    unsigned int            NumberOfDecimal();      // The number of decimals for numeric fields.
    // This may be NULL, This is set only if you use mysql_list_fields().
/*
    const char*             DefaultValue();
    unsigned int            DefaultValueLength();
*/
// Field origination: 
    const char*             Name();
    unsigned int            NameLength();
    const char*             OriginName();
    unsigned int            OriginNameLength();
    // 
    const char*             Table();
    unsigned int            TableLength();
    const char*             OriginTable();
    unsigned int            OriginTableLength();
    // 
    const char*             Database();
    unsigned int            DatabaseLength();
    // 
    const char*             Catalog();
    unsigned int            CatalogLength();
// Result set or value related: 
    unsigned long           Length();               // Field length.
    unsigned long           MaxLength();            // The maximum width of the field for the result set
    // 
    bool                    IsNull();
    // 
    // You cannot treat these as null-terminated strings if field values may contain binary data, 
    //  because such values may contain null bytes internally.
    const char*             Value(const char* szNullValue = NULL);
    std::string             ToString(const char* szNullValue = NULL);
    char                    ToChar(char cNullValue = '\0', bool* bIsTruncated = NULL);
    long                    ToLong(long lNullValue = 0);
    double                  ToDouble(double fNullValue = 0);
// 
    friend class            CRowMysql;
};

//----------------------------------------------------------------------------//
// CRowMysql: This class represent a mysql row from a result set.
//----------------------------------------------------------------------------//
class CRowMysql
{
private: 
    // Field information: 
    unsigned int        m_nFieldCount;              // Number of fields.
    MYSQL_FIELD*        m_pField;                   // Field information(array) from result set.
    // Row data: 
    MYSQL_ROW           m_pRow;                     // One row from result set; It's a char**, array of string.
    // 
    unsigned long*      m_lpFieldLength;            // Field length array from result set.
public: 
    CRowMysql();
    ~CRowMysql();
// 
    void                Clear();
    // 
    unsigned int        GetFieldCount() const;
    // nIndex starts from 0.
    bool                GetField(unsigned int nIndex, CFieldMysql& refField) const;
    CFieldMysql         operator [] (unsigned int nIndex) const;
// 
    friend class        CQueryMysql;
};

//----------------------------------------------------------------------------//
// CQueryMysql: This class represent a mysql result set.
//----------------------------------------------------------------------------//
class CQueryMysql
{
private: 
    // Database connection.
    CConnectionMysql&   m_refConnection;
    // 
    MYSQL_RES*          m_pResult;                  // Query result set.
    // 
    MYSQL_FIELD*        m_pField;                   // Field information(array) from result set.
public: 
    explicit CQueryMysql(CConnectionMysql &refConnection);
    ~CQueryMysql();
// 
    // Return rows affected, but for a SELECT it returns 0: 
    unsigned long       Query(const char* szSql);
    unsigned long       Query(const std::string& sSql);
    // If the query contains binary data, you can use RealQuery: 
    unsigned long       RealQuery(const char* pSql, long lSqlLen);

// 
    // If field count is 0, that means last SQL does not generate result set: 
    unsigned int        GetFieldCount();
    const char*         GetFieldName(int nIdx);
// 
    // Return rows stored.
    unsigned long       StoreResult();              // Full result set: .
    void                Rewind();                   // Rewind to the beginning of the result set, can only be used with StoreResult().
    void                UseResult();                // Just used result set: .
    // Fetch a row: 
    bool                Fetch(CRowMysql& refRow);   // Fetch a row into m_pRow;
    // Free result: 
    void                FreeResult();
// 
    void                Close();
};

#endif
