/*
	HDDsetC.c	HDDをCドライブにFDDをA/Bドライブに移動する
	XCコンパイル ： cc /Y HDDsetC.c
	GCCコンパイル： gcc -ldos HDDsetC.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>

#define DPB_MAX 26 /* A_drive～Z_drive */
#define FLG_SET (1) /* フラグ状態：カレントドライブ */
#define FLG_CLR (0) /* フラグ状態：カレントではない */

struct DPBPTR dpb[DPB_MAX];  /* DPBリスト */
int    curdrv_flag[DPB_MAX]; /* カレントドライブ記憶フラグ */

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

/* カレントドライブを検索してフラグに登録 */
void store_curdrv()
{
	int drive; /* カレントドライブ番号 */
	int i;

	drive = CURDRV(); /* カレントドライブ番号を取得 */
	for (i = 0; i < DPB_MAX; i++) {
		/* 全てのフラグをクリア */
		curdrv_flag[i] = FLG_CLR;
	}
	/* カレントドライブ番号のフラグだけをセット */
	curdrv_flag[drive] = FLG_SET;
}

/* カレントドライブフラグを交換 */
void swap_flag(int drv_No)
{
	int flag_tmp;

	/* drv_Noのドライブと(drv_No-1)のドライブのフラグを入れ換える */
	flag_tmp = curdrv_flag[drv_No];
	curdrv_flag[drv_No] = curdrv_flag[drv_No - 1];
	curdrv_flag[drv_No - 1] = flag_tmp;
}

/* カレントドライブを入れ換え後のドライブにする */
void restore_curdrv()
{
	int i;
	for (i = 0; i < DPB_MAX; i++) {
		if (curdrv_flag[i] == FLG_SET) {
			/* 移動後のカレントドライブ番号を検出した */
			CHGDRV(i);
			/* 以降の検索は不要なので処理を中断 */
			break;
		}
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
			swap_flag(2-1);
		}
	}

	for (i = (fd_No+1); i <= DPB_MAX; i++) {
		if ( !(dpb[i].id == FD && dpb[i].unit == unit_No) ) {
			continue; /* FDではないので次のドライブ番号を検索 */
		}
		else {
			/* FDより前に他のドライブが存在する */
			for (j = i; j > fd_No; j--) { /* 該当ドライブから前方向へ */
				DRVXCHG(j, j-1); /* 前のドライブと交換 0:curdrv, 1:DriveA*/
				swap_flag(j-1);  /* 0:DriveA, 1:DriveB */
			}
			return; /* fd_Noまで入れ替え完了 */
		}
	}
}

void main()
{
	printf("HDDsetC : HDDをCドライブにFDDをA/Bドライブに移動\n");

	/* ドライブ入れ換え前のカレントドライブを記憶 */
	store_curdrv();

	/* FD0をAドライブに移動 */
	search_dpb(); /* DPBを取得 */
	rearrange_drive_order(0);

	/* FD1をBドライブに移動 */
	search_dpb(); /* DPBを再取得 */
	rearrange_drive_order(1);

	/* カレントドライブを移動後のドライブに移す */
	restore_curdrv();

	exit(0);
}
