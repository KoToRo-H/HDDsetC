# HDDsetC.x
### ■説明
X68000のHDDをCドライブ、FDDをA/Bドライブに移動します。

DOSコールでFDDをA/Bドライブに移動することで、HDDをCドライブにするプログラムです。
X68000 Zで使うことを想定しているので、増設FDDや特殊なドライブはあまり配慮していません。
HDDをCドライブにすることだけが目的です。

昔からX68000はFDDをA/Bドライブに固定して使っていました。
その理由はPC-9801と併用しているとHDDドライブを間違えるからです。

＊順番に前後のドライブ番号を入れ替えていくだけのプログラムなので賢い方式では無いです。

### ■コンパイル方法
・XCの場合 : cc /Y HDDsetC.c
・GCCの場合 : gcc -ldos HDDsetC.c

### ■参考
readDPB.x：DPB（Drive Parameter Block）を読み込んで表示するだけの習作プログラムです。
