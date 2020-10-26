# The Full Bug writeup

## 問題文

デバッグできん

## about

Rustバイナリです．

想定解はアンチデバッグを潰してレジスタを書き換える方法ですが，これもflagの出力部分を読めば解けるのでmediumです．

## writeup

そのままだとgdb経由で実行できません．

`dbg.main`を読むと，最初の方で`detect`と名前にある関数を呼び出しています．

`detect`では`ptrace`を使ってデバッガの検知を行っているので，この呼び出しをnopで潰します．

```txt
│           0x0000a257      e8b4050000     call sym The_Full_Bug::detect::h18a724201806bd04 ; main.rs:6
```

```xxd
2597 0000a240: c366 2e0f 1f84 0000 0000 000f 1f44 0000  .f...........D..
2598 0000a250: 4881 ec48 0200 00e8 b405 0000 eb0f 488b  H..H..........H.
                                ↑ここ
```

0x90(nop)で潰す

```xxd
2597 0000a240: c366 2e0f 1f84 0000 0000 000f 1f44 0000  .f...........D..
2598 0000a250: 4881 ec48 0200 0090 9090 9090 eb0f 488b  H..H..........H.
```

また，mainでは，他にも`check_flag`と名前にある関数を呼んでいます．

名前からこの関数がflagのチェックをしていると予測できるので，ここにbreakpointを置きます．

```txt
gdb-peda$ b check_flag
Breakpoint 1 at 0xa76e: file src/main.rs, line 22.
```

また，`main`を読むと，`check_flag`の戻り値(rax)によって分岐している部分があるのがわかります．

適当な文字列を入力した際は戻り値が0になっているので，これをgdbを使って1に書き換えます．

gdbで適当な文字列を入力し，`check_flag`から`ret`する直前に`set $rax=1`します．

```txt
gdb-peda$
[----------------------------------registers-----------------------------------]
RAX: 0x0
RBX: 0x7fffff7ff000
RCX: 0x4
RDX: 0x5555555edb10 --> 0x5555555ee380 --> 0x5555555efe60 --> 0x0
RSI: 0x5555555ecaf0 --> 0x5555555edb10 --> 0x5555555ee380 --> 0x5555555efe60 --> 0x0
RDI: 0x5555555ce016 --> 0x1000400020004
RBP: 0x1
RSP: 0x7fffffffce50 --> 0x5555555cec00 --> 0x65676f68 ('hoge')
RIP: 0x55555555e7fd (<The_Full_Bug::check_flag+157>:    add    rsp,0x78)
R8 : 0x3
R9 : 0x78bffff
R10: 0x7ed8320b
R11: 0x219c91a9
R12: 0x1
R13: 0x7fffff7fe000
R14: 0x5555555c8e28 --> 0x555555568a80 (<core::ptr::drop_in_place>:     sub    rsp,0x10)
R15: 0x7fffffffd308 --> 0x55555555e250 (<The_Full_Bug::main>:   sub    rsp,0x248)
EFLAGS: 0x246 (carry PARITY adjust ZERO sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x55555555e7f4 <The_Full_Bug::check_flag+148>:       mov    al,BYTE PTR [rsp+0x7]
   0x55555555e7f8 <The_Full_Bug::check_flag+152>:       and    al,0x1
   0x55555555e7fa <The_Full_Bug::check_flag+154>:       movzx  eax,al
=> 0x55555555e7fd <The_Full_Bug::check_flag+157>:       add    rsp,0x78
   0x55555555e801 <The_Full_Bug::check_flag+161>:       ret
   0x55555555e802 <The_Full_Bug::check_flag+162>:       mov    QWORD PTR [rsp+0x48],rax
   0x55555555e807 <The_Full_Bug::check_flag+167>:       mov    DWORD PTR [rsp+0x50],edx
   0x55555555e80b <The_Full_Bug::check_flag+171>:
    jmp    0x55555555e7e7 <The_Full_Bug::check_flag+135>:           jmp    0x55555555e7e7 <The_Full_Bug::check_flag+135>
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffce50 --> 0x5555555cec00 --> 0x65676f68 ('hoge')
0008| 0x7fffffffce58 --> 0x5555555ecaf0 --> 0x5555555edb10 --> 0x5555555ee380 --> 0x5555555efe60 --> 0x0
0016| 0x7fffffffce60 --> 0x40 ('@')
0024| 0x7fffffffce68 --> 0x40 ('@')
0032| 0x7fffffffce70 --> 0x2005555555cec00
0040| 0x7fffffffce78 --> 0x5555555cec00 --> 0x65676f68 ('hoge')
0048| 0x7fffffffce80 --> 0x4
0056| 0x7fffffffce88 --> 0x5555555cec00 --> 0x65676f68 ('hoge')
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
0x000055555555e7fd      24      }
gdb-peda$ set $rax=1
gdb-peda$ ni
[----------------------------------registers-----------------------------------]
RAX: 0x1
RBX: 0x7fffff7ff000
RCX: 0x4
RDX: 0x5555555edb10 --> 0x5555555ee380 --> 0x5555555efe60 --> 0x0
RSI: 0x5555555ecaf0 --> 0x5555555edb10 --> 0x5555555ee380 --> 0x5555555efe60 --> 0x0
RDI: 0x5555555ce016 --> 0x1000400020004
RBP: 0x1
RSP: 0x7fffffffcec8 --> 0x55555555e3eb (<The_Full_Bug::main+411>:       mov    BYTE PTR [rsp+0x5f],al)
RIP: 0x55555555e801 (<The_Full_Bug::check_flag+161>:    ret)
R8 : 0x3
R9 : 0x78bffff
R10: 0x7ed8320b
R11: 0x219c91a9
R12: 0x1
R13: 0x7fffff7fe000
R14: 0x5555555c8e28 --> 0x555555568a80 (<core::ptr::drop_in_place>:     sub    rsp,0x10)
R15: 0x7fffffffd308 --> 0x55555555e250 (<The_Full_Bug::main>:   sub    rsp,0x248)
EFLAGS: 0x202 (carry parity adjust zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x55555555e7f8 <The_Full_Bug::check_flag+152>:       and    al,0x1
   0x55555555e7fa <The_Full_Bug::check_flag+154>:       movzx  eax,al
   0x55555555e7fd <The_Full_Bug::check_flag+157>:       add    rsp,0x78
=> 0x55555555e801 <The_Full_Bug::check_flag+161>:       ret
   0x55555555e802 <The_Full_Bug::check_flag+162>:       mov    QWORD PTR [rsp+0x48],rax
   0x55555555e807 <The_Full_Bug::check_flag+167>:       mov    DWORD PTR [rsp+0x50],edx
   0x55555555e80b <The_Full_Bug::check_flag+171>:
    jmp    0x55555555e7e7 <The_Full_Bug::check_flag+135>:           jmp    0x55555555e7e7 <The_Full_Bug::check_flag+135>
   0x55555555e80d:      nop    DWORD PTR [rax]
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffcec8 --> 0x55555555e3eb (<The_Full_Bug::main+411>:      mov    BYTE PTR [rsp+0x5f],al)
0008| 0x7fffffffced0 --> 0x0
0016| 0x7fffffffced8 --> 0x0
0024| 0x7fffffffcee0 --> 0x0
0032| 0x7fffffffcee8 --> 0x0
0040| 0x7fffffffcef0 --> 0x0
0048| 0x7fffffffcef8 --> 0x0
0056| 0x7fffffffcf00 --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
0x000055555555e801 in The_Full_Bug::check_flag (input=...) at src/main.rs:24
24      }
gdb-peda$ c
Continuing.
ISCCTF{ptrace_c4n_4ls0_b3_u53d_t0_d3t3ct_d3bugg1ng}
[Inferior 1 (process 1133360) exited normally]
Warning: not running
gdb-peda$
```

## flag

`ISCCTF{ptrace_c4n_4ls0_b3_u53d_t0_d3t3ct_d3bugg1ng}`
