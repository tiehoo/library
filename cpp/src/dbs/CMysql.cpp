/* =============================================================================
// Name:		 CDBMysql.cpp
// Description:  
// Function:     
// Vertion£º     (0.1)
// Auther:	     Stephen Hu(Tie Hu)
// Corporation:  Softroute
// Copyright (c) 
// Compilation:  
// Modified      (MM/DD/YY)  (Version) (Previours version) (Purpose)
   Stephen Hu    24/07/2005  0.1                           First Generate
============================================================================= */
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
// Mysql error codes: 
#include <errmsg.h>         // Client error;
#include <mysqld_error.h>   // Server error;
// 
#include <sstream>
// 
#include "CException.h"
#include "CMysql.h"

/*
#include <iostream>
using namespace std;
*/

//----------------------------------------------------------------------------//
// CThreadBeforeMysql: 
//----------------------------------------------------------------------------//
/*
If you want to connect to an external server without starting the embedded server, you have to specify a negative value for argc.
*/
CBeforeThreadMysql::CBeforeThreadMysql(int argc, char **argv, char **groups)
{
    if(0 != mysql_library_init(argc, argv, groups))
    {
        throw CException(-1, "%s(%ld): CBeforeThreadMysql::CBeforeThreadMysql(): mysql_library_init() failed.", __FILE__, __LINE__);
    }
}

CBeforeThreadMysql::~CBeforeThreadMysql()
{
    mysql_library_end();
}

bool CBeforeThreadMysql::IsThreadSafe() const
{
    if(1 == mysql_thread_safe()) return(true);
    else return(false);
}

//----------------------------------------------------------------------------//
// CInThreadMysql: 
//----------------------------------------------------------------------------//
CInThreadMysql::CInThreadMysql()
{
    if(0 != mysql_thread_init())
    {
        throw CException(-1, "%s(%ld): CBeforeThreadMysql::CBeforeThreadMysql(): mysql_library_init() failed.", __FILE__, __LINE__);
    }
}

CInThreadMysql::~CInThreadMysql()
{
    BeforePthreadExit();
}

void CInThreadMysql::BeforePthreadExit()
{
    mysql_thread_end();
}

//----------------------------------------------------------------------------//
// CConnectionMysql: 
//----------------------------------------------------------------------------//
/*
 Application programs should use this general outline for interacting with MySQL: 
   1. Initialize the MySQL library by calling mysql_library_init(). This function exists in both the mysqlclient C client library and the mysqld embedded server library, so it is used whether you build a regular client program by linking with the -libmysqlclient flag, or an embedded server application by linking with the -libmysqld flag.
   2. Initialize a connection handler by calling mysql_init() and connect to the server by calling mysql_real_connect().
   3. Issue SQL statements and process their results. (The following discussion provides more information about how to do this.)
   4. Close the connection to the MySQL server by calling mysql_close().
   5. End use of the MySQL library by calling mysql_library_end().
*/
// 
CConnectionMysql::CConnectionMysql(std::string sHost, std::string sUser, std::string sPass, std::string sDatabase, std::string sUnixSocket, int nPort) 
{
    try
    {
        // Initialize all members: 
        Clear();
        // Initialize MYSQL structure: 
        Init();
        // Connect: 
        if(!sHost.empty()) Connect(sHost, sUser, sPass, sDatabase, sUnixSocket, nPort);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::CConnectionMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CConnectionMysql::~CConnectionMysql()
{
    try
    {
        Disconnect();
//        CleanUp();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::~CConnectionMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
void CConnectionMysql::Clear()
{
    // 
    m_bIsConnected  = false;
    // 
    m_sHost.clear();
    m_sUser.clear();
    m_sPass.clear();
    m_sDatabase.clear();
    m_sUnixSocket.clear();
    m_nPort         = 0;
    // 
    m_lClientFlag   = 0;
}

/*
In a non-multi-threaded environment, the call to mysql_library_init() may be omitted, because mysql_init() will invoke it automatically as necessary. 
However, mysql_library_init() is not thread-safe in a multi-threaded environment, and thus neither is mysql_init(), which calls mysql_library_init(). 
You must either call mysql_library_init() prior to spawning any threads, or else use a mutex to protect the call, whether you invoke mysql_library_init() 
or indirectly via mysql_init(). This should be done prior to any other client library call.
*/
void CConnectionMysql::Init()
{
    if(NULL == mysql_init(&m_stMysql))
    {
        throw CException(-1, "%s(%ld): CConnectionMysql::Init(): mysql_init() failed. Insufficient memory.", __FILE__, __LINE__);
    }
}

// 
void CConnectionMysql::Close()
{
    mysql_close(&m_stMysql);
}

void CConnectionMysql::CleanUp()
{
    // Finalize server/library, clean up memory, shutdown embedded server.
    mysql_library_end();
}

//----------------------------------------------------------------------------//
void CConnectionMysql::SetHost(std::string sHost)
{
    m_sHost = sHost;
}

void CConnectionMysql::SetUser(std::string sUser)
{
    m_sUser = sUser;
}

void CConnectionMysql::SetPass(std::string sPass)
{
    m_sPass = sPass;
}

void CConnectionMysql::SetDatabase(std::string sDatabase)
{
    m_sDatabase = sDatabase;
}

void CConnectionMysql::SetUnixSocket(std::string sUnixSocket)
{
    m_sUnixSocket = sUnixSocket;
}

void CConnectionMysql::SetPort(int nPort)
{
    if(nPort < 0) throw CException(-1, "%s(%ld): CConnectionMysql::SetPort(): Invalid port: %d.", __FILE__, __LINE__, nPort);
    m_nPort = nPort;
}

//----------------------------------------------------------------------------//
// 1146
unsigned int CConnectionMysql::GetErrorCode()
{
    return(mysql_errno(&m_stMysql));
}

// 42S02
std::string CConnectionMysql::GetSqlState()
{
    return(mysql_sqlstate(&m_stMysql));
}

// Table 'test.no_such_table' doesn't exist
std::string CConnectionMysql::GetErrorMessage()
{
    return(mysql_error(&m_stMysql));
}

// ERROR 1146 (42S02): Table 'test.no_such_table' doesn't exist
std::string CConnectionMysql::GetErrorString()
{
    // Error code: 
    std::stringstream ssCode;
    ssCode << GetErrorCode();
    // Error message: 
    std::string sError;
    sError = "ERROR" + ssCode.str() + "(" + GetSqlState() + ")" + GetErrorMessage();
    return(sError);
}

//----------------------------------------------------------------------------//
void CConnectionMysql::SetAutoReconnect(bool bAutoReconnect)
{
    if(IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::SetAutoReconnect(): You should call this function before Connect().", __FILE__, __LINE__);
    my_bool bMyAuto;
    if(bAutoReconnect) bMyAuto = 1;
    else bMyAuto = 0;
    // 
    if(0 != mysql_options(&m_stMysql, MYSQL_OPT_RECONNECT, &bMyAuto))
    {
        throw CException(-1, "%s(%ld): CConnectionMysql::SetAutoReconnect(): mysql_option() failed.", __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------//
void CConnectionMysql::Reconnect(bool bCheckConnection)
{
    try
    {
        if(bCheckConnection && !IsConnected())
        {
            throw CException(-1, "%s(%ld): CConnectionMysql::Reconnect(): You need to call Connect() before you call Reconnect().", __FILE__, __LINE__);
        }
        // Reconnect, but don't reset connected flag.
        Close();
        // host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag
        if(!mysql_real_connect(&m_stMysql, m_sHost.c_str(), m_sUser.c_str(), m_sPass.c_str(), m_sDatabase.c_str(), m_nPort, m_sUnixSocket.c_str(), m_lClientFlag))
        {
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::Reconnect(): mysql_real_connect() failed, %s.", __FILE__, __LINE__, GetErrorString().c_str());
        }
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Connect(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CConnectionMysql::Connect()
{
    try
    {
        if(IsConnected())
        {
            Disconnect();
            //Init();
        }
        Reconnect(false);
        m_bIsConnected = true;
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Connect(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::Connect(std::string sHost, std::string sUser, std::string sPass, std::string sDatabase, std::string sUnixSocket, int nPort)
{
    try
    {
        // 
        SetHost(sHost);
        SetUser(sUser);
        SetPass(sPass);
        SetDatabase(sDatabase);
        SetUnixSocket(sUnixSocket);
        SetPort(nPort);
        // 
        Connect();
    }catch(CException eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Connect(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
bool CConnectionMysql::IsConnected() const
{
    return(m_bIsConnected);
}

// Checks whether the connection to the server is working. 
// If the connection has gone down, an attempt to reconnect is made unless auto-reconnect is disabled. 
bool CConnectionMysql::IsAlive()
{
    try
    {
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::IsAlive(): Database not connected yet.", __FILE__, __LINE__);
        switch(mysql_ping(&m_stMysql))
        {
        case 0:                         // Connection is still alive: 
            return(true);
        default: 
            // 
            switch(GetErrorCode())
            {
                case CR_SERVER_GONE_ERROR:      // The MySQL server has gone away.
                    throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::IsAlive(): [%s].", __FILE__, __LINE__, GetErrorString().c_str());
                case CR_COMMANDS_OUT_OF_SYNC:   // Commands were executed in an improper order.
                case CR_UNKNOWN_ERROR:          // An unknown error occurred. 
                default: 
                return(false);
            }
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::IsAlive(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
void CConnectionMysql::Disconnect()
{
    Close();
    m_bIsConnected = false;
}

//----------------------------------------------------------------------------//
// 
void CConnectionMysql::SelectDatabase(std::string sDatabase)
{
    if(mysql_select_db(&m_stMysql, sDatabase.c_str()) != 0)
    {
        throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::SelectDatabase(): mysql_select_db() failed, [%s].", __FILE__, __LINE__, GetErrorString().c_str());
    }
}

//----------------------------------------------------------------------------//
unsigned long CConnectionMysql::Execute(const char* szSql)
{
    // 
    try
    {
        if(NULL == szSql) throw CException(-1, "%s(%ld): CConnectionMysql::Execute(): NULL pointer.", __FILE__, __LINE__);
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::Execute(): Database has not connected yet.", __FILE__, __LINE__);
        if(mysql_query(&m_stMysql, szSql) != 0)
        {
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::Execute(): mysql_query() failed, [%s].", __FILE__, __LINE__, GetErrorString().c_str());
        }
        return(mysql_affected_rows(&m_stMysql));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Execute(const char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

unsigned long CConnectionMysql::Execute(const std::string& sSql)
{
    // 
    try
    {
        return(Execute(sSql.c_str()));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Execute(const std::string&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::SetAutoCommitOn()
{
    try
    {
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::AutoCommitOn(): Database not connected yet.", __FILE__, __LINE__);
        if(mysql_autocommit(&m_stMysql, 1)!= 0)
        {
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::AutoCommitOn(): mysql_autocommit() failed, [%s].", __FILE__, __LINE__, 
                GetErrorString().c_str());
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::SetAutoCommitOn(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::SetAutoCommitOff()
{
    try
    {
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::AutoCommitOff(): Database not connected yet.", __FILE__, __LINE__);
        if(mysql_autocommit(&m_stMysql, 0) != 0)
        {
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::AutoCommitOff(): mysql_autocommit() failed, [%s].", __FILE__, __LINE__, 
                GetErrorString().c_str());
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::SetAutoCommitOff(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::StartTransaction()
{
    try
    {
        Execute("START TRANSACTION");
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::StartTransaction(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::Commit()
{
    try
    {
        // 
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::Commit(): Database not connected yet.", __FILE__, __LINE__);
        if(mysql_commit(&m_stMysql) != 0)
        {
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::Commit(): mysql_commit() failed, [%s].", __FILE__, __LINE__, 
                GetErrorString().c_str());
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Commit(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

void CConnectionMysql::Rollback() 
{
    try
    {
        // 
        if(!IsConnected()) throw CException(-1, "%s(%ld): CConnectionMysql::Rollback(): Database not connected yet.", __FILE__, __LINE__);
        if(mysql_rollback(&m_stMysql)!=0)
        {
            /*
            throw CException(GetErrorCode(), "%s(%ld): CConnectionMysql::Rollback(): mysql_rollback() failed, [%s].", __FILE__, __LINE__, 
                GetErrorString().c_str());
            */
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CConnectionMysql::Rollback(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
// CFieldMysql: This class represent a mysql field from a row.
//----------------------------------------------------------------------------//
CFieldMysql::CFieldMysql()
{
    try
    {
        Clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CFieldMysql::CFieldMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CFieldMysql::~CFieldMysql()
{
}

//----------------------------------------------------------------------------//
void CFieldMysql::Clear()
{
    m_lFieldLength  = 0;
    m_pField        = NULL;
    m_pValue        = NULL;
}

const CFieldMysql& CFieldMysql::operator = (const CFieldMysql& refFieldMysql)
{
    try
    {
        // 
        m_lFieldLength  = refFieldMysql.m_lFieldLength;
        m_pField        = refFieldMysql.m_pField;
        m_pValue        = refFieldMysql.m_pValue;
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CFieldMysql::CFieldMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// Field definiton: 
MYSQL_FIELD_TYPE_T CFieldMysql::Type()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Type(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->type);
}

// Whether a field has a numeric type.
bool CFieldMysql::IsNumeric()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::IsNumeric(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(IS_NUM(Type()));
}

// To distinguish between binary and non-binary data for string data types.
bool CFieldMysql::IsBinary()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::IsBinary(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    if(63 == m_pField->charsetnr) return(true);
    else return(false);
}

// 
bool CFieldMysql::IsDefinedNotNull()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::IsDefinedNotNull(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(IS_NOT_NULL(m_pField->flags));
}

bool CFieldMysql::IsPrimaryKey()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::IsPrimaryKey(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(IS_PRI_KEY(m_pField->flags));
}

// The number of decimals for numeric fields.
unsigned int CFieldMysql::NumberOfDecimal()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::NumberOfDecimal(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->decimals);
}

/*
const char* CFieldMysql::DefaultValue()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::DefaultValue(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->def);
}

unsigned int CFieldMysql::DefaultValueLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::DefaultValueLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->def_length);
}
*/

// Field origination: 
const char* CFieldMysql::Name()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Name(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->name);
}
unsigned int CFieldMysql::NameLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::NameLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->name_length);
}

const char* CFieldMysql::OriginName()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::OriginName(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->org_name);
}
unsigned int CFieldMysql::OriginNameLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::OriginNameLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->org_name_length);
}

// 
const char* CFieldMysql::Table()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Table(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->table);
}
unsigned int CFieldMysql::TableLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::TableLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->table_length);
}

const char* CFieldMysql::OriginTable()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::OriginTable(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->org_table);
}
unsigned int CFieldMysql::OriginTableLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::OriginTableLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->org_table_length);
}

// 
const char* CFieldMysql::Database()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Database(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->db);
}
unsigned int CFieldMysql::DatabaseLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::DatabaseLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->db_length);
}

// 
const char* CFieldMysql::Catalog()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Catalog(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->catalog);
}
unsigned int CFieldMysql::CatalogLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::CatalogLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->catalog_length);
}

// Result set or value related: 
// The maximum width of the field for the result set
unsigned long CFieldMysql::MaxLength()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::MaxLength(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_pField->max_length);
}

// Field length.
unsigned long CFieldMysql::Length()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Length(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    return(m_lFieldLength);
}

// 
bool CFieldMysql::IsNull()
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::IsNull(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    // 
    if(NULL == m_pValue) return(true);
    else return(false);
}

// You cannot treat these as null-terminated strings if field values may contain binary data, 
//  because such values may contain null bytes internally.
const char* CFieldMysql::Value(const char* szNullValue)
{
    if(NULL == m_pField) throw CException(-1, "%s(%ld): CFieldMysql::Value(): No field information, call CRowMysql::GetField() first.", __FILE__, __LINE__);
    if(IsNull()) return(szNullValue);
    else return(m_pValue);
}

// 
std::string CFieldMysql::ToString(const char* szNullValue)
{
    if(IsNull())
    {
        if(NULL == szNullValue) return(std::string());
        else return(std::string(szNullValue));
    }else return(std::string(Value()));
}

char CFieldMysql::ToChar(char cNullValue, bool* bIsTruncated)
{
    if(IsNull()) return(cNullValue);
    else
    {
        if(NULL != bIsTruncated)
        {
            if(Length() > 1) *bIsTruncated = true;
            else *bIsTruncated = false;
        }
        return(Value()[0]);
    }
}

// 
long CFieldMysql::ToLong(long lNullValue)
{
    // 
    char    *pEnd;
    long    lValue = -1;
    int     i;
    
    // 1. Check if the value is NULL?
    if(IsNull()) return(lNullValue);
    
    // 2. Check if the value is convertable?
    if(!IsNumeric()) throw CException(-1, "%s(%ld): CFieldMysql::ToLong(): Incompatible field type can not convert to long, value is:[%s]", __FILE__, __LINE__, ToString().c_str());
    
    // 3. Convert value: 
    const char* szValue = Value();
    //  3.1. Check blank string.
    if('\0' == szValue[0])
    {
        // Treat as NULL: 
        return(lNullValue);
        //throw CException(-1, "%s(%ld): CFieldMysql::ToLong(long): Value contents nothing.", __FILE__, __LINE__);
    }
    //  3.2. Check 0 value.
    // FIXME: Use regular expresion.
    if('+' == szValue[0] || '-' == szValue[0]) i = 1;
    else i = 0;
    for(; ('0' == szValue[i] || '.' == szValue[i]) && '\0' != szValue[i]; ++i);
    if('\0' == szValue[i]) return(0);   // The value is 0.
    //  3.3. Convert value.
    lValue = strtol(szValue, &pEnd, 10);
    // TODO: errorno is not thread safe: 
    //if(EINVAL == errno || ERANGE == errno || '\0' != *pEnd)
    if(0 == lValue || '\0' != *pEnd)
    {
        throw CException(-1, "%s(%ld): CFieldMysql::ToLong(long): Conversion failed: [%s].", __FILE__, __LINE__, szValue);
    }
    return(lValue);
}

// 
double CFieldMysql::ToDouble(double fNullValue)
{
    // 
    char    *pEnd;
    double  fValue = -1;
    int     i;
    
    // 1. Check if the value is NULL?
    if(IsNull()) return(fNullValue);
    
    // 2. Check if the value is convertable?
    if(!IsNumeric()) throw CException(-1, "%s(%ld): CFieldMysql::ToLong(): Incompatible field type can not convert to double, value is:[%s]", __FILE__, __LINE__, ToString().c_str());
    
    // 3. Convert value: 
    const char* szValue = Value();
    //  3.1. Check balank string.
    if('\0' == szValue[0])
    {
        // Treat as NULL: 
        return(fNullValue);
        //throw CException(-1, "%s(%ld): CFieldMysql::ToLong(long): Value contents nothing.", __FILE__, __LINE__);
    }
    //  3.2. Check 0 value.
    // FIXME: Use regular expresion.
    if('+' == szValue[0] || '-' == szValue[0]) i = 1;
    else i = 0;
    for(; ('0' == szValue[i] || '.' == szValue[i]) && '\0' != szValue[i]; ++i);
    if('\0' == szValue[i]) return(0);   // The value is 0.
    //  3.3. Convert value.
    fValue = strtod(szValue, &pEnd);
    // TODO: errorno is not thread safe: 
    //if(EINVAL == errno || ERANGE == errno || '\0' != *pEnd)
    if((fValue > -0.000000000000001 && fValue < 0.000000000000001) || '\0' != *pEnd)
    {
        throw CException(-1, "%s(%ld): CFieldMysql::ToDouble(double): Conversion failed: [%s].", __FILE__, __LINE__, szValue);
    }
    return(fValue);
}

//----------------------------------------------------------------------------//
// CRowMysql: This class represent a mysql row from a result set.
//----------------------------------------------------------------------------//
CRowMysql::CRowMysql()
{
    try
    {
        Clear();
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CRowMysql::CRowMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CRowMysql::~CRowMysql()
{
}

//----------------------------------------------------------------------------//
void CRowMysql::Clear()
{
    m_nFieldCount   = 0;
    m_pField        = NULL;
    m_pRow          = NULL;
    m_lpFieldLength = NULL;
}

// 
unsigned int CRowMysql::GetFieldCount() const
{
    return(m_nFieldCount);
}

// 
bool CRowMysql::GetField(unsigned int nIndex, CFieldMysql& refField) const
{
    try
    {
        if(nIndex >= GetFieldCount()) 
        {
            return(false);
        }
        // 
        if(NULL == m_pField || NULL == m_pRow || NULL == m_lpFieldLength)
        {
            throw CException(-1, "%s(%ld): CRowMysql::GetField(): No row information, call CQueryMysql::Fetch() first.", __FILE__, __LINE__);
        }
        // 
        refField.Clear();
        refField.m_lFieldLength = m_lpFieldLength[nIndex];
        refField.m_pField       = &(m_pField[nIndex]);
        refField.m_pValue       = m_pRow[nIndex];
        return(true);
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CRowMysql::GetField(unsigned int, CFieldMysql&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
CFieldMysql CRowMysql::operator [] (unsigned int nIndex) const
{
    try
    {
        CFieldMysql objField;
        if(!GetField(nIndex, objField))
        {
            throw CException(-1, "%s(%ld): CRowMysql::operator [] (unsigned int): No field with index of: %d, should between: 0 and %d.", __FILE__, __LINE__, 
                    nIndex, GetFieldCount());
        }
        return(objField);
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CRowMysql::operator [] (unsigned int): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//----------------------------------------------------------------------------//
// CQueryMysql: 
//----------------------------------------------------------------------------//
CQueryMysql::CQueryMysql(CConnectionMysql &refConnection):m_refConnection(refConnection) 
{
    try
    {
        m_pResult           = NULL;
        m_pField            = NULL;
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::CQueryMysql(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

CQueryMysql::~CQueryMysql()
{
     Close();
}

//----------------------------------------------------------------------------//
void CQueryMysql::FreeResult()
{
     if(m_pResult != NULL)
     {
        mysql_free_result(m_pResult);
        m_pResult = NULL;
    }
}

void CQueryMysql::Close()
{
    FreeResult();
}

//----------------------------------------------------------------------------//
unsigned long CQueryMysql::Query(const char* szSql)
{
    try
    {
        if(NULL == szSql) throw CException(-1, "%s(%ld): CQueryMysql::Query(const char*): NULL pointer.", __FILE__, __LINE__);
        // 
        if(!m_refConnection.IsConnected())
        {
            throw CException(-1, "%s(%ld): CQueryMysql::Query(const char*): Database not connected yet.", __FILE__, __LINE__);
        }
        // 
        if(mysql_query(&(m_refConnection.m_stMysql), szSql) != 0)
        {
            throw CException(m_refConnection.GetErrorCode(), "%s(%ld): CQueryMysql::Query(const char*): mysql_query() failed, [%s], [%s].", __FILE__, __LINE__, 
                m_refConnection.GetErrorString().c_str(), szSql);
        }
        if(GetFieldCount() > 0) return(0);                                  // SELECT
        else return(mysql_affected_rows(&(m_refConnection.m_stMysql)));     // INSERT, DELETE, UPDATE
    }catch(CException &eEx) 
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::Query(const char*): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
unsigned long CQueryMysql::Query(const std::string& sSql)
{
    try
    {
        return(Query(sSql.c_str()));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::Query(const std::string&): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
unsigned long CQueryMysql::RealQuery(const char* pSql, long lSqlLen)
{
    try
    {
        if(NULL == pSql) throw CException(-1, "%s(%ld): CQueryMysql::RealQuery(): NULL pointer.", __FILE__, __LINE__);
        if(lSqlLen < 0) throw CException(-1, "%s(%ld): CQueryMysql::RealQuery(): Invalid SQL length: %ld.", __FILE__, __LINE__, lSqlLen);
        // 
        if(!m_refConnection.IsConnected())
        {
            throw CException(-1, "%s(%ld): CQueryMysql::RealQuery(): Database not connected yet.", __FILE__, __LINE__);
        }
        // 
        if(mysql_real_query(&(m_refConnection.m_stMysql), pSql, lSqlLen) != 0)
        {
            throw CException(m_refConnection.GetErrorCode(), "%s(%ld): CQueryMysql::RealQuery(): mysql_real_query() failed, [%s]", __FILE__, __LINE__, 
                m_refConnection.GetErrorString().c_str());
        }
        if(GetFieldCount() > 0) return(0);  // SELECT
        else return(mysql_affected_rows(&(m_refConnection.m_stMysql)));   // INSERT, DELETE, UPDATE
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::RealQuery(const char*, long): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// 
unsigned int CQueryMysql::GetFieldCount()
{
    try
    {
        if(!m_refConnection.IsConnected())
        {
            throw CException(-1, "%s(%ld): CQueryMysql::GetFieldCount(): Database not connected yet.", __FILE__, __LINE__);
        }
        // 
        return(mysql_field_count(&(m_refConnection.m_stMysql)));
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::GetFieldCount(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

const char* CQueryMysql::GetFieldName(int nIdx)
{
    if(m_pField == NULL) throw CException(-1, "%s(%ld): CQueryMysql::GetFieldName(): No field information available.", __FILE__, __LINE__);
    if(nIdx < 0 || nIdx >= GetFieldCount())
    {
        throw CException(-1, "%s(%ld): CQueryMysql::GetFieldType(): No such field index: %d, should between 0 and %d.", __FILE__, __LINE__, 
            nIdx, GetFieldCount() - 1);
    }
    return(m_pField[nIdx].name);
}

// 
unsigned long CQueryMysql::StoreResult()
{
    try
    {
        if(!m_refConnection.IsConnected())
        {
            throw CException(-1, "%s(%ld): CQueryMysql::StoreResult(): Database not connected yet.", __FILE__, __LINE__);
        }
        if(GetFieldCount() <= 0)
        {
            // Last statement did not generate a result set: 
            throw CException(-1, "%s(%ld): CQueryMysql::StoreResult(): Last SQL has no result set.", __FILE__, __LINE__);
        }else
        {
            // 
            if(NULL != m_pResult)
            {
                // There is a result set already: Free it first.
                FreeResult();
            }
            
            // There should be a result set: 
            if(NULL == (m_pResult = mysql_store_result(&(m_refConnection.m_stMysql))))
            {
                // With result set but returned NULL: 
                // a) There was a malloc() failure (for example, if the result set was too large). 
                // b) The data couldn't be read (an error occurred on the connection).
                if(m_refConnection.GetErrorCode() == 0)
                {
                    // No error indicated: 
                    throw CException(-1, "%s(%ld): CQueryMysql::StoreResult(): Internal error, mysql_store_result() failed, but no error code.", __FILE__, __LINE__);
                }else
                {
                    throw CException(m_refConnection.GetErrorCode(), 
                        "%s(%ld): CQueryMysql::StoreResult(): mysql_store_result() failed, [%s].", __FILE__, __LINE__, m_refConnection.GetErrorString().c_str());
                }
            }else
            {
                // Result stored: 
                //  Set field information: 
                m_pField = mysql_fetch_fields(m_pResult);
                return(mysql_num_rows(m_pResult));          // Rows returned by query.
            }
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::StoreResult(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

// May be used only in conjunction with mysql_store_result(), not with mysql_use_result()
void CQueryMysql::Rewind()
{
    if(NULL == m_pResult) throw CException(-1, "%s(%ld): CQueryMysql::Rewind(): No result set, call CQueryMysql::StoreResult() first.", __FILE__, __LINE__);
    mysql_data_seek(m_pResult, 0);
}

void CQueryMysql::UseResult()
{
    try
    {
        if(!m_refConnection.IsConnected())
        {
            throw CException(-1, "%s(%ld): CQueryMysql::UseResult(): Database not connected yet.", __FILE__, __LINE__);
        }
        if(GetFieldCount() <= 0)
        {
            // Last statement did not generate a result set: 
            throw CException(-1, "%s(%ld): CQueryMysql::UseResult(): Last SQL has no result set.", __FILE__, __LINE__);
        }else
        {
            // 
            if(NULL != m_pResult)
            {
                // There is a result set already: Free it first.
                FreeResult();
            }
            // There should be a result set: 
            if((m_pResult = mysql_use_result(&(m_refConnection.m_stMysql))) == NULL)
            {
                // With result set but returned NULL: 
                // a) There was a malloc() failure (for example, if the result set was too large). 
                // b) The data couldn't be read (an error occurred on the connection).
                if(m_refConnection.GetErrorCode() == 0)
                {
                    // No error indicated: 
                    throw CException(-1, "%s(%ld): CQueryMysql::UseResult(): Internal error, mysql_use_result() failed, but no error code.", __FILE__, __LINE__);
                }else
                {
                    throw CException(m_refConnection.GetErrorCode(), 
                        "%s(%ld): CQueryMysql::UseResult(): mysql_store_result() failed, [%s].", __FILE__, __LINE__, m_refConnection.GetErrorString().c_str());
                }
            }else
            {
                // Result stored: 
                //  Set field information: 
                m_pField = mysql_fetch_fields(m_pResult);
                // Does not return the correct value until all the rows in the result set have been retrieved. 
                //return(mysql_num_rows(m_pResult));
            }
        }
    }catch(CException &eEx)
    {
        eEx.PushMessage("%s(%ld): CQueryMysql::UseResult(): Failed.", __FILE__, __LINE__);
        throw eEx;
    }
}

//Fetch a row into m_pRow;
bool CQueryMysql::Fetch(CRowMysql& refRow)
{
    if(m_pResult == NULL) throw CException(-1, "%s(%ld): CQueryMysql::Fetch(): Result set is NULL.", __FILE__, __LINE__);
    if((refRow.m_pRow = mysql_fetch_row(m_pResult)) == NULL)
    {
        // EOF or error:
        if(0 == mysql_errno(&(m_refConnection.m_stMysql))) return(false); // EOF
        else
        {
            throw CException(m_refConnection.GetErrorCode(), "%s(%ld): CQueryMysql::Fetch(): mysql_fetch_row failed, [%s].", __FILE__, __LINE__, 
                m_refConnection.GetErrorString().c_str());
        }
    }
    // 
    refRow.m_nFieldCount = GetFieldCount();
    refRow.m_pField      = m_pField;
    // Get field lengths array: 
    refRow.m_lpFieldLength = mysql_fetch_lengths(m_pResult);
    return(true);
}
