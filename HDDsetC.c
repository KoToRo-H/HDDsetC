/*
	HDDsetC.c	HDDをCドライブにFDDをA/Bドライブに移動する
	XCコンパイル： cc /Y HDDsetC.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>

#define DPB_MAX 26 /* A_drive～Z_drive */

struct DPBPTR dpb[DPB_MAX]; /* DPBリスト */

/* メディアバイト id */
enum DRIVE_ID {
	FD   = 0xFE,
	SCSI = 0xF7,
	SASI = 0xF8,
	CD   = 0xF5,
	MO   = 0xF6,
	RAM  = 0xF9,
};

/* 現在のDPBをリストに登録 */
void search_dpb()
{
	int drive_No;

	for (drive_No = 1; drive_No < DPB_MAX; drive_No++) {
		GETDPB(drive_No, (dpb + drive_No));
	}
}

/* ドライブ入れ替え：先頭から検索してunit_NoのFDを見つけたら終了 */
void rearrange_drive_order(int unit_No)
{
	int fd_No;
	int i, j;

	fd_No = unit_No + 1;
	if (dpb[fd_No].id == FD && dpb[fd_No].unit == unit_No) {
		return; /* 入れ替え不要 */
	}

	if (unit_No == 1) {
		if (dpb[1].id == FD && dpb[1].unit == unit_No) {
			/* FD1がAドライブに割り当たっていた場合だけBドライブと交換 */
			DRVXCHG(1, 2);
		}
	}

	for (i = (fd_No+1); i <= DPB_MAX; i++) {
		if ( !(dpb[i].id == FD && dpb[i].unit == unit_No) ) {
			continue; /* FDではないので次のドライブ番号を検索 */
		}
		else {
			/* FDより前に他のドライブが存在する */
			for (j = i; j > fd_No; j--) {
				DRVXCHG(j, j-1); /* 前のドライブと交換 */
			}
			return; /* fd_Noまで入れ替え完了 */
		}
	}
}

void main()
{
	printf("HDDsetC : HDDをCドライブにFDDをA/Bドライブに移動\n");

	/* FD0をAドライブに移動 */
	search_dpb(); /* DPBを取得 */
	rearrange_drive_order(0);

	/* FD1をBドライブに移動 */
	search_dpb(); /* DPBを再取得 */
	rearrange_drive_order(1);

	/* カレントドライブをCドライブに移す */
	CHGDRV(2);

	exit(0);
}
