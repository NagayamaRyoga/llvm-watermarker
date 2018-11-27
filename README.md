# LLVM Watermarker

LLVMを用いた電子透かし埋め込み器

## 開発環境

Language: C++17

OS: Ubuntu 18.04 (WSL)

Compilers: GCC 7.3.0, Clang 6.0.0

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

## ビルド方法

TODO

## 実行方法

TODO
