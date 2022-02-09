<?php
/* =========================================================================== /
* @category     common class
* @package      dict.class.php
* @author       Stephen Hu<stephen@softroute.net>
* @copyright    2004-2006 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: dictionary.class.php,v 1.9 2007/01/12 23:03:22 kent Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
  Artur Levchenko    08/15/2006  Interface design 
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines and $_GLOBAL settings here.
/============================================================================ */
require_once(dirname(__FILE__) . "/mysql.class.php");

/* =========================================================================== /
    Class interface:
/============================================================================ */
/*
CREATE TABLE `screenshot`.`dictionary`
(
    `database`          VARCHAR(64) NOT NULL DEFAULT '', 
    `table`             VARCHAR(64) NOT NULL DEFAULT '', 
    `field`             VARCHAR(64) NOT NULL DEFAULT '', 
    `value`             VARCHAR(64) NOT NULL DEFAULT '', 
    `name`              VARCHAR(64) NOT NULL DEFAULT '', 
    `display_order`     INT(5) NOT NULL DEFAULT 0, 
    `memo`              VARCHAR(128), 
    PRIMARY KEY(`database`, `table`, `field`, `value`) 
) ENGINE=InnoDB;
*/
// Dictionary using table: dictionary

/*
interface iDictionary
{
    public function __construct(&$conDictionary, $defaultDatabase = NULL);
    public function __destruct();
    // 
    public function SetDefaultDatabase($defaultDatabase);
    // 
    public function GetName($table, $field, $id, $database = '%', $defaultDatabase = NULL);
    public function GetNameList($table, $field, $database = '%', $defaultDatabase = NULL);
    // Get HTML select <option/> list.
    public function GetHtmlSelectOption($table, $field, $database = '%', $idSelected = NULL, $bAddId = false, $defaultDatabase = NULL);
}
*/
/* =========================================================================== /
    Dictionary: retrieve data from system dictionaries
/============================================================================ */
class Dictionary //implements iDictionary 
{
//------------------------------------------------------------------------------
// Member: 
//------------------------------------------------------------------------------
    private $m_conDictionary    = NULL;
    private $m_defaultDatabase  = NULL;
    private $m_tableName        = 'dictionary';

//------------------------------------------------------------------------------
// Private: 
//------------------------------------------------------------------------------
    private function SetConnection(&$conDictionary)
    {
        if(!isset($conDictionary)) throw new Exception("NULL database connection.", -1);
        $this->m_conDictionary = $conDictionary;
    }

//------------------------------------------------------------------------------
// Public: 
//------------------------------------------------------------------------------
    public function __construct(&$conDictionary, $defaultDatabase = NULL)
    {
        try
        {
            $this->SetConnection(&$conDictionary);
            $this->SetDefaultDatabase($defaultDatabase);
        }catch (Exception $exception)
        {
            throw $exception;
        }
    }
    
    public function __destruct()
    {
    }
    
    // 
    public function SetDefaultDatabase($defaultDatabase)
    {
        if(!isset($defaultDatabase) || '' == $defaultDatabase)
        {
            throw new Exception("defaultDatabase is NULL.", -1);
        }
        $this->m_defaultDatabase = $defaultDatabase;
    }

    // 
    public function GetName($table, $field, $id, $database = '%', $defaultDatabase = NULL)
    {
        try
        {
	        // 
	        if(!isset($this->m_conDictionary) || !$this->m_conDictionary->IsConnected())
	        {
	            throw new Exception("No database connection.", -1);
	        }
	        // 
	        if(!isset($table, $field, $id, $database))
	        {
	            throw new Exception("Invalid parameter: table, field, id, database.", -1);
	        }
	        // 
	        if(isset($defaultDatabase)) $this->SetDefraultDatabase($defaultDatabase);
            // 
            $queryDictionary = new QueryMysql($this->m_conDictionary);
            // Table: 
            if(isset($this->m_defaultDatabase)) $sqlFrom = "FROM `{$this->m_defaultDatabase}`.`{$this->m_tableName}`";
            else $sqlFrom = "FROM `{$this->m_tableName}`";
            // Where: 
            $sqlWhere = "WHERE `database` LIKE '{$database}' AND `table` = '{$table}' AND `field` = '{$field}' AND `value` = '{$id}'";
            // 
            $sqlFinal   = "SELECT name {$sqlFrom} {$sqlWhere}";
            // 
            if(false != ($arrRow = $queryDictionary->GetRow($sqlFinal))) return($arrRow['name']);
            else return($id);
        }catch (Exception $exception)
        {
            throw $exception;
        }
    }
    
    public function GetNameList($table, $field, $database = '%', $defaultDatabase = NULL)
    {
        try
        {
	        // 
	        if(!isset($this->m_conDictionary) || !$this->m_conDictionary->IsConnected())
	        {
	            throw new Exception("No database connection.", -1);
	        }
	        // 
	        if(!isset($table, $field, $database))
	        {
	            throw new Exception("Invalid parameter: table, field, id, database.", -1);
	        }
	        // 
	        if(isset($defaultDatabase)) $this->SetDefraultDatabase($defaultDatabase);
            // 
            $queryDictionary = new QueryMysql($this->m_conDictionary);
            // Table: 
            if(isset($this->m_defaultDatabase)) $sqlFrom = "FROM `{$this->m_defaultDatabase}`.`{$this->m_tableName}`";
            else $sqlFrom = "FROM `{$this->m_tableName}`";
            // Where: 
            $sqlWhere   = "WHERE `database` LIKE '{$database}' AND `table` = '{$table}' AND `field` = '{$field}'";
            // 
            $sqlFinal   = "SELECT `value` id, name {$sqlFrom} {$sqlWhere} ORDER BY display_order";
            // 
            if(false != ($arrList = $queryDictionary->GetList($sqlFinal)))
            {
                $arrName = array();
                foreach($arrList as $key => $arrRow)
                {
                    $arrName[$arrRow['id']] = $arrRow['name'];
                }
                return($arrName);
            }else return(false);
        }catch (Exception $exception) 
        {
            throw $exception;
        }
    }
    
    public function GetHtmlSelectOption($table, $field, $database = '%', $idSelected = NULL, $bAddId = false, 
        $defaultDatabase = NULL) 
    {
        try
        {
            $htmlSelectOption = "";
            if(false != ($arrList = $this->GetNameList($table, $field, $database, $defaultDatabase))) 
            {
                foreach($arrList as $key => $value)
                {
                    if($bAddId)
                    {
                        if(isset($idSelected) && $key == $idSelected) $htmlSelectOption .= "<option value='{$key}' selected>{$key}-{$value}</option>";
                        else $htmlSelectOption .= "<option value='{$key}'>{$key}-{$value}</option>";
                    }else
                    {
                        if(isset($idSelected) && $key == $idSelected) $htmlSelectOption .= "<option value='{$key}' selected>{$value}</option>";
                        else $htmlSelectOption .= "<option value='{$key}'>{$value}</option>";
                    }
                }
            }
            // 
            return($htmlSelectOption);
        }catch (Exception $exception) 
        {
            throw $exception;
        }
    }
} // class Dictionary
?>
