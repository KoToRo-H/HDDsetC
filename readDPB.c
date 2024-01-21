#include <stdio.h>
#include <doslib.h>

#define MAXDPB	26 /* A_drive - Z_drive */

void main(void)
{
	struct DPBPTR	dpb;
	int		drive;
	
	for (drive = 1; drive < (MAXDPB + 1); drive++) {
		GETDPB(drive, &dpb);
		if (dpb.id == 0x00)
			continue;
		printf("[%02d] drive %c:, unit %02d, id %02X\n",
				drive, dpb.drive + 'A', dpb.unit, dpb.id);
	}

	return;
}
