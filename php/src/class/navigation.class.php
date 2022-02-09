<?php
/* =========================================================================== /
* @category     Page navigation
* @package      navigation.class.php
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2006 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: navigation.class.php,v 1.2 2006/09/13 20:13:33 stephen Exp $
* @link         
* @modification     (MM/DD/YY)  (Purpose)
   Stephen Hu       08/05/2006  First Generate
/============================================================================ */

/* =========================================================================== /
* Place includes, constant defines and $_GLOBAL settings here.
/============================================================================ */

/* =========================================================================== /
* Place class interface here: All public member variables and public functions.
/============================================================================ */
/*
interface iNavigationBar
{
    public function __construct($urlBase);
    public function __destruct();
    
    // Max lines per page: 
    //  Then SetLinePage() can not set a value more than this value.
    public function SetLinePageMax($nMaxLinePage);
    // Navigation: 
    //  Base URL: www.xxx.com
    public function SetUrlBase($urlBase);
    //  Return current url with all navigation prameters and other parameters.
    public function GetCurrentUrl();
    //  Lines per page to show: 
    public function SetLinePage($nLinePage);
    //  Total lines in the result set: 
    public function SetLineTotal($nLineTotal);
    //  Offset in the result set: 
    public function SetOffset($nOffset);
    
    // Application interfaces: 
    //  Get navigation variables: 
    public function GetNavigationVariable();
    //  Get query offset and limit: 
    public function GetOffset();
    public function GetLinePage();
    //  Get navigation UI components: 
    //  201 - 205 of 121074 record(s)
    public function GetLineFrom();
    public function GetLineTo();
    public function GetLineTotal();
    //  Display [5] [10] [20] records per page.
    public function GetUrlPageSize($nPageSize);
    //  [ <  36 37 38 39 40 41 42 43 44 45  > ]
    public function GetUrlFirstPage($sign = '[');
    public function GetUrlPreviousPage($sign = '<');
    public function GetUrlNextPage($sign = '>');
    public function GetUrlLastPage($sign = ']');
    public function GetUrlPage($nPage);
    public function GetHtmlPage($nPageCount = 10);
    //  Simplified interface: 
    public function GetHtmlPageRecordInformation();
    public function GetHtmlPageSizeSelectorBar($arrPageSize);
    public function GetHtmlPageNavigationBar($pageCount = 10, $signFirst = '[', $singPrevious = '<', $signNext = '>', $signLast = ']');
}
*/

/* =========================================================================== /
// AntiFraud: System Interface
/============================================================================ */
/*
// Constants: 
define("self::NAV_VAR_LINE_PAGE", "nav_npg");
define("self::NAV_VAR_OFFSET",    "nav_off");
define("self::NAV_MAX_PAGE_LINE", 100);
define("self::NAV_DEF_PAGE_LINE", 10);
*/

// 
class NavigationBar //implements iNavigationBar
{
//------------------------------------------------------------------------------
// Member: 
//------------------------------------------------------------------------------
    // Constants: 
    const   NAV_VAR_LINE_PAGE   = "nav_npg";
    const   NAV_VAR_OFFSET      = "nav_off";
    const   NAV_MAX_PAGE_LINE   = 100;
    const   NAV_DEF_PAGE_LINE   = 10;
    // Members: 
    private $m_nLinePageMax = self::NAV_MAX_PAGE_LINE;  // If you want set lines of a page more than this, it will only take this.
    // URL: 
    private $m_urlBase;                                 // Basic navigation URL.
    // 
    private $m_nLinePage    = self::NAV_DEF_PAGE_LINE;  // Lines per page.
    private $m_nLineTotal   = 0;                        // Total lines.
    private $m_nOffset      = 0;                        // Offset.
    private $m_nPage        = 0;                        // Total pages.
//------------------------------------------------------------------------------
// Private: 
//------------------------------------------------------------------------------
    // Prepare for parameters: abc.com/abc.php -> abc.com/abc.php? or abc.com/abc.php?p1=2 -> abc.com/abc.php?p1=2&
    private function FixUrl($url)
    {
        switch(substr($url, -1, 1))
        {
            case '?': 
            case '&': 
                return($url);
            default: 
                if(strpos($url, '?')) return($url . '&');
                else return($url . '?');
        }
    }
//------------------------------------------------------------------------------
// Public: 
//------------------------------------------------------------------------------
    public function __construct($urlBase) 
    {
        try
        {
            $this->SetUrlBase($urlBase);
        }catch(Exception $exception) 
        {
            throw $exception;
        }
    }
    // 
    public function __destruct()
    {
    }

    //  Max lines per page: 
    public function SetLinePageMax($nMaxLinePage)
    {
        if(!isset($nMaxLinePage)) throw new Exception("nMaxLinePage is NULL.", -1);
        $this->m_nLinePageMax = $nMaxLinePage;
    }
    
    //  Base URL: 
    public function SetUrlBase($urlBase)
    {
        if(!isset($urlBase)) throw new Exception("urlBase is NULL.", -1);
        $this->m_urlBase = $this->FixUrl($urlBase);
    }
    
    //  Lines per page: 
    public function SetLinePage($nLinePage)
    {
        if(!isset($nLinePage)) throw new Exception("nLinePage is NULL.", -1);
        if($nLinePage > $this->m_nLinePageMax) $this->m_nLinePage = $this->m_nLinePageMax;
        else $this->m_nLinePage = $nLinePage;
        $this->m_nPage = ceil($this->m_nLineTotal/$this->m_nLinePage);
    }
    
    //  Total lines in the result set: 
    public function SetLineTotal($nLineTotal)
    {
        if(!isset($nLineTotal)) throw new Exception("nLineTotal is NULL.", -1);
        $this->m_nLineTotal = $nLineTotal;
        $this->m_nPage = ceil($this->m_nLineTotal/$this->m_nLinePage);
    }
    
    //  Offset in the result set: 
    public function SetOffset($nOffset)
    {
        if(!isset($nOffset)) throw new Exception("nOffset is NULL.", -1);
        $this->m_nOffset = $nOffset;
    }
    
    //  Get navigation variables: 
    public function GetNavigationVariable()
    {
        // Lines per page: 
        if(isset($_GET[self::NAV_VAR_LINE_PAGE])) 
        {
            $this->SetLinePage((int)$_GET[self::NAV_VAR_LINE_PAGE]);
        }
        // Current page number: 
        if(isset($_GET[self::NAV_VAR_OFFSET]))
        {
            $this->SetOffset((int)$_GET[self::NAV_VAR_OFFSET]);
        }
        // Update URL parameters: 
        if(isset($_GET))
        {
            $urlParameter = "";
            foreach($_GET as $key => $value)
            {
                if(self::NAV_VAR_LINE_PAGE == $key) continue;
                if(self::NAV_VAR_OFFSET == $key) continue;
                $urlParameter .= "{$key}=" . urlencode($value) . "&";
            }
            // Update base URL: 
            $this->m_urlBase = $this->FixUrl($this->m_urlBase . $urlParameter);
        }
    }
    
    //  Get query offset and limit: 
    public function GetOffset()
    {
        return($this->m_nOffset);
    }
    // 
    public function GetLinePage()
    {
        return($this->m_nLinePage);
    }
    
    //  Navigation: 
    //  Page Record Information: 
    public function GetLineFrom()
    {
        if($this->m_nLineTotal > 0) return($this->m_nOffset + 1);
        else return(0);
    }
    // 
    public function GetLineTo()
    {
        if($this->m_nOffset + $this->m_nLinePage > $this->m_nLineTotal) return($this->m_nLineTotal);
        return($this->m_nOffset + $this->m_nLinePage);
    }
    // 
    public function GetLineTotal()
    {
        return($this->m_nLineTotal);
    }
    // 501 - 520 of 121074 record(s)
    public function GetHtmlPageRecordInformation()
    {
        $nLineFrom  = $this->GetLineFrom();
        $nLineTo    = $this->GetLineTo();
        $nLineTotal = $this->GetLineTotal();
        // 
        return("{$nLineFrom} - {$nLineTo} of {$nLineTotal} record(s)");
    }

    //  Page size selector: 
    public function GetUrlPageSize($nPageSize) 
    {
        if(!isset($nPageSize)) throw new Exception("Invalid parameter: nPageSize.", -1);
        return("<a href='{$this->m_urlBase}" . self::NAV_VAR_LINE_PAGE . "={$nPageSize}&" . self::NAV_VAR_OFFSET . "={$this->m_nOffset}'>{$nPageSize}</a>");
    }
    //  Display [5] [10] [20] records per page.
    public function GetHtmlPageSizeSelectorBar($arrPageSize)
    {
        if(!is_array($arrPageSize)) throw new Exception("arrPageSize is NULL.", -1);
        if(count($arrPageSize) < 1) throw new Exception("Page size array is empty, at least need one page size.", -1);
        // 
        $htmlPageSizeSelector = "Display ";
        // 
        foreach($arrPageSize as $nPageSize)
        {
            if($nPageSize == $this->m_nLinePage) $htmlPageSizeSelector .= "[{$nPageSize}] ";
            else $htmlPageSizeSelector .= "[" . $this->GetUrlPageSize($nPageSize) . "] ";
        }
        // 
        $htmlPageSizeSelector .= "records per page";
        return($htmlPageSizeSelector);
    }

    //  |< <  36 37 38 39 40 41 42 43 44 45  > >|
    public function GetUrlFirstPage($sign = '[')
    {
        if(!isset($sign)) $sign = '[';
        $url = "";
        if($this->m_nOffset <= 0)
        {
            $url = $sign;
        }else
        {
            $url = "<a href='{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "=0&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}'> {$sign} </a>";
        }
        return($url);
    }
    // 
    public function GetUrlPreviousPage($sign = '<')
    {
        if(!isset($sign)) $sign = '<';
        $url = "";
        if($this->m_nOffset <= 0)
        {
            $url = $sign;
        }else
        {
            if(($nPreviousOff = $this->m_nOffset - $this->m_nLinePage) < 0) $nPreviousOff = 0;
            $url = "<a href='{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "=$nPreviousOff&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}'> {$sign} </a>";
        }
        return($url);
    }
    // 
    public function GetUrlNextPage($sign = '>') 
    {
        if(!isset($sign)) $sign = '>';
        // 
        $url = "";
        $nMaxOff = ($this->m_nPage - 1) * $this->m_nLinePage;
        if($this->m_nOffset >= $nMaxOff)
        {
            $url = $sign;
        }else
        {
            //$nNextOff = $this->m_nOffset + $this->m_nLinePage;
            if(($nNextOff = $this->m_nOffset + $this->m_nLinePage) > $this->m_nLineTotal)
            {
                $nNextOff = $this->m_nLineTotal - ($this->m_nLineTotal%$this->m_nLinePage);
            }
            $url = "<a href='{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "={$nNextOff}&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}'> {$sign} </a>";
        }
        return($url);
    }
    // 
    public function GetUrlLastPage($sign = ']')
    {
        if(!isset($sign)) $sign = ']';
        // 
        $url = "";
        $nMaxOff = ($this->m_nPage - 1) * $this->m_nLinePage;
        if($this->m_nOffset >= $nMaxOff)
        {
            $url = $sign;
        }else
        {
            $url = "<a href='{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "={$nMaxOff}&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}'> {$sign} </a>";
        }
        return($url);
    }
    
    // 
    public function GetCurrentUrl()
    {
        return("{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "={$this->m_nOffset}&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}");
    }
    
    // 
    public function GetUrlPage($nPage)
    {
        if(!isset($nPage)) throw new Exception("Invalid parameter: [$nPage].", -1);
        // 
        $url = "";
        $nOffset = ($nPage - 1) * $this->m_nLinePage;
        if($nOffset >= $this->m_nLineTotal) return(false);
        if($nOffset == $this->m_nOffset) return("$nPage");
        else $url = "<a href='{$this->m_urlBase}" . self::NAV_VAR_OFFSET . "=$nOffset&" . self::NAV_VAR_LINE_PAGE . "={$this->m_nLinePage}'>{$nPage}</a>";
        return($url);
    }
    // 
    public function GetHtmlPage($nPageCount = 10)
    {
        if(!isset($nPageCount)) throw new Exception("Invalid parameter", -1);
        // Calculate page range: 
        if($this->m_nOffset - $this->m_nLinePage * ceil($nPageCount/2) >= 0)
        {
            $nOffBeg = $this->m_nOffset - $this->m_nLinePage * ceil($nPageCount/2);
            $nPageBeg = floor($nOffBeg / $this->m_nLinePage) + 1;
        }else
        {
            $nOffBeg = 0;
            $nPageBeg = 1;
        }
        // 
        $url = "";
        // 
        for($i = $nPageBeg; $i < $nPageBeg + $nPageCount; $i++)
        {
            if(false == ($urlPage = $this->GetUrlPage($i))) break;
            $url .= $urlPage . ' ';
        }
        return($url);
    }
    
    // 
    public function GetHtmlPageNavigationBar($pageCount = 10, $signFirst = '[', $singPrevious = '<', $signNext = '>', $signLast = ']')
    {
        $urlFirstPage       = $this->GetUrlFirstPage($signFirst);
        $urlPreviousPage    = $this->GetUrlPreviousPage($singPrevious);
        $urlNextPage        = $this->GetUrlNextPage($signNext);
        $urlLastPage        = $this->GetUrlLastPage($signLast);
        $htmlPage           = $this->GetHtmlPage($pageCount);
        $url = "{$urlFirstPage} {$urlPreviousPage} {$htmlPage} {$urlNextPage} {$urlLastPage}";
        return($url);
    }
}

?>
