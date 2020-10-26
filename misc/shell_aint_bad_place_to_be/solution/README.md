# Shell Ain't Bad Place to Be writeup

## 問題文

```txt
jailbreakできますか？

`nc 203.0.113.0 5000`
```

## about

簡易的なchroot jailに閉じ込められる問題です．

問題へアクセスすると，

- 現在`/home/misc/jail`にいる
- flagは`/home/misc`にある
- `exit cd ls pwd chroot mkdir cat`のコマンドが使える

事がわかります．

しかし，`pwd`すると`/`にいる為，`cat ../flag.txt`等はできません．

いくつかコマンドが用意されている上，ソースコードも読めます．

`chroot(2)`が`cwd`を変更しない事を利用したchroot jailbreakです．

## writeup

ソースコードを読むと，`chroot`は`chroot(args[1])`しかしない事がわかる．

`man 2 chroot`を読むとわかる様に，`mkdir foo;chroot foo; cd ..`で`chroot jail`から逃げ出せてしまう．

よって，以下の順番でコマンドを送信するとflagが読める

 1. mkdir break(任意のディレクトリ名)
 2. chroot break
 3. cat ../flag.txt

## flag

`ISCCTF{chr00t_1s_n0t_ch4ng3_cwd}`
