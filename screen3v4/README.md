# リニアにタイルを並べたバージョン

縦方向ならリニアに並べられるので並べてAPI化する。

1. コピー
2. 拡大縮小コピー
3. 回転拡大縮小コピー
4. 回転コピー
5. カラーキー付きコピー
6. カラーキー付き拡大縮小コピー
7. カラーキー付き回転拡大縮小コピー
8. カラーキー付き回転コピー

元データを1バイトで2ドットの縦方向に並んだデータとする。
struct SCR34 {
    u16 width;
    u16 height;
    u8 data[1];
}
なデータ構造でファイルから読み込める。
コピーができるだけでいいか。

## 1. バイト単位高速コピー

1. [x] バイト単位でコピー
2. [x] バイト単位クリッピング付きコピー
3. [ ] バイト単位クリッピング、カラーキー、付きコピー
4. [ ] 1画面VRAM転送
5. [ ] 範囲指定VRAM転送
6. [ ] クリッピング付き範囲指定VRAM転送

## 2. ドット単位処理

1. [ ] ドット単位コピー
2. [ ] ドット単位クリッピング付きコピー
3. [ ] ドット単位カラーキー、クリッピング付きコピー
4. [ ] 範囲指定VRAM転送
5. [ ] クリッピング付き範囲指定VRAM転送

## 3. BG処理

ハードウエアスクロール的にスクロールするための機能

N x 64 のBG作成

SC34のドットデータはVRAM上では縦につながっていて48ドットあるいは53ドットが表示される。
64ドットだと切りが良いので64ドット単位でBGを用意できると座標計算が楽になる。
横を64ドットとするとぴったりだけど、リニアに増やせるので66ドットあれば緩やかなスクロールが可能だけど、マップチップ敷き詰めを考えると８ドット単位で
64+8=72ドットなどがよいかもしれないし、128ドットくらいあったほうがいいこともありそうなのだけどこうだと決める必要性は低いが、とりあえず実装が必要。

まずは固定で8x8のマップチップと64ドットx72ドットで作る。2304bytes = 2.3kbytesか。

1. マップチップからBG画像を生成する。
2. BG画像からスクロールして仮想VRAMに転送する。

3. スクロール値とマップチップアドレス、タイルマップアドレスからBG生成機能を作る。

BGバッファを持たずに毎回生成するのでメモリを少なく扱える機能。


1. putPixel() で点を打てる
2. drawVLine() でクリッピングなしの縦線が引ける
3. fillRect() でクリッピングなしの長方形が描ける
4. drawVLine() でクリッピングありの縦線が引ける
5. fillRect() でクリッピング付きの長方形が描ける
rectとrectのインターセクションが取れるといい。
6. getPixel() で色が取れる。
6. クリッピングなし範囲指定の縦ラインコピーができる。
6. クリッピングなし範囲指定のrectコピーができる。
6. クリッピングあり範囲指定の縦ラインコピーができる。
6. クリッピングあり範囲指定のrectコピーができる。
