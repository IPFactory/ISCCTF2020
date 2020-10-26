# bookshop writeup

## 問題文

D☆V

## about

D☆Vはドキドキ☆ビジュアルの略です．

色々な雑誌が売っているというプログラムです．

雑誌の中にflagが混ざっていますが，高すぎて所持金が足りません．

一応バイナリにパッチを当てる事が想定解ですが，flagの出力部分を読む事でも解けるので`easy`です．

## writeup

`main`を読むと，最初に色々と初期化してるのがわかるので，`flag`の値段を代入している部分を探す．

```txt
│           0x000018ff      f30f1efa       endbr64
│           0x00001903      55             push rbp
│           0x00001904      4889e5         mov rbp, rsp
│           0x00001907      4881ecd00100.  sub rsp, 0x1d0
│           0x0000190e      64488b042528.  mov rax, qword fs:[0x28]
│           0x00001917      488945f8       mov qword [var_8h], rax
│           0x0000191b      31c0           xor eax, eax
│           0x0000191d      488d95e0feff.  lea rdx, [s]
│           0x00001924      b800000000     mov eax, 0
│           0x00001929      b91f000000     mov ecx, 0x1f
│           0x0000192e      4889d7         mov rdi, rdx
│           0x00001931      f348ab         rep stosq qword [rdi], rax
│           0x00001934      66c745d4d007   mov word [var_2ch], 0x7d0
│           0x0000193a      48b84b697261.  movabs rax, 0x61726172694b  ; 'Kirara'
│           0x00001944      ba00000000     mov edx, 0
│           0x00001949      48898530feff.  mov qword [var_1d0h], rax
│           0x00001950      48899538feff.  mov qword [var_1c8h], rdx
│           0x00001957      c78540feffff.  mov dword [var_1c0h], 0
│           0x00001961      c78544feffff.  mov dword [var_1bch], 0x17c
│           0x0000196b      48c78548feff.  mov qword [var_1b8h], 0x58414d ; 'MAX'
│           0x00001976      48c78550feff.  mov qword [var_1b0h], 0
│           0x00001981      c78558feffff.  mov dword [var_1a8h], 0
│           0x0000198b      c7855cfeffff.  mov dword [var_1a4h], 0x17c
│           0x00001995      48b843617261.  movabs rax, 0x7461726143    ; 'Carat'
│           0x0000199f      ba00000000     mov edx, 0
│           0x000019a4      48898560feff.  mov qword [var_1a0h], rax
│           0x000019ab      48899568feff.  mov qword [var_198h], rdx
│           0x000019b2      c78570feffff.  mov dword [var_190h], 0
│           0x000019bc      c78574feffff.  mov dword [var_18ch], 0x1ea
│           0x000019c6      48b8466f7277.  movabs rax, 0x64726177726f46 ; 'Forward'
│           0x000019d0      ba00000000     mov edx, 0
│           0x000019d5      48898578feff.  mov qword [var_188h], rax
│           0x000019dc      48899580feff.  mov qword [var_180h], rdx
│           0x000019e3      c78588feffff.  mov dword [var_178h], 0
│           0x000019ed      c7858cfeffff.  mov dword [var_174h], 0x24e
│           0x000019f7      48b84d697261.  movabs rax, 0x656c636172694d ; 'Miracle'
│           0x00001a01      ba00000000     mov edx, 0
│           0x00001a06      48898590feff.  mov qword [var_170h], rax
│           0x00001a0d      48899598feff.  mov qword [var_168h], rdx
│           0x00001a14      c785a0feffff.  mov dword [var_160h], 0
│           0x00001a1e      c785a4feffff.  mov dword [var_15ch], 0x16e
│           0x00001a28      48c785a8feff.  mov qword [var_158h], 0x67616c66 ; 'flag'
│           0x00001a33      48c785b0feff.  mov qword [var_150h], 0
│           0x00001a3e      c785b8feffff.  mov dword [var_148h], 0
│           0x00001a48      c785bcfeffff.  mov dword [var_144h], 0xffff <- 値段
│           0x00001a52      c785c0feffff.  mov dword [var_140h], 3
│           0x00001a5c      c785c4feffff.  mov dword [var_13ch], 3
│           0x00001a66      c785c8feffff.  mov dword [var_138h], 1
│           0x00001a70      c785ccfeffff.  mov dword [var_134h], 2
│           0x00001a7a      c785d0feffff.  mov dword [var_130h], 3
│           0x00001a84      c785d4feffff.  mov dword [var_12ch], 1
│           0x00001a8e      488d3dd50600.  lea rdi, str.Welcome_to_BookStore ; 0x216a ; "Welcome to Book
```

```xxd
419 00001a20: a4fe ffff 6e01 0000 48c7 85a8 feff ff66  ....n...H......f
420 00001a30: 6c61 6748 c785 b0fe ffff 0000 0000 c785  lagH............
421 00001a40: b8fe ffff 0000 0000 c785 bcfe ffff ffff  ................
                                                 ↑ここ
```

これを書き換えればflagの値段を変えられるので，0にでもしておきます(所持金を増やしても良いが)．

```xxd
419 00001a20: a4fe ffff 6e01 0000 48c7 85a8 feff ff66  ....n...H......f
420 00001a30: 6c61 6748 c785 b0fe ffff 0000 0000 c785  lagH............
421 00001a40: b8fe ffff 0000 0000 c785 bcfe ffff 0000  ................
```

すると，`flag`が0円になるので買って読みます．

## flag

`ISCCTF{y0u_c4n_p4tch_b1n4r13s_w1th_xxd_4nd_vim}`
