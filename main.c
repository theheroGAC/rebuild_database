/*
 * PS Vita 3.60 module dump
 * Based on https://github.com/xyzz/vita-modump/blob/master/main.c
 * Credits goes to xyz (original vita-modump), st4rk, smoke and theflow
 * it's very buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <psp2/power.h>
#include <psp2/ctrl.h>

#include "graphics.h"


int get_key(void) {
	static unsigned buttons[] = {
		SCE_CTRL_LTRIGGER,
		SCE_CTRL_RTRIGGER,
		SCE_CTRL_TRIANGLE,
		SCE_CTRL_CIRCLE,
		SCE_CTRL_CROSS,
		SCE_CTRL_SQUARE,
	};

	static unsigned prev = 0;
	SceCtrlData pad;
	while (1) {
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		unsigned new = prev ^ (pad.buttons & prev);
		prev = pad.buttons;
		for (int i = 0; i < sizeof(buttons)/sizeof(*buttons); ++i)
			if (new & buttons[i])
				return buttons[i];

		sceKernelDelayThread(1000); // 1ms
	}
}

void press_exit(void) {
	psvDebugScreenPrintf("Press any key to exit this application.\n");
	get_key();
	scePowerRequestColdReset();
	exit(0);
}

//cafe babe (sony devs be naughty)
char magik[] = {
	0xBE, 0xBA, 0xFE, 0xCA
};

int main(int argc, char **argv) {
	int key = 0;
	psvDebugScreenInit();

again:
	
	psvDebugScreenPrintf("Press CIRCLE to update database (id.dat method)\n");
	psvDebugScreenPrintf("Press CROSS to rebuild database (app.db method)\n");
	psvDebugScreenPrintf("Press SQUARE to rebuild database (dbr.db-err method)\n");
	psvDebugScreenPrintf("Press TRIANGLE to restart PSVITA\n");
	psvDebugScreenPrintf("Press RTRIGGER to shutdown PSVITA\n");
	psvDebugScreenPrintf("Press LTRIGGER to suspend PSVITA\n");
	
	key = get_key();
	
	if (key == SCE_CTRL_CROSS) {
		psvDebugScreenPrintf("Rebuilding database...\n");
		sceIoRemove("ur0:shell/db/app.db");
	} else if (key == SCE_CTRL_SQUARE) {
		psvDebugScreenPrintf("Rebuilding database...\n");
		SceUID id;
		id = sceIoOpen("ur0:shell/db/dbr.db-err", SCE_O_WRONLY|SCE_O_CREAT, 0777);
		//chosen by fair dice roll !
		sceIoWrite(id, magik, 4);
		sceIoClose(id);
	}
	else if (key == SCE_CTRL_CIRCLE) {
		psvDebugScreenPrintf("Updating database...\n");
		sceIoRemove("ux0:id.dat");
		SceUID id;
		id = sceIoOpen("ux0:/id.dat", SCE_O_WRONLY|SCE_O_CREAT, 0777);
		sceIoWrite(id, NULL, 0x0);
		sceIoClose(id);
		}
		else if (key == SCE_CTRL_TRIANGLE) {
		scePowerRequestColdReset();
		}
	else if (key == SCE_CTRL_RTRIGGER) {
		scePowerRequestStandby();
		}
	else if (key == SCE_CTRL_LTRIGGER) {
		scePowerRequestSuspend();
	}else {
		printf("Invalid input, try again.\n\n");
		goto again;
	}
	
	press_exit();
}
	
	
	
	

