<?php
    try
    {
        // Include: 
        require_once("../class/mysql.class.php");
    // Connection: 
        // Connect: 
        $connection  = new ConnectionMysql("192.168.86.161", "tie", "pass", "test");
        //  
        if($connection->IsConnected())
        {
            echo("Database connected.\n");
        }else
        {
            echo("Database connection failed.\n");
        }
        // 
        $connection->SetAutoCommitOn();
        $connection->SetAutoCommitOff();
        $connection->StartTransaction();
        $connection->Commit();
        $connection->Rollback();
    // Query: 
        // 
        $query       = new QueryMysql($connection);
        // Create a table: 
        $connection->SetLogOn();
        if(!($query->DoesTableExist("test")))
        {
            $sql        = "CREATE TABLE test.test(f1 INT(9) UNSIGNED NOT NULL, f2 VARCHAR(128), f3 DATETIME, PRIMARY KEY(f1)) ENGINE=InnoDB";
           $query->Execute($sql);
            echo("Table created.\n");
        }
        // Transaction: 
        $connection->StartTransaction();
        // Insert: 
        $arrField = array("f1" => 1, "f2" => "Are you ok?", "f3" => QueryMysql::WrapExpression("STR_TO_DATE('08-May-08 04:13 pm', '%d-%b-%y %h:%i %p')"));
        try
        {
            $id = $query->Insert("test.test", $arrField);
            echo("Inserted: {$id}\n");
        }catch(Exception $exception)
        {
            if(1062 != $exception->getCode())   // Duplicated entry.
            {
                throw $exception;
            }
        }
        // Insert on duplicate: 
        $arrField['f2'] = "<b>Good</b>";
        $id = $query->Insert("test.test", $arrField, $arrField);
        echo("Merged: {$id}\n");
        // 
        $arrField['f1'] = 2;
        $arrField['f2'] = "I'm Stephen";
        $arrField['f3'] = QueryMysql::WrapExpression("NOW()");
        $id = $query->Insert("test.test", $arrField, $arrField);
        echo("Inserted: {$id}\n");

        // Update: 
        $arrUpdate['f2'] = "I'm ok now";
        $arrUpdate['f3'] = QueryMysql::WrapExpression("NOW()");
        $query->Update("test", $arrUpdate, "f1=?", array(1));
        // Get row: 
        if(false != ($arrRec = $query->GetRow("SELECT * FROM test.test WHERE f1 = ?", array(1))))
        {
            echo("Row:\n");
            print_r($arrRec);
            echo("\n");
        }else
        {
            echo("Can not find row.");
        }
        // Get list: 
        if(false != ($arrList = $query->GetList("SELECT * FROM test.test")))
        {
            echo("List:\n");
            print_r($arrList);
            echo("\n");
        }else
        {
            echo("Can not find row.");
        }
        // Delete: 
        $query->Delete("test", "f1 = ?", array(1));
        // Fetch one by one: 
        $sql = "SELECT * FROM test";
        if($query->Execute($sql) > 0)
        {
            if(false != ($arrRec = $query->Fetch()))
            {
                echo("Row:\n");
                print_r($arrRec);
                echo("\n");
            }
        }
        // Rollback: 
        $connection->Rollback();
        // 
        $connection->SetLogOff();
    }catch(Exception $exception)
    {
        echo "Exception caught.\n";
        print($exception); 
    }
?>
