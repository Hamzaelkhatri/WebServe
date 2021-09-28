<?php

session_start();
?>
<!DOCTYPE html>
<html>

<head>
    <title>Form</title>
</head>

<body>
    <form action="form.php" method="post" enctype="application/x-www-form-urlencoded" >
        <input type="text" name="email" placeholder="Email">
        <input type="text" name="phone" placeholder="Phone">
        <input type="submit" value="Submit">
    </form>
    <?php
    if (isset($_POST['email']) && isset($_POST['phone']))
    {
        $_SESSION['email'] = $_POST['email'];
        $_SESSION['phone'] = $_POST['phone'];
    }
    ?>
</body>

</html>