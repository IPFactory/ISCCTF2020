# ISCCTF 2020

![Markdownlint Workflow](https://github.com/ipfactory/ISCCTF2020/workflows/Markdownlint%20Workflow/badge.svg)

![iscctf_logo](https://drive.google.com/uc?export=view&id=1SY43rwoVQLTyzW-YS8voxgY1Jb4xXPKi)

## 概要

2020/10/24(土)10:00-23:00(JST)で開催した入門者向けCTF`ISCCTF2020`の問題リポジトリです。
なお、ISCCTFの登録ユーザ数は135で、うち獲得スコア1点以上のユーザ数は84でした。

## 問題一覧

- Web
  - [warmup 100pts(74solves)][Greetinjs](web/Greetinjs/)
  - [easy 285pts(45solves)][Yonezer](web/yonezer/)
  - [easy 475pts(16solves)][mark damn it](web/mark-damn-it/)
  - [medium 491pts(10solves)][crackjwt](web/crackjwt/)
- Pwn
  - [warmup 436pts(25solves)][stuck](pwn/stuck/)
  - [easy 468pts(18solves)][echo](pwn/echo/)
  - [easy 493pts(9solves)][portal](pwn/portal/)
  - [medium 498pts(6solves)][iter_fold](pwn/iter_fold/)
- Reversing
  - [warmup 100pts(63solves)][strings](rev/strings/)
  - [easy 468pts(18solves)][bookshop](rev/bookshop/)
  - [medium 495pts(8solves)][The Full Bug](rev/The_Full_Bug/)
- Forensics
  - [warmup 387pts(33solves)][Last Logon](forensics/Last_Logon/)
- Misc
  - [easy 0pts(4solves)][maze](misc/maze/)
  - [easy 468pts(18solves)][Shell Ain't Bad Place to Be](misc/shell_aint_bad_place_to_be/)

## その他補足事項

### ディレクトリの構成について

ディレクトリの構成について、大まかなルールは下記の通りです。

- `カテゴリ名/問題名/README.md`に問題の概要があります。
- 本番で配布されたファイルは、`カテゴリ名/問題名/distfiles/`または`カテゴリ/問題名/rawdistfiles/`に入っています。
  - 本番でファイルを配布しなかった問題は、`distfiles/`も`rawdistfiles/`もありません。
- `カテゴリ名/問題名/solution/README.md`に作問者writeupがあります。

### 問題のデプロイについて

- WebやPwn、その他のデプロイが必要な問題は、`カテゴリ名/問題名/`で`docker-compose up --build`を実行することで、デプロイできます。
- デプロイした問題を止める場合は、`docker-compose stop`を実行してください。
