<?php
if (isset($_GET["source"])) {
    highlight_file(__FILE__);
    return;
}

$file = fopen('/var/www/app/private/secret.txt', 'r');
$secret = fgets($file);
fclose($file);

if (!isset($_COOKIE['token'])) {
    $flg = 1;
}
?>
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>crackjwt</title>
    <style>
        p {
            text-align: center;
        }
    </style>
</head>

<body>
    <p>
        <?php
        $parted = explode('.', $_COOKIE['token']);
        $signature = $parted[2];

        if (isset($flg) || hash('sha256', base64_decode($parted[0]) . base64_decode($parted[1]) . $secret) != base64_decode($signature)) {
            die('<script>alert("Invalid token!!");document.location="/"</script>');
        }

        $payload = json_decode(base64_decode($parted[1]), true);
        $isAdmin = $payload["isAdmin"];
        if ($isAdmin == 0) {
            echo '<img src="static/nyoronyoro.gif" alt="nyoronyoro"><p>You don\'t have the authority to read flag.<br>Please come back as an administrator.</p>';
        } else {
            require('/var/www/app/private/flag.php');
            echo '<img src="./static/congrats.gif" alt="congrats"><p>Congrats!!<br><strong>' . $flag . '</strong></p>';
        }
        ?>
    </p>
    <!-- <a href="?source">debug</a> -->
</body>

</html>