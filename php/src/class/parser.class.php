<?php
/* =========================================================================== /
* @category     
* @package      parser.class.php
* @author       Stephen(Tie) Hu <stephen@softroute.net>
* @copyright    2004-2007 The Softroute Co. Ltd.
* @license      
* @version      CVS: $Id: parser.class.php,v 1.100 2007/04/03 16:15:58 kent Exp $
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
interface iParser
{
    public function __construct();
    public function __destruct();

    // String quotation escaping
    public function HtmlEncode($htmlString);
    public function HtmlDecode($encodedString);

    // Data validation: 
    public function IsUserIdValid($userId);
    public function IsPasswordValid($password);
    public function IsPhoneValid($phone, $usca = true);
    public function IsDomainValid($domain);
    public function IsEmailValid($eMail);

    // Data conversion: 
    public function RegulatePhone($phone, $usca = true);

    // Data generation: 
    public function GetRandomString($stringLength = 32);
    // 
    public function 
}
*/

class Parser
{
//------------------------------------------------------------------------------
// Member: 
//------------------------------------------------------------------------------
// Constant: 
    // Regular expression patten: 
    // http://www.virtuosimedia.com/dev/php/37-tested-php-perl-and-javascript-regular-expressions
    const REG_CC_MAJOR          = '/^(?:4[0-9]{12}(?:[0-9]{3})?|5[1-5][0-9]{14}|6011[0-9]{12}|622((12[6-9]|1[3-9][0-9])|([2-8][0-9][0-9])|(9(([0-1][0-9])|(2[0-5]))))[0-9]{10}|64[4-9][0-9]{13}|65[0-9]{14}|3(?:0[0-5]|[68][0-9])[0-9]{11}|3[47][0-9]{13})*$/';
    const REG_CC_AE             = '/^(3[47][0-9]{13})*$/';
    // 
    const REG_POST_CODE_US      = '/^([0-9]{5}(?:-[0-9]{4})?)*$/';
    const REG_POST_CODE_CA      = '/^([ABCEGHJKLMNPRSTVXY][0-9][A-Z]\s*[0-9][A-Z][0-9])*$/';
    const REG_POST_CODE_AU      = '/^((0[289][0-9]{2})|([1345689][0-9]{3})|(2[0-8][0-9]{2})|(290[0-9])|(291[0-4])|(7[0-4][0-9]{2})|(7[8-9][0-9]{2}))*$/';
    const REG_POST_CODE_UK      = '/^([A-Z]{1,2}[0-9][A-Z0-9]? [0-9][ABD-HJLNP-UW-Z]{2})*$/';
    // 
    const       REG_USER_ID         = '/^[[:alpha:]][[:alnum:]._]{2,63}$/i';
    const       REG_PASSWORD        = '/^[[:alnum:]~!@#$%\^&\*\-_+=|<>,.:;]{4,64}$/i';
    //  North American phone number: 
    const       REG_PHONE_USCA      = '/^([+]*|[0]*)([1]?)([(]?)([1-9][0-9]{2,2})([)]?)([0-9]{3,3})([-]?)([0-9]{4,4})([-]?[0-9]{1,8})?$/';
    const       REG_PHONE_USCA_REG  = '${2}${4}${6}${8}${9}';
    //  All phone number: 
    const       REG_PHONE_OTHER     = '/^([+0]*)(([0-9]|[.-]|[(]|[)]){3,32})$/';
    //  Email: tie.hu@hotmail.com
    const       REG_DOMAIN          = '/^(([[:alnum:]-]{1,64})[.])*([[:alnum:]-]{1,64})$/i';
    //  local-part@domain
    const       REG_EMAIL           = '/^([[:alnum:]!#$%&\'*+-\/=?\^_`{|}~]+)@(([[:alnum:]-]{1,64})[.])*([[:alnum:]-]{1,64})$/i';
    //  Date: mm/dd/yyyy
    const       REG_DATE1           = "/(0[1-9]|1[012])[- /.](0[1-9]|[12][0-9]|3[01])[- /.](19|20)\d\d/";
    //  Date: dd-mm-yyyy
    const       REG_DATE2           = "/(0[1-9]|[12][0-9]|3[01])[- /.](0[1-9]|1[012])[- /.](19|20)\d\d/";
// 
    public static $m_arrCountry     = array(
	'AF' => 'AFGHANISTAN',
	'AX' => 'ALAND ISLANDS',
	'AL' => 'ALBANIA',
	'DZ' => 'ALGERIA',
	'AS' => 'AMERICAN SAMOA',
	'AD' => 'ANDORRA',
	'AO' => 'ANGOLA',
	'AI' => 'ANGUILLA',
	'AQ' => 'ANTARCTICA',
	'AG' => 'ANTIGUA AND BARBUDA',
	'AR' => 'ARGENTINA',
	'AM' => 'ARMENIA',
	'AW' => 'ARUBA',
	'AU' => 'AUSTRALIA',
	'AT' => 'AUSTRIA',
	'AZ' => 'AZERBAIJAN',
	'BS' => 'BAHAMAS',
	'BH' => 'BAHRAIN',
	'BD' => 'BANGLADESH',
	'BB' => 'BARBADOS',
	'BY' => 'BELARUS',
	'BE' => 'BELGIUM',
	'BZ' => 'BELIZE',
	'BJ' => 'BENIN',
	'BM' => 'BERMUDA',
	'BT' => 'BHUTAN',
	'BO' => 'BOLIVIA',
	'BA' => 'BOSNIA AND HERZEGOVINA',
	'BW' => 'BOTSWANA',
	'BV' => 'BOUVET ISLAND',
	'BR' => 'BRAZIL',
	'IO' => 'BRITISH INDIAN OCEAN TERRITORY',
	'BN' => 'BRUNEI DARUSSALAM',
	'BG' => 'BULGARIA',
	'BF' => 'BURKINA FASO',
	'BI' => 'BURUNDI',
	'KH' => 'CAMBODIA',
	'CM' => 'CAMEROON',
	'CA' => 'CANADA',
	'CV' => 'CAPE VERDE',
	'KY' => 'CAYMAN ISLANDS',
	'CF' => 'CENTRAL AFRICAN REPUBLIC',
	'TD' => 'CHAD',
	'CL' => 'CHILE',
	'CN' => 'CHINA',
	'CX' => 'CHRISTMAS ISLAND',
	'CC' => 'COCOS (KEELING) ISLANDS',
	'CO' => 'COLOMBIA',
	'KM' => 'COMOROS',
	'CG' => 'CONGO',
	'CD' => 'CONGO, DEMOCRATIC REPUBLIC',
	'CK' => 'COOK ISLANDS',
	'CR' => 'COSTA RICA',
	'CI' => 'COTE D\'IVOIRE',
	'HR' => 'CROATIA',
	'CU' => 'CUBA',
	'CY' => 'CYPRUS',
	'CZ' => 'CZECH REPUBLIC',
	'DK' => 'DENMARK',
	'DJ' => 'DJIBOUTI',
	'DM' => 'DOMINICA',
	'DO' => 'DOMINICAN REPUBLIC',
	'EC' => 'ECUADOR',
	'EG' => 'EGYPT',
	'SV' => 'EL SALVADOR',
	'GQ' => 'EQUATORIAL GUINEA',
	'ER' => 'ERITREA',
	'EE' => 'ESTONIA',
	'ET' => 'ETHIOPIA',
	'FK' => 'FALKLAND ISLANDS (MALVINAS)',
	'FO' => 'FAROE ISLANDS',
	'FJ' => 'FIJI',
	'FI' => 'FINLAND',
	'FR' => 'FRANCE',
	'GF' => 'FRENCH GUIANA',
	'PF' => 'FRENCH POLYNESIA',
	'TF' => 'FRENCH SOUTHERN TERRITORIES',
	'GA' => 'GABON',
	'GM' => 'GAMBIA',
	'GE' => 'GEORGIA',
	'DE' => 'GERMANY',
	'GH' => 'GHANA',
	'GI' => 'GIBRALTAR',
	'GR' => 'GREECE',
	'GL' => 'GREENLAND',
	'GD' => 'GRENADA',
	'GP' => 'GUADELOUPE',
	'GU' => 'GUAM',
	'GT' => 'GUATEMALA',
	'GG' => 'GUERNSEY',
	'GN' => 'GUINEA',
	'GW' => 'GUINEA-BISSAU',
	'GY' => 'GUYANA',
	'HT' => 'HAITI',
	'HM' => 'HEARD ISLAND & MCDONALD ISLANDS',
	'VA' => 'HOLY SEE (VATICAN CITY STATE)',
	'HN' => 'HONDURAS',
	'HK' => 'HONG KONG',
	'HU' => 'HUNGARY',
	'IS' => 'ICELAND',
	'IN' => 'INDIA',
	'ID' => 'INDONESIA',
	'IR' => 'IRAN, ISLAMIC REPUBLIC OF',
	'IQ' => 'IRAQ',
	'IE' => 'IRELAND',
	'IM' => 'ISLE OF MAN',
	'IL' => 'ISRAEL',
	'IT' => 'ITALY',
	'JM' => 'JAMAICA',
	'JP' => 'JAPAN',
	'JE' => 'JERSEY',
	'JO' => 'JORDAN',
	'KZ' => 'KAZAKHSTAN',
	'KE' => 'KENYA',
	'KI' => 'KIRIBATI',
	'KR' => 'KOREA',
	'KW' => 'KUWAIT',
	'KG' => 'KYRGYZSTAN',
	'LA' => 'LAO PEOPLE\'S DEMOCRATIC REPUBLIC',
	'LV' => 'LATVIA',
	'LB' => 'LEBANON',
	'LS' => 'LESOTHO',
	'LR' => 'LIBERIA',
	'LY' => 'LIBYAN ARAB JAMAHIRIYA',
	'LI' => 'LIECHTENSTEIN',
	'LT' => 'LITHUANIA',
	'LU' => 'LUXEMBOURG',
	'MO' => 'MACAO',
	'MK' => 'MACEDONIA',
	'MG' => 'MADAGASCAR',
	'MW' => 'MALAWI',
	'MY' => 'MALAYSIA',
	'MV' => 'MALDIVES',
	'ML' => 'MALI',
	'MT' => 'MALTA',
	'MH' => 'MARSHALL ISLANDS',
	'MQ' => 'MARTINIQUE',
	'MR' => 'MAURITANIA',
	'MU' => 'MAURITIUS',
	'YT' => 'MAYOTTE',
	'MX' => 'MEXICO',
	'FM' => 'MICRONESIA, FEDERATED STATES OF',
	'MD' => 'MOLDOVA',
	'MC' => 'MONACO',
	'MN' => 'MONGOLIA',
	'ME' => 'MONTENEGRO',
	'MS' => 'MONTSERRAT',
	'MA' => 'MOROCCO',
	'MZ' => 'MOZAMBIQUE',
	'MM' => 'MYANMAR',
	'NA' => 'NAMIBIA',
	'NR' => 'NAURU',
	'NP' => 'NEPAL',
	'NL' => 'NETHERLANDS',
	'AN' => 'NETHERLANDS ANTILLES',
	'NC' => 'NEW CALEDONIA',
	'NZ' => 'NEW ZEALAND',
	'NI' => 'NICARAGUA',
	'NE' => 'NIGER',
	'NG' => 'NIGERIA',
	'NU' => 'NIUE',
	'NF' => 'NORFOLK ISLAND',
	'MP' => 'NORTHERN MARIANA ISLANDS',
	'NO' => 'NORWAY',
	'OM' => 'OMAN',
	'PK' => 'PAKISTAN',
	'PW' => 'PALAU',
	'PS' => 'PALESTINIAN TERRITORY, OCCUPIED',
	'PA' => 'PANAMA',
	'PG' => 'PAPUA NEW GUINEA',
	'PY' => 'PARAGUAY',
	'PE' => 'PERU',
	'PH' => 'PHILIPPINES',
	'PN' => 'PITCAIRN',
	'PL' => 'POLAND',
	'PT' => 'PORTUGAL',
	'PR' => 'PUERTO RICO',
	'QA' => 'QATAR',
	'RE' => 'REUNION',
	'RO' => 'ROMANIA',
	'RU' => 'RUSSIAN FEDERATION',
	'RW' => 'RWANDA',
	'BL' => 'SAINT BARTHELEMY',
	'SH' => 'SAINT HELENA',
	'KN' => 'SAINT KITTS AND NEVIS',
	'LC' => 'SAINT LUCIA',
	'MF' => 'SAINT MARTIN',
	'PM' => 'SAINT PIERRE AND MIQUELON',
	'VC' => 'SAINT VINCENT AND GRENADINES',
	'WS' => 'SAMOA',
	'SM' => 'SAN MARINO',
	'ST' => 'SAO TOME AND PRINCIPE',
	'SA' => 'SAUDI ARABIA',
	'SN' => 'SENEGAL',
	'RS' => 'SERBIA',
	'SC' => 'SEYCHELLES',
	'SL' => 'SIERRA LEONE',
	'SG' => 'SINGAPORE',
	'SK' => 'SLOVAKIA',
	'SI' => 'SLOVENIA',
	'SB' => 'SOLOMON ISLANDS',
	'SO' => 'SOMALIA',
	'ZA' => 'SOUTH AFRICA',
	'GS' => 'SOUTH GEORGIA AND SANDWICH ISL.',
	'ES' => 'SPAIN',
	'LK' => 'SRI LANKA',
	'SD' => 'SUDAN',
	'SR' => 'SURINAME',
	'SJ' => 'SVALBARD AND JAN MAYEN',
	'SZ' => 'SWAZILAND',
	'SE' => 'SWEDEN',
	'CH' => 'SWITZERLAND',
	'SY' => 'SYRIAN ARAB REPUBLIC',
	'TW' => 'TAIWAN',
	'TJ' => 'TAJIKISTAN',
	'TZ' => 'TANZANIA',
	'TH' => 'THAILAND',
	'TL' => 'TIMOR-LESTE',
	'TG' => 'TOGO',
	'TK' => 'TOKELAU',
	'TO' => 'TONGA',
	'TT' => 'TRINIDAD AND TOBAGO',
	'TN' => 'TUNISIA',
	'TR' => 'TURKEY',
	'TM' => 'TURKMENISTAN',
	'TC' => 'TURKS AND CAICOS ISLANDS',
	'TV' => 'TUVALU',
	'UG' => 'UGANDA',
	'UA' => 'UKRAINE',
	'AE' => 'UNITED ARAB EMIRATES',
	'GB' => 'UNITED KINGDOM',
	'US' => 'UNITED STATES',
	'UM' => 'UNITED STATES OUTLYING ISLANDS',
	'UY' => 'URUGUAY',
	'UZ' => 'UZBEKISTAN',
	'VU' => 'VANUATU',
	'VE' => 'VENEZUELA',
	'VN' => 'VIET NAM',
	'VG' => 'VIRGIN ISLANDS, BRITISH',
	'VI' => 'VIRGIN ISLANDS, U.S.',
	'WF' => 'WALLIS AND FUTUNA',
	'EH' => 'WESTERN SAHARA',
	'YE' => 'YEMEN',
	'ZM' => 'ZAMBIA',
	'ZW' => 'ZIMBABWE'
    );
    // 
    public static $m_arrStateUs     = array(
	'AL'=>'ALABAMA',
	'AK'=>'ALASKA',
	'AS'=>'AMERICAN SAMOA',
	'AZ'=>'ARIZONA',
	'AR'=>'ARKANSAS',
	'CA'=>'CALIFORNIA',
	'CO'=>'COLORADO',
	'CT'=>'CONNECTICUT',
	'DE'=>'DELAWARE',
	'DC'=>'DISTRICT OF COLUMBIA',
	'FM'=>'FEDERATED STATES OF MICRONESIA',
	'FL'=>'FLORIDA',
	'GA'=>'GEORGIA',
	'GU'=>'GUAM GU',
	'HI'=>'HAWAII',
	'ID'=>'IDAHO',
	'IL'=>'ILLINOIS',
	'IN'=>'INDIANA',
	'IA'=>'IOWA',
	'KS'=>'KANSAS',
	'KY'=>'KENTUCKY',
	'LA'=>'LOUISIANA',
	'ME'=>'MAINE',
	'MH'=>'MARSHALL ISLANDS',
	'MD'=>'MARYLAND',
	'MA'=>'MASSACHUSETTS',
	'MI'=>'MICHIGAN',
	'MN'=>'MINNESOTA',
	'MS'=>'MISSISSIPPI',
	'MO'=>'MISSOURI',
	'MT'=>'MONTANA',
	'NE'=>'NEBRASKA',
	'NV'=>'NEVADA',
	'NH'=>'NEW HAMPSHIRE',
	'NJ'=>'NEW JERSEY',
	'NM'=>'NEW MEXICO',
	'NY'=>'NEW YORK',
	'NC'=>'NORTH CAROLINA',
	'ND'=>'NORTH DAKOTA',
	'MP'=>'NORTHERN MARIANA ISLANDS',
	'OH'=>'OHIO',
	'OK'=>'OKLAHOMA',
	'OR'=>'OREGON',
	'PW'=>'PALAU',
	'PA'=>'PENNSYLVANIA',
	'PR'=>'PUERTO RICO',
	'RI'=>'RHODE ISLAND',
	'SC'=>'SOUTH CAROLINA',
	'SD'=>'SOUTH DAKOTA',
	'TN'=>'TENNESSEE',
	'TX'=>'TEXAS',
	'UT'=>'UTAH',
	'VT'=>'VERMONT',
	'VI'=>'VIRGIN ISLANDS',
	'VA'=>'VIRGINIA',
	'WA'=>'WASHINGTON',
	'WV'=>'WEST VIRGINIA',
	'WI'=>'WISCONSIN',
	'WY'=>'WYOMING',
	'AE'=>'ARMED FORCES AFRICA \ CANADA \ EUROPE \ MIDDLE EAST',
	'AA'=>'ARMED FORCES AMERICA (EXCEPT CANADA)',
	'AP'=>'ARMED FORCES PACIFIC'
    );
    // 
    public static $m_arrProvinceCa  = array(
    "BC" => "BRITISH COLUMBIA", 
    "ON" => "ONTARIO", 
    "NL" => "NEWFOUNDLAND AND LABRADOR", 
    "NS" => "NOVA SCOTIA", 
    "PE" => "PRINCE EDWARD ISLAND", 
    "NB" => "NEW BRUNSWICK", 
    "QC" => "QUEBEC", 
    "MB" => "MANITOBA", 
    "SK" => "SASKATCHEWAN", 
    "AB" => "ALBERTA", 
    "NT" => "NORTHWEST TERRITORIES", 
    "NU" => "NUNAVUT",
    "YT" => "YUKON TERRITORY"
    );
    
//------------------------------------------------------------------------------
// Public: 
//------------------------------------------------------------------------------
/*
    // Constructor
    public function __construct()
    {
        // Do nothing: 
    }
    // Destructor
    public function __destruct()
    {
        // Do nothing: 
    }
*/
    // 
    public static function Utf8ToAscii($string, $option = 'TRANSLIT') {
        return(iconv('UTF-8', "ASCII//{$option}", $string));
    }
    
    // 
    public static function HtmlEncode($htmlString) {
        return(htmlentities($htmlString));
    }
    
    // 
    public static function HtmlDecode($encodedString) {
        return(html_entity_decode($encodedString));
    }

    // 
    public static function IsCaProvince($province) {
        if(empty($province)) {
            return(false);
        }
        $prov       = trim($province);
        // Detect encoding: 
        if(mb_detect_encoding($province, 'UTF-8', true)) {
            $prov = self::Utf8ToAscii($prov);
        }
        // 
        $prov       = strtoupper($prov);
        $prov_len   = strlen($prov);
        foreach(self::$m_arrProvinceCa as $key => $value) {
            if($prov_len != 2) {
                if($value == $prov) {
                    return(true);
                }
            } else {
                if($key == $prov) {
                    return(true);
                }
            }
        }
        return(false);
    }
    
    // 
    public static function IsUsState($state) {
        if(empty($state)) {
            return(false);
        }
        $prov       = trim($state);
        // Detect encoding: 
        if(mb_detect_encoding($state, 'UTF-8', true)) {
            $prov = self::Utf8ToAscii($prov);
        }
        // 
        $prov       = strtoupper($prov);
        $prov_len   = strlen($prov);
        foreach(self::$m_arrStateUs as $key => $value) {
            if($prov_len != 2) {
                if($value == $prov) {
                    return(true);
                }
            } else {
                if($key == $prov) {
                    return(true);
                }
            }
        }
        return(false);
    }
    
    // 
    public static function IsUsPostCode($postCode) {
        // Check NULL: 
        if(empty($postCode))
        {
            return(false);
        }
    	// Only space quotation mark, question mark and brackets not permited: 
        return(preg_match(self::REG_POST_CODE_US, $postCode));
    }
    // 
    public static function IsCaPostCode($postCode) {
        // Check NULL: 
        if(empty($postCode))
        {
            return(false);
        }
    	// Only space quotation mark, question mark and brackets not permited: 
        return(preg_match(self::REG_POST_CODE_CA, $postCode));
    }
    
// Format checking: 
    // User ID: 
    public static function IsUserIdValid($userId)
    {
        // Check NULL: 
        if(empty($userId))
        {
            return(false);
        }

        // Case insensitive: 
        return(preg_match(self::REG_USER_ID, $userId));
    }

    // Password: 
    public static function IsPasswordValid($password)
    {
        // Check NULL: 
        if(empty($password))
        {
            return(false);
        }
    	// Only space quotation mark, question mark and brackets not permited: 
        return(preg_match(self::REG_PASSWORD, $password));
    }

    // Phone Number: +14162738888, +1(416)2738888, 0014162738888, (416)273-6688
    public static function IsPhoneValid($phone, $usca = true)
    {
        $phone = str_replace(' ', '', $phone);
        if($usca)
        {
            return(preg_match(self::REG_PHONE_USCA, $phone));
        }else
        {
            return(preg_match(self::REG_PHONE_OTHER, $phone));
        }
    }
    // 
    public static function RegulatePhone($phone, $usca = true)
    {
        $phone = str_replace(' ', '', $phone);
        // 
        if($usca)
        {
            return(preg_replace(self::REG_PHONE_USCA, self::REG_PHONE_USCA_REG, $phone));
        }else 
        {
            $phone = preg_replace('/([^[:digit:]])/', '', $phone);
            $phone = preg_replace('/^([0]*)/', '', $phone);
            return($phone);
        }
    }

    // Domain: 
    public static function IsDomainValid($domain)
    {
        if(strlen($domain) > 255) 
        {
            return(false);
        }
        return(preg_match(self::REG_DOMAIN, $domain));
    }
    // Email: 
    public static function IsEmailValid($eMail)
    {
        return(preg_match(self::REG_EMAIL, $eMail));
    }

// Random string generator: 
    public static function GetRandomString($stringLength = 32)
    {
    	$rand_num = mt_rand();
    	return(strtolower(substr(md5($rand_num), 0, $stringLength)));
    }
    
    public static function GetRandomNumber($min, $max)
    {
        return(mt_rand($min, $max));
    }
}

?>
