<head>
    <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
    <link rel="stylesheet" href="/resources/demos/style.css">
    <script src="https://code.jquery.com/jquery-1.12.4.js"></script>
    <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
    <script>
    $( function() {
    $( "#accordion" ).accordion();
    } );
    </script>
</head>

<?php

    // so we can see any php errors
    error_reporting(E_ALL);
    ini_set("display_errors", 1);

    // this will add the php we have to create our data base connection 
    // and so we can use the mySQL object
    include "db_connect.php";
    
    // get user input and append wildcards
    $userSearch = "%" . $_POST["keyword"] . "%";


    // Let user know what they searched exactly but without wildcards
    echo "<h1>Search Results For: " . $_POST["keyword"] . "</h1>";

    // WHat will be given to the DB to return Data
    $searchQuery = "SELECT Question, Answer, Google_Name FROM jokestable LEFT JOIN googleusers ON jokestable.gID = googleusers.Google_ID WHERE Question LIKE ?";
    //$searchQuery = "SELECT * FROM jokestable";

    $preparedQuery = $server_connection->prepare($searchQuery);     // prepare query for check
    $preparedQuery->bind_param("s", $userSearch);                   // add user in put to query and syntax check
    $preparedQuery->execute();                                      // equivelant to ->query()
    $preparedQuery->store_result();                                // store results from search

    $preparedQuery->bind_result($Question, $Answer, $Username);   // bind results from seach to actual variables

    //echo $searchQuery;
    //$results = $server_connection->query($searchQuery);
?>


<!--Below we wrap the results we get from the search in the accordion container to the jquery formatting can apply to it-->
<div id="accordion">
    <?php
        
        while($preparedQuery->fetch())        // will update the variables we've binded
        {

            if ($Username == NULL) {
                $Username = "Jokes App Dev";
            }
            //echo "ID:" . $row["ID"] . "  " . " Question: " . $row["Question"] . "  " . "Answer: " . $row["Answer"] . "<br>"; 
            echo "<h3>$Question</h3>"; 
            echo "<div><p>$Answer<br><br>" . "~ Joke submitted by user: $Username<p></div>";
        }

    ?>
</div>

<h2><a href="index.php">Back to Home</a></h2>