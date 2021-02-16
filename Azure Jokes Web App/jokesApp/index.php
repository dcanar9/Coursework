<html>
<head>
    <title>Jokes App</title>
    <!-- Latest compiled and minified CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css">

    <!-- jQuery library -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>

    <!-- Latest compiled JavaScript -->
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
</head>
<body>

    <h1>Dennis' Jokes Application</h1> 
    <h2 style="text-align:right">
    <a href="jokesdb/google_login.php">Login Page</a>
    <br><br>
    <a href="https://dennisjokesapp.azurewebsites.net/jokesApp/jokesdb/google_login.php?logout=1">Log out</a> <br>
    </h2>

    <form class="form-horizontal" action="searchJokes.php" method="POST">
    <fieldset>

    <!-- Form Name -->
    <legend>Joke Search</legend>

    <!-- Search input-->
    <div class="form-group">
    <label class="col-md-4 control-label" for="keyword">Search Input</label>
    <div class="col-md-4">
        <input id="keyword" name="keyword" type="search" placeholder="e.g. knock-knock, why did the.., etc." class="form-control input-md" required="">
        <p class="help-block">Type what kind of joke you are looking for?</p>
    </div>
    </div>

    <!-- Button -->
    <div class="form-group">
    <label class="col-md-4 control-label" for="singlebutton"></label>
    <div class="col-md-4">
        <button id="singlebutton" name="singlebutton" class="btn btn-primary">Submit</button>
    </div>
    </div>

    </fieldset>
    </form>


    <hr>

    <!-- Use of PHP Blocks to show button only when a user is logged in-->
    <?php
    
        session_start();        // allow for session vars on this page
        if(isset($_SESSION['userid'])):
    
    ?>

    <form class="form-horizontal", action="addJoke.php" method="POST">
    <fieldset>

    <!-- Form Name -->
    <legend>Insert Joke</legend>

    <!-- Text input-->
    <div class="form-group">
    <label class="col-md-4 control-label" for="Question">Question</label>  
    <div class="col-md-5">
    <input id="Question" name="question" type="text" placeholder="e.g. Why did the chicken cross the road?" class="form-control input-md" required="">
    <span class="help-block">Add you joke question above</span>  
    </div>
    </div>

    <!-- Text input-->
    <div class="form-group">
    <label class="col-md-4 control-label" for="Answer">Answer</label>  
    <div class="col-md-5">
    <input id="Answer" name="answer" type="text" placeholder="e.g. To get to the other side." class="form-control input-md" required="">
    <span class="help-block">Add joke punchline above</span>  
    </div>
    </div>

    <!-- Button -->
    <div class="form-group">
    <label class="col-md-4 control-label" for="submit"></label>
    <div class="col-md-4">
        <button id="submit" name="submit" class="btn btn-primary">Add Joke</button>
    </div>
    </div>

    </fieldset>
    </form>

    <!-- if there is no user logged in then then this will appear instead of the add jokes form -->
    <?php else: ?>

    <div align="center">
    <h2>You Must Login to Add a Joke.</h2>
    <h3>Login <a href="jokesdb/google_login.php">Here</a></h3>
    <p>Google Account Required.</p>
    </div>

    <!--  identify the end of the if block -->
    <?php endif; ?>

    <?php
        // close connection
        $server_connection->close();
    ?>
    
</body>
</html>