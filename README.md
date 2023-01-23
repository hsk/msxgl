# MSXGL を使ってゲームプログラミングをしよう

最新の SDCC 4.2.0 を使ってMSXの未来を感じよう。

![Alt text](s_game.gif) ![Alt text](myv9990.gif)


https://github.com/aoineko-fr/MSXgl.git

(2023/01/23現在) 最新の SDCC4.2.0 をサポートしているゲームライブラリ MSXgl を使って開発してみます。

## インストール

https://aoineko.org/msxgl/index.php?title=Install

ここを参考にDeepL先生に翻訳してもらいました:

-----------

MSXglのインストール方法

以下の手順に従ってください。

- 1️⃣ ディレクトリを作成する（例えば "MSXgl "と名付ける）。

    注：フルパスにスペースを入れないディレクトリを使用することを推奨します。

- 2️⃣ エンジンをインストールします。

    Gitリポジトリ https://github.com/aoineko-fr/MSXgl.git (v0.8.0-alphaタグ)をクローンする。
    または、最新版のエンジンをダウンロードし、解凍してください。

- 3️⃣ (オプション) エミュレータ自動起動オプションを使用する場合。

    MSXgl/projects/default_config.js ファイル（デフォルトのBuildツールの設定）を編集する。
    Emulator "を検索し、お気に入りのエミュレータのパスを設定し（OpenMSX, Emulicious, BlueMSX, Meisei, fMSX or Ru-MSX）、"DoRun = true "を設定します。
    注意: default_config.js の他の多くのオプションは、プロジェクトの設定によって上書きされます。

- ✅ これで完了です！ライブラリのサンプルを試したり、まったく新しいゲームを作ったりすることができます。

## macOS

WindowsとLinuxでは、必要なツールはすべてMSXglパッケージに組み込まれていますが、macOSユーザーはMSXglのビルドツールを実行する前に、Node.js（18.12.1またはそれ以降）とSDCC（4.2.0）を手動でインストールする必要があります。このインストールガイドを参照してください。

Buildツールで使用するすべてのプログラムのパスは、default_config.jsファイルで変更することができます。

-----------

とのことですので M1 Mac 環境では以下のようにインストールしました:


```
mkdir msxgl
cd msxgl
git clone https://github.com/aoineko-fr/MSXgl.git
code MSXgl/engine/script/js/default_config.js
```
Macなのでsdccはbrewでインストール

```bash
brew install sdcc
```

設定は以下の箇所を書き換えました:

```javascript
DoRun     = true;	//-- Start the program automatically at the end of the build (boolean)
Compiler  = `sdcc`;				//-- Path to the C compile program (string)
Assembler = `sdasz80`;			//-- Path to the assembler program (string)
Linker    = `sdcc`;				//-- Path to the linker program (string)
MakeLib   = `sdar`;				//-- Path to the program to generate lib file (string)
Emulator  = "openmsx";										//-- Path to the emulator to launch the project (string)
```

Mac の場合はビルドが必要なのでビルドします。

```
cd tools/MSXtk
./build.sh
cd ../..
```

できたら、サンプルをビルド実行してみます:

```
./build.sh
cd projects/samples
./build.sh s_game
```

ペンギンのジャンプアクションが動いたら成功です。

![Alt text](s_game.gif)

たぶん、jsファイルが有るサンプルが遊べるはずなので以下のプログラムが動くはず:

```
./build.sh s_arkos
./build.sh s_ayfx
./build.sh s_bios
./build.sh s_clock
./build.sh s_dos
./build.sh s_dos0
./build.sh s_dos2
./build.sh s_draw
./build.sh s_game
./build.sh s_hello
./build.sh s_joystk
./build.sh s_keybrd
./build.sh s_lvgm
./build.sh s_mapper
./build.sh s_math
./build.sh s_mouse
./build.sh s_ntap
./build.sh s_onet
./build.sh s_pcmenc
./build.sh s_pcmplay
./build.sh s_print
./build.sh s_psg
./build.sh s_pt3
./build.sh s_scroll
./build.sh s_sm1
./build.sh s_sprite
./build.sh s_swsprt
./build.sh s_sys
./build.sh s_text
./build.sh s_trilo
./build.sh s_usr
./build.sh s_v9990
./build.sh s_vdpcmd
./build.sh s_vgm
./build.sh s_wyz
./build.sh s_wyz2
./build.sh s_zip
```

## v9990 のサンプルを動かそう

s_v9990 のサンプルをビルドすれば動くはず:

```
./build.sh s_v9990
```

文字しか出ないじゃないか？と思うのですが、 v9990 は別画面に出力されてるので F12 を押して v9990 の出力と通常画面の出力を切り替えましょう。
ペンギンがたくさん。たぶん、スプライトですね。
左右のカーソルキーで色々な画面モードのデモを見ることができます。

![Alt text](myv9990.gif)

## オリジナルアプリを作ろう

サンプルの動かし方は分かった。どうやって作るのでしょうか？

https://aoineko.org/msxgl/index.php?title=Create_my_first_program

ここに従って以下のコマンドを実行しました:

```
cd ..
cp -r template myprog
cd myprog
./build.sh 
```

![Alt text](myprog.gif)

文字が出るだけですが成功です。

## オリジナルアプリを作ろう 2

MSXglのフォルダの外にプロジェクトディレクトリを作ってみましょう。

```
cd msxgl
mkdir apps
cp -rf MSXgl/apps/template myprog
```

apps/myprog/build.sh の対応箇所を以下のように書き換えます:

```
if type -P node; then
	node ../../MSXgl/engine/script/js/build.js target=$1
else
	../../MSXgl/tools/build/Node/node ../../MSXgl/engine/script/js/build.js target=$1
fi
```

これでOKです。

## オリジナルv9990アプリを作ろう

次はv9990を使ったアプリの開発をしてみましょう。

msxglフォルダに戻って必要なファイルをコピーしてビルドします。

```
mkdir -p apps/myv9990/content; cd apps/myv9990
cp ../../MSXgl/projects/samples/build* ../../MSXgl/projects/samples/s_v9990* ../../MSXgl/projects/samples/project_config.js ../../MSXgl/projects/samples/msxgl_config*.h .
cp -rf ../../MSXgl/projects/samples/content/v9990 content/.
```

build.shの対応箇所を以下のように書き換えます:

```
if type -P node; then
	node ../../MSXgl/engine/script/js/build.js projname=$1 target=$2
else
	../../MSXgl/tools/build/Node/node ../../MSXgl/engine/script/js/build.js projname=$1 target=$2
fi
```

```
./build.sh s_v9990
```

動いたら成功です。

![Alt text](myv9990.gif)

F12を押してv9990の出力が確認しましょう。

他のサンプルプログラムも調べて１個ずつ取り出してみたり、buildの設定を変えるとより使いやすくなるでしょうし、勉強にもなるでしょう。

## ムービーを取ろう

自分はMakefile使うのが好きなので以下のようなMakefileを作って make で実行 make gif で gif ファイル make mp4 で mp4 を作ります。

```Makefile
rec:
	openmsx -machine Panasonic_FS-A1GT -ext gfx9000 -ext Moonsound -carta out/template.rom -command "set speed 9999;after time 13.5 \"set speed 100;record start aaa\""
mp4:
	ffmpeg -y -i ~/.openMSX/videos/aaa.avi -b 1500k -r 30  -vf "scale=960:720" -vcodec libx264 -pix_fmt yuv420p aaa.mp4
gif:
	mkdir -p mov
	ffmpeg -y -i ~/.openMSX/videos/aaa.avi -r 30 mov/%04d.png
	convert +map  -delay 3 mov/*.png aaa.gif
	rm -rf mov
	code aaa.gif
```

`set speed 9999;after time 13.5` は高速起動のおまじないです。

![Alt text](myprog.gif)

![Alt text](myv9990.gif)

## デバッガを使ってみよう

ある程度プログラムを組んでいるとデバッガが欲しくなることがあります。

Emulicious は Java 製のマルチプラットフォームなMSX対応のデバッガ付きエミュレータです。

https://emulicious.net/downloads/

Emulicious は v9990 には残念ながら対応していないようですが、v9990はprintfデバッグができるので通常のMSXプログラムのデバッガが欲しい場合に設定すると便利でしょう。

VSCode の拡張のemulicious-debuggerを追加します。

https://marketplace.visualstudio.com/items?itemName=emulicious.emulicious-debugger

VSCodeの設定は以下のようにEmuliciousとjavaのPathを加えます:

```json
  "emulicious-debugger.emuliciousPath": "/Users/hiroshisakurai/Downloads/Emulicious",
  "emulicious-debugger.javaPath": "/opt/homebrew/opt/openjdk@11/bin/java"
```

Mac なので起動コマンドを作ります:

```
vi ~/Downloads/Emulicious/emulicious
```

内容はこんな感じに:

```
#!/bin/zsh
java -jar ~/Downloads/Emulicious/Emulicious.jar $@
```

実行権限を与えます:

```
chmod 755 ~/Downloads/Emulicious/emulicious
```

myprog をコピーしてmydbgを作って設定してみます。

default_config.js の Debugger にも設定しておきましょう。使い方はよくわかりませんがｗ

```js
Debugger  = "~/Downloads/Emulicious/emulicious";
```

project_config.js で Debugger の設定をします。

```js
//-- Prepare program for debug (boolean)
Debug = true;

//-- Move debug symbols to deployement folder (boolean)
DebugSymbols = true;
```

.vscode/launch.json :

```json
{
    // https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "emulicious-debugger",
            "request": "launch",
            "name": "Launch in Emulicious",
            "program": "${workspaceFolder}/out/template.rom",
            "port": 58870,
            "stopOnEntry": false,
            "preLaunchTask": "build"
        }
    ]
}
```

.vscode/tasks.json :

```json
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "./build.sh"
        }
    ]
}
```

と設定してVS Code のデバッカを起動します。
ブレイクポイントを設定して止まれば成功です。

## VSCode のワークスペースを使おう

msxgl.code-workspace

```
{
	"folders": [
		{
			"name": "1.myprog",
			"path": "apps/myprog"
		},
		{
			"name": "2.myv9990",
			"path": "apps/myv9990"
		},
		{
			"name": "3.mydbg",
			"path": "apps/mydbg"
		},
		{
			"name": "msxgl",
			"path": "."
		},
	],
}
```

と書いておいて vscode でワーウスペースを開くとプロジェクトを複数切り替えて見れるので便利です。
