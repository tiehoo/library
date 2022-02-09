<?php
    try {
        mb_internal_encoding('UTF-8');
        // Include: 
        require_once("../class/parser.class.php");
        // 
        $province = "Ontario";
        $province = "QC";
        $province = "MW";
        $province = "Québec";
        if(Parser::IsCaProvince($province)) {
            echo("$province is a canadian province.\n");
        } else {
            echo("$province is not a canadian province.\n");
        }
        $province = "FLORIDA";
        $province = "FL";
        if(Parser::IsUsState($province)) {
            echo("$province is a US state.\n");
        } else {
            echo("$province is not a US state.\n");
        }
        // 
        $post_code = '370108';
        $post_code = '33010';
        if(Parser::IsUsPostCode($post_code)) {
            echo("$post_code is a US post code.\n");
        } else {
            echo("$post_code is not a US post code.\n");
        }
        $post_code = 'M2N6Z7';
        $post_code = 'M2N 6Z7';
        $post_code = 'M2N  6Z7';
        if(Parser::IsCaPostCode($post_code)) {
            echo("$post_code is a CA post code.\n");
        } else {
            echo("$post_code is not a CA post code.\n");
        }
die();
        
        // 2. HTML: Encode and show it on HTML page.
        $html_string            = "I'll \"walk\" the <b>dog</b> now";
        echo("{$html_string}\n");
        $html_string_encoded    = Parser::HtmlEncode($html_string);
        echo("{$html_string_encoded}\n");
        $html_string_decoded    = Parser::HtmlDecode($html_string_encoded);
        echo("{$html_string_decoded}\n\n");
        
        // 3. Validation: 
        $user_id = "a123";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }
        
        $user_id = "a1234";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }
        
        $user_id = "a123456789";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }

        $user_id = "a1234567890";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }
        
        $user_id = "1a2345";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }
        
        $user_id = "_a2345";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }

        $user_id = "a.2345";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }

        $user_id = "a_2345";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }

        $user_id = "a-2345";
        if(Parser::IsUserIdValid($user_id, 5, 10)) 
        {
            echo("{$user_id} is valid user ID.\n");
        }else
        {
            echo("{$user_id} is invalid user ID.\n");
        }

        // 
        $password = "1234";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }

        $password = "12345";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }

        $password = "a?345";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }
        
        $password = "a'345";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }

        $password = "a\"345";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }

        $password = "a 345";
        if(Parser::IsPasswordValid($password, 5, 10)) 
        {
            echo("{$password} is valid password.\n");
        }else
        {
            echo("{$password} is invalid password.\n");
        }

        // 
        $phone = "+1(416)273-8888";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "+1(416)273-8888-300";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "001(416)273-8888";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "001(416)273-8888-300";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "(1)416-273-8888";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "+1(416)(273)8888";
        if(Parser::IsPhoneValid($phone)) 
        {
            echo("{$phone} is valid US/CA phone.\n");
        }else
        {
            echo("{$phone} is invalid US/CA phone.\n");
        }
        // 
        $phone = "+00011(86)(731)8411023";
        if(Parser::IsPhoneValid($phone, false)) 
        {
            echo("{$phone} is valid phone number.\n");
        }else
        {
            echo("{$phone} is invalid phone number.\n");
        }
        
        // 
        $phone = "+1(416)273-8888-300";
        $phone_reg = Parser::RegulatePhone($phone);
        echo("{$phone} -> {$phone_reg}\n");
        // 
        $phone = "+00011(86)(731)8411023";
        $phone_reg = Parser::RegulatePhone($phone, false);
        echo("{$phone} -> {$phone_reg}\n");

        // 
        $domain = "www.vbuzzer.com";
        if(Parser::IsDomainValid($domain)) 
        {
            echo("{$domain} is valid domain.\n");
        }else
        {
            echo("{$domain} is invalid domain.\n");
        }
        $domain = "vbuzzer.bz";
        if(Parser::IsDomainValid($domain)) 
        {
            echo("{$domain} is valid domain.\n");
        }else
        {
            echo("{$domain} is invalid domain.\n");
        }
        $domain = "v-buzzer";
        if(Parser::IsDomainValid($domain)) 
        {
            echo("{$domain} is valid domain.\n");
        }else
        {
            echo("{$domain} is invalid domain.\n");
        }
        $domain = "vbuzzer_com";
        if(Parser::IsDomainValid($domain)) 
        {
            echo("{$domain} is valid domain.\n");
        }else
        {
            echo("{$domain} is invalid domain.\n");
        }
        
        $email = "vbuzzer_com@vbuzzer.com";
        if(Parser::IsEmailValid($email)) 
        {
            echo("{$email} is valid email.\n");
        }else
        {
            echo("{$email} is invalid email.\n");
        }
        $email = "vbuzzer_com_vbuzzer.com";
        if(Parser::IsEmailValid($email)) 
        {
            echo("{$email} is valid email.\n");
        }else
        {
            echo("{$email} is invalid email.\n");
        }
        $email = "@vbuzzer.com";
        if(Parser::IsEmailValid($email)) 
        {
            echo("{$email} is valid email.\n");
        }else
        {
            echo("{$email} is invalid email.\n");
        }
        $email = "someone@";
        if(Parser::IsEmailValid($email)) 
        {
            echo("{$email} is valid email.\n");
        }else 
        {
            echo("{$email} is invalid email.\n");
        }
        
        // 
        $string = Parser::GetRandomString();
        echo("Random: [$string]\n");
        
        // 
        $number = Parser::GetRandomNumber(0, 9);
        echo("Number: {$number}\n");
    }catch(Exception $exception) 
    {
        print($exception);
        echo("\n");
    }
?>
