<?php
function generate($secret)
{
    $header = json_encode(array(
        'alg' => 'sha256',
        'typ' => 'JWT'
    ));

    $payload = json_encode(array(
        'isAdmin' => '1'
    ));
    $signature = hash('sha256', $header . $payload . $secret);
    return trim(base64_encode($header), '=') . '.' .
        trim(base64_encode($payload), '=') . '.' .
        trim(base64_encode($signature), '=');
}
echo generate("48a939f9d0ef3778ee4fbbca6ffdd933");
