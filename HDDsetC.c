/*
	HDDsetC.c	HDD��C�h���C�u��FDD��A/B�h���C�u�Ɉړ�����
	XC�R���p�C���F cc /Y HDDsetC.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>

#define DPB_MAX 26 /* A_drive�`Z_drive */

struct DPBPTR dpb[DPB_MAX]; /* DPB���X�g */

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
		}
	}

	for (i = (fd_No+1); i <= DPB_MAX; i++) {
		if ( !(dpb[i].id == FD && dpb[i].unit == unit_No) ) {
			continue; /* FD�ł͂Ȃ��̂Ŏ��̃h���C�u�ԍ������� */
		}
		else {
			/* FD���O�ɑ��̃h���C�u�����݂��� */
			for (j = i; j > fd_No; j--) {
				DRVXCHG(j, j-1); /* �O�̃h���C�u�ƌ��� */
			}
			return; /* fd_No�܂œ���ւ����� */
		}
	}
}

void main()
{
	printf("HDDsetC : HDD��C�h���C�u��FDD��A/B�h���C�u�Ɉړ�\n");

	/* FD0��A�h���C�u�Ɉړ� */
	search_dpb(); /* DPB���擾 */
	rearrange_drive_order(0);

	/* FD1��B�h���C�u�Ɉړ� */
	search_dpb(); /* DPB���Ď擾 */
	rearrange_drive_order(1);

	/* �J�����g�h���C�u��C�h���C�u�Ɉڂ� */
	CHGDRV(2);

	exit(0);
}
