<?php
    //Add cookies to http request
    $cookie_name = "user";
    $cookie_value = "John Doe";
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
?>

<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="shortcut icon" href="data:image/x-icon;," type="image/x-icon">


    <title>
        <?php
            echo "Title with PHP";
        ?>
    </title>
</head>

<body>
    <h1>
        <?php
            echo "Welcome to Home Page of php";
            // Get HTTP COOKIES
            // echo "<br>";
            // echo "HTTP Cookies: ";
            ?>
    </h1>
</body>

</html>