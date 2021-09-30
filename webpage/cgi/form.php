<!DOCTYPE html>
<html>
<?php
session_start();
// print_r($_COOKIE);
if (isset($_COOKIE['name']) && isset($_COOKIE['email'])) {

    $str = "name=" . $_COOKIE['name'] . "&email=" . $_COOKIE['email'];
    parse_str($str, $output);
    echo "Welcome Mr " . $output['name'] . " To your session</br> Your Email is " . $output['email'];
    exit;
}
?>

<head>
    <title>Form</title>
</head>

<body>
    <form enctype="application/x-www-form-urlencoded">
        <input type="text" name="name" placeholder="Name">
        <input type="text" name="email" placeholder="Email">
         <input type="submit" value="Submit">
    </form>
    <?php
    // print_r($_COOKIE);
    if (isset($_COOKIE['name']) && isset($_COOKIE['email'])) {

        $str = "name=" . $_COOKIE['name'] . "&email=" . $_COOKIE['email'];
        parse_str($str, $output);
        echo "Welcome Mr " . $_COOKIE['name'] . " To your session</br> Your Email is " . $_COOKIE['email'];
        exit;
    }
    ?>
</body>

</html>
