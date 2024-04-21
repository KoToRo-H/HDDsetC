/*
	HDDsetC.c	HDD��C�h���C�u��FDD��A/B�h���C�u�Ɉړ�����
	XC�R���p�C�� �F cc /Y HDDsetC.c
	GCC�R���p�C���F gcc -ldos HDDsetC.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>

#define DPB_MAX 26 /* A_drive�`Z_drive */
#define FLG_SET (1) /* �t���O��ԁF�J�����g�h���C�u */
#define FLG_CLR (0) /* �t���O��ԁF�J�����g�ł͂Ȃ� */

struct DPBPTR dpb[DPB_MAX];  /* DPB���X�g */
int    curdrv_flag[DPB_MAX]; /* �J�����g�h���C�u�L���t���O */

/* ���f�B�A�o�C�g id */
enum DRIVE_ID {
	FD   = 0xFE,
	SCSI = 0xF7,
	SASI = 0xF8,
	CD   = 0xF5,
	MO   = 0xF6,
	RAM  = 0xF9,
};

/* ���݂�DPB�����X�g�ɓo�^ */
void search_dpb()
{
	int drive_No;

	for (drive_No = 1; drive_No < DPB_MAX; drive_No++) {
		GETDPB(drive_No, (dpb + drive_No));
	}
}

/* �J�����g�h���C�u���������ăt���O�ɓo�^ */
void store_curdrv()
{
	int drive; /* �J�����g�h���C�u�ԍ� */
	int i;

	drive = CURDRV(); /* �J�����g�h���C�u�ԍ����擾 */
	for (i = 0; i < DPB_MAX; i++) {
		/* �S�Ẵt���O���N���A */
		curdrv_flag[i] = FLG_CLR;
	}
	/* �J�����g�h���C�u�ԍ��̃t���O�������Z�b�g */
	curdrv_flag[drive] = FLG_SET;
}

/* �J�����g�h���C�u�t���O������ */
void swap_flag(int drv_No)
{
	int flag_tmp;

	/* drv_No�̃h���C�u��(drv_No-1)�̃h���C�u�̃t���O����ꊷ���� */
	flag_tmp = curdrv_flag[drv_No];
	curdrv_flag[drv_No] = curdrv_flag[drv_No - 1];
	curdrv_flag[drv_No - 1] = flag_tmp;
}

/* �J�����g�h���C�u����ꊷ����̃h���C�u�ɂ��� */
void restore_curdrv()
{
	int i;
	for (i = 0; i < DPB_MAX; i++) {
		if (curdrv_flag[i] == FLG_SET) {
			/* �ړ���̃J�����g�h���C�u�ԍ������o���� */
			CHGDRV(i);
			/* �ȍ~�̌����͕s�v�Ȃ̂ŏ����𒆒f */
			break;
		}
	}
}

/* �h���C�u����ւ��F�擪���猟������unit_No��FD����������I�� */
void rearrange_drive_order(int unit_No)
{
	int fd_No;
	int i, j;

	fd_No = unit_No + 1;
	if (dpb[fd_No].id == FD && dpb[fd_No].unit == unit_No) {
		return; /* ����ւ��s�v */
	}

	if (unit_No == 1) {
		if (dpb[1].id == FD && dpb[1].unit == unit_No) {
			/* FD1��A�h���C�u�Ɋ��蓖�����Ă����ꍇ����B�h���C�u�ƌ��� */
			DRVXCHG(1, 2);
			swap_flag(2-1);
		}
	}

	for (i = (fd_No+1); i <= DPB_MAX; i++) {
		if ( !(dpb[i].id == FD && dpb[i].unit == unit_No) ) {
			continue; /* FD�ł͂Ȃ��̂Ŏ��̃h���C�u�ԍ������� */
		}
		else {
			/* FD���O�ɑ��̃h���C�u�����݂��� */
			for (j = i; j > fd_No; j--) { /* �Y���h���C�u����O������ */
				DRVXCHG(j, j-1); /* �O�̃h���C�u�ƌ��� 0:curdrv, 1:DriveA*/
				swap_flag(j-1);  /* 0:DriveA, 1:DriveB */
			}
			return; /* fd_No�܂œ���ւ����� */
		}
	}
}

void main()
{
	printf("HDDsetC : HDD��C�h���C�u��FDD��A/B�h���C�u�Ɉړ�\n");

	/* �h���C�u���ꊷ���O�̃J�����g�h���C�u���L�� */
	store_curdrv();

	/* FD0��A�h���C�u�Ɉړ� */
	search_dpb(); /* DPB���擾 */
	rearrange_drive_order(0);

	/* FD1��B�h���C�u�Ɉړ� */
	search_dpb(); /* DPB���Ď擾 */
	rearrange_drive_order(1);

	/* �J�����g�h���C�u���ړ���̃h���C�u�Ɉڂ� */
	restore_curdrv();

	exit(0);
}
