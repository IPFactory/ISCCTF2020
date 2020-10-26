<?php
if (isset($_GET["source"])) {
    highlight_file(__FILE__);
    return;
}

$file = fopen('/var/www/app/private/secret.txt', 'r');
$secret = fgets($file);
fclose($file);
if (!isset($_COOKIE['token'])) {
    setcookie('token', generate($secret));
}

function generate($secret)
{
    $header = json_encode(array(
        'alg' => 'sha256',
        'typ' => 'JWT'
    ));

    $payload = json_encode(array(
        'isAdmin' => '0'
    ));
    $signature = hash('sha256', $header . $payload . $secret);
    return trim(base64_encode($header), '=') . '.' .
        trim(base64_encode($payload), '=') . '.' .
        trim(base64_encode($signature), '=');
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

        .flgBtn {
            display: inline-block;
            padding: 0.3em 1em;
            text-decoration: none;
            color: #1DA1F2;
            border: solid 2px #1DA1F2;
            border-radius: 3px;
            transition: .5s;
        }

        .flgBtn:hover {
            background: #1DA1F2;
            color: white;
        }
    </style>
</head>

<body>
    <p>
        <img src="static/welcome.gif" alt="welcome"><br>
        <a href="flag.php" class="flgBtn">Get Flag</a>
    </p>
    <!-- <a href="?source">debug</a> -->
</body>

</html>