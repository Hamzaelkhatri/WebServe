

<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>
        <?php
            echo HELLO;
        ?>
    </title>
</head>
<body>
<form action="upload.php" method="post" enctype="multipart/form-data">
    Select image to upload:
    <input type="file" name="uploaded_file" id="uploaded_file">
    <input type="submit" value="Upload Image" name="submit">
</form>
</body>
</html>

