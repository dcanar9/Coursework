<?php

    // will connect us to the database using previous php written
    include "db_connect.php";

    //Start a session to use session variable
    session_start();

    // so we can see any php errors
    error_reporting(E_ALL);
    ini_set("display_errors", 1);

    // will see if the user has logged in through session variable
    if (!$_SESSION["userid"]) {
        echo "Only authorized may add jokes to our webiste. Please Login <a href='jokesdb/google_login.php'>Here</a>.<br>";
        exit; // end program
    }
    else    // if the user has logged in we will extract the session variables
    {
        $userID = $_SESSION["userid"];
    }


    // getting the user in put from the POST request and then adding slashes if any quotation marks need to be escaped
    $question = addSlashes($_POST["question"]);
    $answer = addSlashes($_POST["answer"]);

    // Create an insert statement so we can add the question into our DB
    //$insertQuery = "INSERT INTO JokesTable (ID, Question, Answer, Users_ID) VALUES ( NULL, '$question', '$answer', '$userID' )";
    $insertQuery = "INSERT INTO jokestable (ID, Question, Answer, gID) VALUES ( NULL, ?, ?, ? )";

    $preparedQuery = $server_connection->prepare($insertQuery);             // prepare search query
    $preparedQuery->bind_param("ssi", $question, $answer, $userID);      // syntax check user input (let object know passing string, string, integer)
    $result = $preparedQuery->execute();                                              // execute insert query
    
    
    // will perform the insert into the DB or will diplay an error
    //$result = $server_connection->query($insertQuery) or die(mysqli_error($server_connection));

    // will print a message on success or failure
    if ($result) 
    {
        echo "<h2>Joke Added Succesfully, Thank you {$_SESSION['username']}</h2><br>";
        //echo "<p>Click <a href='index.php'>here</a> to return home</p><br>";
    }
    else 
    {
        echo "<h2>An error occured when adding your joke :(</h2><br>";
    }

    echo "<a href ='index.php'>Return Home</a>";
?>