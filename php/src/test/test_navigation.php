<?php
    require_once("../class/navigation.class.php");
    // 
    $objNavBar = new NavigationBar("http://192.168.0.200/libphp/test/test_navigation.php");
    
    // Get HTTP variables: 
    $objNavBar->GetNavigationVariable();
    
    // Set total lines: 
    $objNavBar->SetLineTotal(21);
    
    // 
    $htmlPageRecord = $objNavBar->GetHtmlPageRecordInformation();
    echo "$htmlPageRecord<br>";
    // 
    $htmlPageSizeSelectorBar = $objNavBar->GetHtmlPageSizeSelectorBar(array(5, 10, 20, 50));
    echo "$htmlPageSizeSelectorBar<br>";
    // 
    $htmlNavigationBar = $objNavBar->GetHtmlPageNavigationBar();
    echo "$htmlNavigationBar<br>";
?>
