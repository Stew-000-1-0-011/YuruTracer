# YuruTracer

STM32F103xBでライントレーサを動かすコード。
総合工学プロジェクト2024で使われた。

## 朧げ記憶リスト
- ADC + DMAでセンサを読んでたはず
- ピン配置や動作の把握には`YuruTracer.ioc`や`Core/Inc`下の`motor.hpp`や`mymain.hpp`を参照して
- ほとんどの処理は`Core/Inc`下の`.hpp`ファイルに書いてる。`.cpp`は必要な場合のみ使ってる
- `.h`や`.c`はほとんど自動生成で作ったファイル

## VSCode + clangdで開けるよ
1. 入れてない人はVSCodeにclangd拡張を入れる
2. このプロジェクトをSTM32CubeIDEで開く
3. iocファイルからコードを生成
  - これにより`/Drivers/`が生える
4. STM32CubeIDEでビルド
  - VSCodeから行えるようにしたいね
  - 既に`Project -> Properties -> C/C++ Build -> JSON Compilation Database Generator`を有効化している
    これにより`compile_commands.json`がビルド時に生成される。これをclangdなどに読ませることで良い感じに補完が効く。
    なお、このままではうまくいかないので以下で少し細工する
5. VSCodeでこのプロジェクトのルートディレクトリを開く
6. `make_commands_json.bash`を実行(`source make_commands_json.bash`)
  - ビルド時に生成されているはずの`Debug/compile_commands.json`を使う。先にビルドしておいてね
7. VSCodeのウィンドウをリロード
  - clangdにcompile_commands.jsonを読ませるため
