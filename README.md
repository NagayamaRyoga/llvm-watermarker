# LLVM Watermarker

LLVMを用いた電子透かし埋め込み器

## 開発環境

Language: C++17

OS: Ubuntu 18.04

Compilers: GCC 7.3.0, Clang 7.0.0, rustc 1.31.1

Libraries: LLVM 7.0

## 命名規約

| 種別 | 規約 | 理由 | 備考 |
|:-----|:----:|:-----|:-----|
| 型 | UpperCamelCase | 慣例 | |
| 名前空間 | snake\_case | 好み | |
| ローカル変数/引数 | snake\_case | 好み | |
| グローバル変数 | g\_snake\_case | 慣例 | 使わない |
| ファイルローカル変数 | snake\_case | 好み | `static`ではなくanonymous namespaceを使う |
| 関数 | snake\_case | なんとなく | |
| メンバ変数 | snake\_case\_ | 慣例 | |
| `static`メンバ変数 | snake\_case | なんとなく | 必要なければ避ける |
| メンバ関数 | snake\_case | なんとなく | |
| メンバ型 | snake\_case | 標準準拠 | |
| 列挙子 | UpperCamelCase | 好み | かっこいい気がする |
| マクロ | SNAKE\_CASE | 慣例 | 必要ある時のみ |

## プロジェクトのダウンロード

```sh
$ git clone git@github.com:NagayamaRyoga/llvm-watermarker.git
$ cd llvm-watermarker
```

## ビルド

```sh
$ make
```

## 実行方法

```sh
# コードレイアウトによる埋め込み
$ opt -load=bin/src/nykk.so -block-wm -watermark="<watermark>" -o <output-llvm-bitcode> <input-llvm-bitcode>
# 命令の変更による埋め込み
$ opt -load=bin/src/nykk.so -inst-wm -watermark="<watermark>" -o <output-llvm-bitcode> <input-llvm-bitcode>
```
