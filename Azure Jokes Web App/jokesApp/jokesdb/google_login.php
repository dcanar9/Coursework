<html>
<head><link rel="stylesheet" href="/JokesApp.css"></head>
</html> 

<?php

    error_reporting(E_ALL);
    ini_set("display_errors", 1);
    
    session_start();

    // use google auto loader to load all the authentication php scripts
    require_once('vendor/autoload.php');

    // google api credentials
    $googleID = "764185549236-honk6kbq94jjbk0f0uok2hooac4avpd0.apps.googleusercontent.com";
    $googleSecret = "Ju_7Pl_r7vUT1K6OOie38p6G";

    // will redirect to our php page after confirmation
    $redirect_url = "https://dennisjokesapp.azurewebsites.net/jokesApp/jokesdb/google_login.php";

    // mySql credentials
    $srvr = "azurejokesdb.mysql.database.azure.com";
    $usr = "denniscanar";
    $pswrd = "Pinina11";
    $dbName = "jokes";

    // Create google client API object    
    $googleClient = new Google_Client();
    
    // setting api variables so we may use it 
    $googleClient->setClientId($googleID);
    $googleClient->setClientSecret($googleSecret);
    $googleClient->setRedirectUri($redirect_url);

    // set the scope of the information you are requesting
    $googleClient->addScope("email");
    $googleClient->addScope("profile");

    // use google client to login and get API service
    $service = new Google_Service_OAuth2($googleClient);

    // if logout form was submitted
    if (isset($_GET["logout"])) {
        // have api release user data
        $googleClient->revokeToken($_SESSION["access_token"]);      // remove 
        session_destroy();                                          // destroy session data
        header('Location: https://dennisjokesapp.azurewebsites.net/jokesApp/index.php');                              // send user back to home page
    }

    if (isset($_GET["code"])) {
        $googleClient->authenticate($_GET["code"]);
        $_SESSION["access_token"] = $googleClient->getAccessToken();
        header("Location: " . filter_var($redirect_url, FILTER_SANITIZE_URL));
        exit;
    }

    // case user is already logged in
    if (isset($_SESSION["access_token"]) && $_SESSION["access_token"]){
        $googleClient->setAccessToken($_SESSION["access_token"]);
    }

    // if the user commes in with no get request then they will be shown a login form
    else {
        $authUrl = $googleClient->createAuthUrl();
    }

    // if the auth url is set that means there was no get request
    if (isset($authUrl)) {
        // display the login url
        echo "<div align='center'>";
        echo "<h1 id='PageTitle'>Google Authentication Login</h1>";
        echo "<h3>You will need a Google Account to sign in.</h3>";
        echo "<a href={$authUrl}> Click Here </a>" ;
        echo "</div>";
    }

    // if none of the conditionas above are met the user must be logged in 
    // get user object so we can use its data
    
    else{
        echo "<div align='center'>";
        $user = $service->userinfo->get();  

        $db_conn = new mysqli($srvr, $usr, $pswrd, $dbName);

        // if db connection fails display message
        if ($db_conn->connect_error) {
            die("Error ( " . $db_conn->connect_errno . " )" . $db_conn->connect_error);
        }

        $result = $db_conn->query("SELECT COUNT(Google_ID) as usercount FROM googleusers WHERE Google_ID = $user->id");
        $user_count = $result->fetch_object()->usercount;

        // show user picture
        echo "<img src = {$user->picture} style='float:rigth;margin-top 33px;' />";

        // if there is a user already registered with this google account
        if($user_count){
            echo "<br><br>Welcome Back {$user->name}!";
            echo "<br>[<a href = '{$redirect_url}?logout=1'> Log Out </a>] [<a href = 'https://dennisjokesapp.azurewebsites.net/jokesApp/index.php'> Home </a>]";
        }
        // else register the user
        else {
            
            echo "<br> Hello {$user->name}, thanks for registering!";
            echo "<br>[<a href = '{$redirect_url}?logout=1'> Log Out </a>] [<a href = 'https://dennisjokesapp.azurewebsites.net/jokesApp/index.php'> Home </a>]";
            $insertQuery = $db_conn->prepare("INSERT INTO googleusers (Google_ID, Google_Name, Google_Email, Google_Link, Google_PicLink) VALUES (?, ?, ?, ?, ?)");
            $insertQuery->bind_param("issss", $user->id, $user->name, $user->email, $user->link, $user->picture);
            $insertQuery->execute();
            echo $db_conn->error;
        }

        // display data about the new user
        echo " <ul style='list-style-position:inside;' > <li>Username: {$user->name} </li> <li>User ID: {$user->id}</li> <li>User Email: {$user->email} </li></ul>";

        // set session vars
        $_SESSION["username"] = $user->name;
        $_SESSION["userid"] = $user->id;
        $_SESSION["useremail"] = $user->email;
        echo "</div>";
    }
    
?>