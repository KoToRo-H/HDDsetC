# HDDsetC.x
■説明<br>
X68000のHDDをCドライブ、FDDをA/Bドライブに移動します。

DOSコールでFDDをA/Bドライブに移動することで、HDDをCドライブにするプログラムです。<br>
X68000 Zで使うことを想定しているので、増設FDDや特殊なドライブはあまり配慮していません。<br>
HDDをCドライブにすることだけが目的です。

昔からX68000はFDDをA/Bドライブに固定して使っていました。<br>
その理由はPC-9801と併用しているとHDDドライブを間違えるからです。

＊順番に前後のドライブ番号を入れ替えていくだけのプログラムなので賢い方式では無いです。<br>

■コンパイル方法<br>
・XCの場合 : cc /Y HDDsetC.c<br>
・GCCの場合 : gcc -ldos HDDsetC.c<br>

■参考<br>
readDPB.x：DPB（Drive Parameter Block）を読み込んで表示するだけの習作プログラムです。
