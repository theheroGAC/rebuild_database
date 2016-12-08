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


int main(int argc, char **argv) {
	psvDebugScreenInit();
	psvDebugScreenPrintf("Rebuilding database(maybe)...\n");

	sceIoRemove("ur0:shell/db/app.db");
	
	scePowerRequestColdReset();
	
	sceKernelExitProcess(0);
	return 0;
}
