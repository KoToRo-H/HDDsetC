# HDDsetC.x
■X68000のHDDをCドライブ、FDDをA/Bドライブに移動します。

DOSコールでFDDをA/Bドライブに移動することで、HDDをCドライブにするプログラムです。<br>
X68000 Zで使うことを想定しているので、増設FDDや特殊なドライブはあまり配慮していません。<br>
HDDをCドライブにすることだけが目的です。

昔からX68000はFDDをA/Bドライブに固定して使っていました。<br>
PC-9801と併用しているとHDDドライブを間違えるからです。

＊順番に前後のドライブ番号を入れ替えていくだけのプログラムなので賢い方式では無いです。<br>

■コンパイル方法

・XCの場合 : XC /Y HDDsetC.c<br>
・GCCの場合 : GCC -ldos HDDsetC.c<br>

■参考

readDPB.x：DPBを読み込んで表示するプログラムです。DOSコールの習作ソフトです。
