<?php
/* =========================================================================== /
* Place includes, constant defines and $_GLOBAL settings here.
/============================================================================ */

/* =========================================================================== /
* Place class interface here: All public member variables and public functions.
/============================================================================ */

interface iConnectionMysql
{
    function __construct($dbHost = NULL, $dbUser = NULL, $dbPassword = NULL, $dbDatabase = NULL, $dbPort = NULL, $isPersistent = false);
    public function __destruct();
    // Connection: 
    public function Close();
    public function IsConnected();
    public function GetConnection();
    // 
    public function AffectedRow();
    public function Execute($query);
    // 
    public function UseDatabase($database);
    // Transaction control: 
    public function StartTransaction();
    public function SetAutoCommitOn();
    public function SetAutoCommitOff();
    public function Commit();
    public function Rollback();
    // Error report: 
    public function GetErrorCode();
    public function GetErrorMessage();
    public function GetErrorString();
    // 
    public function SetLogOn();
    public function SetLogOff();
    public function IsLogOn();
}

interface iQueryMysql
{
    public function __construct(&$connectionMysql = NULL);
    public function __destruct();
    public function SetConnection(&$connectionMysql);
// Parser: 
    // Escape string: 
    public function EscapeStringMysql($string);
    public function EscapeDollaSign($string);
    public function UnescapeDollaSign($string);
    public function WrapExpression($expression);
    public function UnwrapExpression($expression);

// Execute
    public function NumberOfRow();
    public function Execute($query, $arrValue = NULL, $arrValue1 = NULL, $holderSimple = self::PLACE_HOLDER_SIMPLE);
// INSERT, UPDATE, DELETE: 
    public function Insert($tableName, $arrField, $arrFieldUpdate = NULL);
    public function Update($tableName, $arrField, $sqlWhere = "0 = 1", $arrValueWhere = NULL);
    public function Delete($tableName, $sqlWhere = "0 = 1", $arrValueWhere = NULL);
    public function Fetch($resultType = MYSQL_BOTH);
    public function GetRow($query, $arrField = NULL, $resultType = MYSQL_BOTH);
    public function GetList($query, $offset = 0, $pageSize = -1, $arrField = NULL, $resultType = MYSQL_BOTH);
    public function GetLastInsertId();
// Database information: 
    public function DoesTableExist($tableName);
}

/*
// Mysql
    // This class has all behaviors of ConnectionMysql or QueryMysql
    // Class Mysql extetends from ConnectionMysql and has a menber class QueryMysql.
*/

/* =========================================================================== /
// ConnectionMysql: 
/============================================================================ */
class ConnectionMysql
{
//------------------------------------------------------------------------------
// Member: 
//------------------------------------------------------------------------------
    public      $m_linkId   = NULL;     // MySQL link identifier returned by mysql_connect() or mysql_pconnect()
    //  
    private     $m_isLogOn  = false;

//------------------------------------------------------------------------------
// Private: 
//------------------------------------------------------------------------------
    // Connect database: 
    private function Connect($dbHost = NULL, $dbUser = NULL, $dbPassword = NULL, $dbDatabase = NULL, $dbPort = NULL, $isPersistent = false)
    {
        // Establish connection, select database.
        if(NULL == $this->m_linkId) 
        {
            // Routine check: 
            if(!isset($dbHost, $dbUser, $dbPassword))
            {
                throw new Exception("Invlaid parameter: dbHost[$dbHost], dbUser[$dbUser], dbPassword[$dbPassword]", -1);
            }
            // Handle port: 
            if(isset($dbPort)) $db_host = $dbHost . ':' . $dbPort;
            else $db_host = $dbHost;
            
            // Connect to database: 
            if($isPersistent)
            {
                $this->m_linkId = @mysql_pconnect($db_host, $dbUser, $dbPassword);
            }
            else
            {
                $this->m_linkId = @mysql_connect($db_host, $dbUser, $dbPassword);
            }
            if(false === $this->m_linkId)
            {
                throw new Exception("Connect database failed.", -1);
            }
            
            // Select database: 
            if(!empty($dbDatabase))
            {
                $this->UseDatabase($dbDatabase);
            }
        }
        return($this->m_linkId);
    }   // Connect()

    // Ping and reconnect servers
    //  return: true: if connection to server is working.
    public function Ping()
    {
        // 
        if(NULL !== $this->m_linkId)
        {
            return(mysql_ping($this->m_linkId));
        }else
        {
            return(false);
        }
    }
    
    // 
    public function UseDatabase($database)
    {
        if(empty($database)) 
        {
            throw new Exception("database is empty.", -1);
        }
        if(false === @mysql_select_db($database, $this->m_linkId))
        {
            throw new Exception(mysql_error($this->m_linkId), mysql_errno($this->m_linkId));
        }
    }

//------------------------------------------------------------------------------
// Public: 
//------------------------------------------------------------------------------
// Public: 
    // Constructor
    public function __construct($dbHost = NULL, $dbUser = NULL, $dbPassword = NULL, $dbDatabase = NULL, $dbPort = NULL, $isPersistent = false)
    {
        try
        {
            if(isset($dbHost))
            {
                $this->Connect($dbHost, $dbUser, $dbPassword, $dbDatabase, $dbPort, $isPersistent);
            }
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Destructor
    public function __destruct()
    {
        try
        {
            $this->Close();
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

// Connect and disconnect
    // Close connection: 
    public function Close()
    {
        try
        {
            if(NULL !== $this->m_linkId)
            {
                @mysql_close($this->m_linkId);
                $this->m_linkId = NULL;
            }
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

// Get connection, test connection
    // Test if database has been connected, and try to reconnect if not.
    //  return: true: if connection to server is working.
    public function IsConnected()
    {
        return($this->Ping());
    }

    // return link ID: 
    public function GetConnection()
    {
        if(NULL == $this->m_linkId)
        {
            throw new Exception("Database has not been connected yet.", -1);
        }else
        {
            return($this->m_linkId);
        }
    }

    // Get the number of affected rows by the last INSERT, UPDATE or DELETE query, -1 if the last query failed.
    public function AffectedRow()
    {
        // Return affected rows: 
        return(mysql_affected_rows($this->GetConnection()));
    }

    // Execute: 
    public function Execute($query)
    {
        if(empty($query))
        {
            throw new Exception("SQL is empty.", -1);
        }
        $resource = mysql_query($query, $this->GetConnection());
        // 
        if(is_resource($resource))
        {
            // Then this is a select, show, describe or explan returned with a result set: return number: 
            throw new Exception("SELECT/SHOW etc not supported here. Use QueryMysql object.", -1);
        }
        if(true === $resource)
        {
            // Succeeded: return number of rows affected.
            return($this->AffectedRow());
        }else if(false === $resource)
        {
            // Failed: 
            throw new Exception($this->GetErrorString() . "\r\nSQL = [\r\n" . $query . "]\r\n", $this->GetErrorCode());
        }else
        {
            throw new Exception("Internal error: Got unexpected value.", -1);
        }
    }

    // Parameter: 
    public function SetCharSet($charset) {
        if(NULL == $this->m_linkId) {
            throw new Exception("No connection. Connect first.", -1);
        }
        mysql_set_charset($charset, $this->m_linkId);
    }
    // 
    public function SetLogOn()
    {
        $this->m_isLogOn = true;
    }
    // 
    public function SetLogOff()
    {
        $this->m_isLogOn = false;
    }
    // 
    public function IsLogOn()
    {
        return($this->m_isLogOn);
    }

    // Transaction control: 
    public function StartTransaction()
    {
        try
        {
            $this->Execute("START TRANSACTION");
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Turn auto commit on or off
    public function SetAutoCommitOn()
    {
        try
        {
            $this->Execute("SET AUTOCOMMIT = 1");
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    public function SetAutoCommitOff()
    {
        try
        {
            $this->Execute("SET AUTOCOMMIT = 0");
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // 
    public function Commit()
    {
        try
        {
            $this->Execute("COMMIT");
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    public function Rollback()
    {
        try
        {
            $this->Execute("ROLLBACK");
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    
    // Returns mysql error
    public function GetErrorCode()
    {
        if(NULL != $this->m_linkId)
        {
            return(mysql_errno($this->m_linkId));
        }else
        {
            return(mysql_errno());
        }
    }
    // 
    public function GetErrorMessage()
    {
        if(NULL != $this->m_linkId)
        {
            return(mysql_error($this->m_linkId));
        }else
        {
            return(mysql_error());
        }
    }
    // 
    public function GetErrorString()
    {
        return($this->GetErrorCode() . ' - ' . $this->GetErrorMessage());
    }
}

/* =========================================================================== /
// QueryMysql: 
/============================================================================ */
class QueryMysql
{
//------------------------------------------------------------------------------
// Member: 
//------------------------------------------------------------------------------
// Constant: 
    // Expression wrap: 
    const       EXP_WRAP_LEFT       = "/*+@-";
    const       EXP_WRAP_RIGHT      = "-@+*/";
    const       PLACE_HOLDER_SIMPLE = "?";
    const       PLACE_HOLDER_FORMAL = "_@P.H-F@_";
// Private: 
    //  Connections: 
    private     $m_connection       = NULL;     // Reference to object ConnectionMysql
    //  Query resources: 
    private     $m_resource         = NULL;     // Resource returned by mysql_query()
    
//------------------------------------------------------------------------------
// Private: 
//------------------------------------------------------------------------------
    // Free result: 
    private function Clear()
    {
        if(is_resource($this->m_resource))
        {
            @mysql_free_result($this->m_resource);
        }
        $this->m_resource = NULL;
    }

// Public: 
    // Constructor
    public function __construct(&$connectionMysql = NULL)
    {
        try
        {
            // 
            if(!empty($connectionMysql)) $this->SetConnection($connectionMysql);
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Destructor
    public function __destruct()
    {
        $this->Clear();
    }

    // Set database connection: 
    public function SetConnection(&$connectionMysql)
    {
        if(empty($connectionMysql) || !is_object($connectionMysql))
        {
            throw new Exception("It's not a valid database connection.", -1);
        }
        $this->m_connection = $connectionMysql;
    }

// Parser: 
    // 
    static public function EscapeStringMysql($string)
    {
        return(mysql_real_escape_string($string));
    }
    //
    static public function EscapeDollaSign($string) 
    {
        return(str_replace("\$", "%-dollar-%", $string));
    }
    //
    static public function UnescapeDollaSign($string) 
    {
        return(str_replace("%-dollar-%", "\$", $string));
    }
    static public function WrapExpression($expression)
    {
        // 
        return(self::EXP_WRAP_LEFT . $expression . self::EXP_WRAP_RIGHT);
    }
    // 
    static public function UnwrapExpression($expression)
    {
        // 
        $expression_trimed = trim($expression);
        if(QueryMysql::IsWrapedExpression($expression))
        {
            return(substr(substr($expression_trimed, strlen(self::EXP_WRAP_LEFT)), 0, -1 * strlen(self::EXP_WRAP_RIGHT)));
        }else return($expression);
    }
    // 
    static public function IsWrapedExpression($expression)
    {
        // 
        $expression_trimed = trim($expression);
        if(substr($expression_trimed, 0, strlen(self::EXP_WRAP_LEFT)) == self::EXP_WRAP_LEFT && 
            substr($expression_trimed, -1 * strlen(self::EXP_WRAP_RIGHT), strlen(self::EXP_WRAP_RIGHT)) == self::EXP_WRAP_RIGHT)
        {
            return(true);
        }else return(false);
    }

    // Replace ? to place holder: 
    public function PrepareSql($query, $holderSimple = self::PLACE_HOLDER_SIMPLE, $holderFormal = self::PLACE_HOLDER_FORMAL)
    {
        // Check input: 
        if(!isset($query) || '' == $query) return($query);
        if(!isset($holderSimple) || '' == $holderSimple) return($query);
        if(!isset($holderFormal) || '' == $holderFormal) return($query);
        // 
        $query = str_replace($holderSimple, $holderFormal, $query);
        return($query);
    }
    
    // Parse space holder into a query: 
    // Parse("SELECT * FROM tab WHERE field1 = '_@P.H@_' AND field2 = _@P.H@_", array('stephen', 0))
    public function ParseSql($query, $arrValue = NULL, $placeHolder = self::PLACE_HOLDER_FORMAL)
    {
        // Check input: 
        if(!isset($query) || '' == $query) return($query);
        if(!isset($arrValue) || !is_array($arrValue) || count($arrValue) <= 0) return($query);

        // Parse space holder one by one: 
        foreach ($arrValue as $key => $value)
        {
            if (NULL === $value)
            {
                $value_final = 'NULL';
            }else if($this->IsWrapedExpression($value))
            {
                // No need to call EscapeString: STR_TO_DATE('2009-12-11', '%y-%m-%d').
                // If called will change it to: STR_TO_DATE(\'2009-12-11\', \'%y-%m-%d\')
                $value_final = $this->UnwrapExpression($value);
            }else
            {
                // Only escape values: 
                $value_final = QueryMysql::EscapeStringMysql($value);
            }
            // $ sign will affect preg_replace(), so need to escape back and forth.
            $query = QueryMysql::UnescapeDollaSign(preg_replace("/{$placeHolder}/", QueryMysql::EscapeDollaSign($value_final), $query, 1));
        }
        return($query);
    }
    
// Execute
    // Rows selected.
    public function NumberOfRow()
    {
        // Check connection: 
        if(NULL == $this->m_resource)
        {
            throw new Exception("No query result available.", -1);
        }
        // Return matched rows: 
        return(mysql_num_rows($this->m_resource));
    }

    // Excute a command: 
    //  return: Affected number of rows
    //  You can also call like this: 
    //  Execute("SELECT * FROM tab WHERE field1 = '?' AND field2 = ? AND ? > '2009-10-10'", array('stephen', 0, WrapExpression("NOW()")))
    public function Execute($query, $arrValue = NULL, $arrValue1 = NULL, $holderSimple = self::PLACE_HOLDER_SIMPLE)
    {
        try
        {
            // Check connection: 
            if(NULL == $this->m_connection || NULL == $this->m_connection->GetConnection())
            {
                throw new Exception("Database has not been connected yet.", -1);
            }
            // If a null string, return 0;
            if(!isset($query) || "" == $query) throw new Exception("Empty query string:[$query].", -1);
            // Free privious result if have: 
            if(NULL !== $this->m_resource) $this->Clear();
            // 
            if($this->m_connection->IsLogOn())
            {
                openlog("QueryMysql", LOG_PID | LOG_PERROR, LOG_LOCAL0);
                syslog(LOG_WARNING, "MYSQL_QUERY_ORIGINAL: [{$query}]");
            }
            // Query
            // Prepare query: Replace simple place holder to formal place holder.
            if(isset($arrValue) || isset($arrValue1))
            {
                $query = $this->PrepareSql($query);
            }
            // Parse query first: 
            if(isset($arrValue) && count($arrValue) > 0)
            {
                $query = $this->ParseSql($query, $arrValue, self::PLACE_HOLDER_FORMAL);
            }
            if(isset($arrValue1) && count($arrValue1) > 0)
            {
                $query = $this->ParseSql($query, $arrValue1, self::PLACE_HOLDER_FORMAL);
            }

            // Loging: 
            $time_start = NULL;
            if($this->m_connection->IsLogOn())
            {
                openlog("QueryMysql", LOG_PID | LOG_PERROR, LOG_LOCAL0);
                $time_start = microtime(true);
                syslog(LOG_WARNING, "MYSQL_QUERY_FINAL: [{$query}]");
            }
            // Execute: 
            $this->m_resource = mysql_query($query, $this->m_connection->GetConnection());
            if($this->m_connection->IsLogOn())
            {
                $time_end = microtime(true);
                $time = $time_end - $time_start;
                syslog(LOG_WARNING, "MYSQL_USED_TIME: [{$time}]");
                closelog();
            }

            // 
            if(is_resource($this->m_resource))
            {
                // Then this is a select, show, describe or explan returned with a result set: return number: 
                return($this->NumberOfRow());
            }
            // The query should be: delete, insert, update or drop etc here.
            if(true === $this->m_resource)
            {
                // Succeeded: return number of rows affected.
                $this->m_resource = NULL;
                return($this->m_connection->AffectedRow());
            }else if(false === $this->m_resource)
            {
                // Failed: 
                $this->m_resource = NULL;
                throw new Exception($this->m_connection->GetErrorString() . "\r\nSQL = [\r\n" . $query . "]\r\n", $this->m_connection->GetErrorCode());
            }else
            {
                throw new Exception("Internal error: Got unexpected value.", -1);
            }
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // Execute()

// INSERT, UPDATE, DELETE: 
    // Insert("sys_log", array('reg_id' => 'stephen', 'par2' => 1.0, 'cat_id' => NULL, 
    //      'time_stamp' => WrapExp('NOW() + INTERVAL 1 DAY'))
    // Return last insert ID: 0: No ID geterated by last insert. >0: ID generated.
    public function Insert($tableName, $arrField, $arrFieldUpdate = NULL)
    {
        try
        {
            if(!isset($tableName)) throw new Exception("Insert without table name.", -1);
            if(!is_array($arrField) || count($arrField) <= 0) throw new Exception("Insert without field.", -1);
            // 
            $fields = implode(array_keys($arrField), "`,`");
            $query  = "INSERT INTO {$tableName} (`{$fields}`) VALUES(";
            // 
            foreach($arrField as $key => $value)
            {
                if(NULL !== $value && !$this->IsWrapedExpression($value))
                {
                    $query .= " '?',";  // This is a normal string variable.
                }else $query .= " ?,";  // This is a non-string variable or an wraped expression.
            }
            $query = substr($query, 0, -1);
            $query .= ") ";
            // 
            if(is_array($arrFieldUpdate) && count($arrFieldUpdate) > 0)
            {
                $query .= "ON DUPLICATE KEY UPDATE ";
                foreach($arrFieldUpdate as $key => $value) 
                {
                    if(NULL !== $value && !$this->IsWrapedExpression($value))
                    {
                        $query .= " `{$key}` = '?',";  // This is a normal string variable.
                    }else $query .= " `{$key}` = ?,";  // This is a non-string variable or an wraped expression.
                }
                $query = substr($query, 0, -1);
            }
            // 
            $this->Execute($query, $arrField, $arrFieldUpdate);
            return($this->GetLastInsertId());
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // Insert()

    // Update("sys_log", array('reg_id' => 'stephen1', 'par2' => NULL), "reg_id = ?", array('stephen')
    public function Update($tableName, $arrField, $sqlWhere = "0 = 1", $arrValueWhere = NULL)
    {
        try
        {
            if(!isset($tableName)) throw new Exception("Update without table name.", -1);
            if(!is_array($arrField) || count($arrField) <= 0) throw new Exception("Update without field.", -1);
            // 
            $query = "UPDATE {$tableName} SET";
            foreach($arrField as $key => $value)
            {
                if(NULL !== $value && !$this->IsWrapedExpression($value))
                {
                    $query .= " `$key` = '?',";
                }else $query .= " `$key` = ?,";
            }
            $query = substr($query, 0, -1);
            // 
            if(!is_string($sqlWhere)) 
            {
                throw new Exception("No where specified. [$sqlWhere]", -1);
            }
            $query .= " WHERE " . $sqlWhere;
            // Merge two array: 
            if(isset($arrValueWhere))
            {
                return($this->Execute($query, array_merge($arrField, $arrValueWhere)));
            }else return($this->Execute($query, $arrField));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // Update()

    // I don't support deleting all records in a table here, you have to use Execute().
    // Delete("test", "name = '?'", array("stephen"));
    public function Delete($tableName, $sqlWhere = "0 = 1", $arrValueWhere = NULL)
    {
        try
        {
            if(!isset($tableName)) throw new Exception("Delete without table name.", -1);
            // 
            if(!isset($sqlWhere) || '' == $sqlWhere) 
            {
                throw new Exception("No where specified. [$sqlWhere]", -1);
            }
            // 
            $query = "DELETE FROM {$tableName} WHERE " . $sqlWhere;
            // 
            return($this->Execute($query, $arrValueWhere));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // Delete()

// SELECT: 
    // Fetch a row: return a row as an array, if FALSE no more.
    // $resultType: MYSQL_ASSOC, MYSQL_NUM, and the default value of MYSQL_BOTH
    public function Fetch($resultType = MYSQL_ASSOC)
    {
        if(NULL == $this->m_resource)
        {
            throw new Exception("No query issued before fetch() or query doesn't have a result set.", -1);
        }
        // 
        $arrRow = mysql_fetch_array($this->m_resource, $resultType);
        // 
        if(is_array($arrRow))
        {
            // Got a row: 
            return($arrRow);
        }else if(false == $arrRow)
        {
            return(false);
        }else
        {
            throw new Exception("Internal error: Got unexpected value.", -1);
        }
    }   // Fetch()

    // Get a record from db: if nothing got, return false
    // $resultType: MYSQL_ASSOC, MYSQL_NUM, and the default value of MYSQL_BOTH
    // 
    public function GetRow($query, $arrField = NULL, $resultType = MYSQL_ASSOC)
    {
    	try
    	{
            if($this->Execute($query, $arrField) > 0)
	        {
	            if($arrRow = $this->Fetch($resultType))
	            {
	                return($arrRow);
	            }
	        }
	        return(false);
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // GetRow()

    // Get a list from db, if $pageSize = -1, means no page limit
    // Return: array of list, if null result return false
    // $resultType: MYSQL_ASSOC, MYSQL_NUM, and the default value of MYSQL_BOTH
    public function GetList($query, $offset = 0, $pageSize = -1, $arrField = NULL, $resultType = MYSQL_ASSOC)
    {
    	try
    	{
            // Handle page_size
	        if($pageSize < 0) $sqlLimit = "";
	        else $sqlLimit = " LIMIT {$offset}, {$pageSize}";
	        // 
	        if(($countRow = $this->Execute($query . $sqlLimit, $arrField)) > 0)
	        {
	            $arrList = array();
	            for ($i = 0; $i < $countRow; $i++)
	            {
	                if($arrRow = $this->Fetch($resultType))
	                {
	                	$arrList[$i] = $arrRow;
	                }
	            }
	            return($arrList);
	        }else return(false);
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // GetList()

    // return: 0: No ID geterated by last insert. >0: ID generated.
    public function GetLastInsertId()
    {
        if(NULL == $this->m_connection->GetConnection()) throw new Exception("Database has not been connected yet.", -1);
        if(false === ($insert_id = mysql_insert_id($this->m_connection->GetConnection())))
        {
        	throw new Exception("Database has not been connected yet.", -1);	// Lost connection.
        }
    	return($insert_id);
    }

// Database information: 
    // If table exists
    public function DoesTableExist($tableName)
    {
        try
        {
            // 
            $rec_table = explode('.', $tableName);
            $rec_count = count($rec_table);
            // 
            $table = "";
            $database = "";
            // 
            if($rec_count <= 1)
            {
                $table = $rec_table[0];
            }else if($rec_count > 2) 
            {
                return(false);
            }else 
            {
                $database   = $rec_table[0];
                $table      = $rec_table[1];
            }
            if(!empty($database)) 
            {
                $this->m_connection->UseDatabase($database);
            }
            // 
            $query = "SHOW TABLES LIKE '{$table}'";
            if(false == $this->GetRow($query))
            {
                return(false);
            }else
            {
                return(true);
            }
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }   // DoesTableExist()
}

/* =========================================================================== /
// Mysql: 
/============================================================================ */
class Mysql extends ConnectionMysql
{
// Private: 
    private $m_query         = NULL;
// Public: 
    // Constructor
    public function __construct($dbHost = NULL, $dbUser = NULL, $dbPassword = NULL, $dbDatabase = NULL, $dbPort = NULL, $isPersistent = false)
    {
        try
        {
            parent::__construct($dbHost, $dbUser, $dbPassword, $dbDatabase, $dbPort, $isPersistent);
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Destructor
    public function __destruct()
    {
        try
        {
            parent::__destruct();
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Overload Connect(): 
    public function Connect($dbHost, $dbUser, $dbPassword, $dbDatabase, $dbPort, $isPersistent)
    {
        try
        {
            parent::Connect($dbHost, $dbUser, $dbPassword, $dbDatabase, $dbPort, $isPersistent);
            //Initialize query object: 
            $this->m_query = new QueryMysql($this);
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

// Execute
    // Rows selected
    public function NumberOfRow()
    {
        try
        {
            return($this->m_query->NumberOfRow());
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
// Excute a command: 
    //  return: Affected number of rows
    public function Execute($query, $arrValue = NULL, $arrValue1 = NULL, $holderSimple = self::PLACE_HOLDER_SIMPLE)
    {
        try
        {
            return($this->m_query->Execute($query, $arrValue, $arrValue1, $holderSimple));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // 
    public function Insert($tableName, $arrField, $arrFieldUpdate = NULL)
    {
        try
        {
            return($this->m_query->Insert($tableName, $arrField, $arrFieldUpdate));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // 
    public function Update($tableName, $arrField, $sqlWhere = "0 = 1", $arrValueWhere = NULL)
    {
        try
        {
            return($this->m_query->Update($tableName, $arrField, $sqlWhere, $arrValueWhere));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // 
    public function Delete($tableName, $sqlWhere = "0 = 1", $arrValueWhere = NULL)
    {
        try
        {
            return($this->m_query->Delete($tableName, $sqlWhere, $arrValueWhere));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
    // Fetch a row: return a row as an array, if FALSE no more.
    public function Fetch($resultType = MYSQL_BOTH)
    {
        try
        {
            return($this->m_query->Fetch($resultType));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

    // Get a record from db: if nothing got, return false
    public function GetRow($query, $arrField = NULL, $resultType = MYSQL_BOTH)
    {
        try
        {
            return($this->m_query->GetRow($query, $arrField, $resultType));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

    // Get a list from db, if $pageSize = -1, means no page limit
    // Return: array of list, if null result return false
    public function GetList($query, $offset = 0, $pageSize = -1, $arrField = NULL, $resultType = MYSQL_BOTH)
    {
        try
        {
            return($this->m_query->GetList($query, $offset, $pageSize, $arrField, $resultType));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

    // return: 0: No ID geterated by last insert. >0: ID generated.
    public function GetLastInsertId()
    {
        try
        {
            return($this->m_query->GetLastInsertId());
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }

// Database information: 
    // If table exists
    public function DoesTableExist($tableName)
    {
        try
        {
            return($this->m_query->DoesTableExist($tableName));
        }catch(Exception $exception)
        {
            throw $exception;
        }
    }
}

?>
