<?php

    //Database=localdb;Data Source=;User Id=azure;Password=6#vWHD_$

    // varibles used when connecting to the jokes database
    $serverName = "azurejokesdb.mysql.database.azure.com";
    $userName = "denniscanar";
    $password = "Pinina11";
    $db_name = "jokes";

    $server_connection = new mysqli($serverName, $userName, $password, $db_name);

    if($server_connection->connect_error)
    {
        echo "Could not connect to database";
    }


?>