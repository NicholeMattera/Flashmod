#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <string.h>

PSP_MODULE_INFO("FlashMod", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define printf	pspDebugScreenPrintf
char write_buffer[128*1024];

int exit_callback(int arg1, int arg2, void *common) {
	sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp) {
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void) {
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

void write_file(const char *readpath, const char *writepath)
{
	int fdin;
	int fdout;

	printf("Writing %s\n", writepath);
	fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
	if(fdin >= 0)
	{
		int bytesRead = 1;
		fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		if(fdout < 0)
		{
			printf("Couldn't open %s\n", writepath);
		}

		bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		while((bytesRead > 0) && (fdout >= 0))
		{
			sceIoWrite(fdout, write_buffer, bytesRead);
			bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		}

		if(fdout >= 0)
		{
			sceIoClose(fdout);
		}

		if(fdin >= 0)
		{
			sceIoClose(fdin);
		}
	}
	else
	{
		printf("Couldn't open %s\n", readpath);
	}
}


int main() {
	SceCtrlData pad;
	
	SetupCallbacks();
	pspDebugScreenInit();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	sceIoUnassign("flash0:"); 
	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);

	pspDebugScreenSetTextColor(0xFFFFFF);
	printf ("FlashMod - by Orbis PSP Development\n");
	printf ("-----------------------------------\n");
	printf ("Cross - Add Asura Effect\n");
	printf ("Circle - Remove Asura Effect\n");
	printf ("Triangle - Add Backgrounds\n");
	printf ("Square - Restore Defaults Backgrounds\n");

	while (1) {
		sceCtrlReadBufferPositive(&pad, 1);
		
		if (pad.Buttons & PSP_CTRL_CROSS) {
			write_file("ms0:/PSP/GAME/FlashMod/ASURA_EFFECT_ON/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");
		}	

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
			write_file("ms0:/PSP/GAME/FlashMod/ASURA_EFFECT_OFF/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");
		}			

		if (pad.Buttons & PSP_CTRL_TRIANGLE) {
			write_file("ms0:/01.bmp","flash0:/vsh/resource/01.bmp");
			write_file("ms0:/02.bmp","flash0:/vsh/resource/02.bmp");
			write_file("ms0:/03.bmp","flash0:/vsh/resource/03.bmp");
			write_file("ms0:/04.bmp","flash0:/vsh/resource/04.bmp");
			write_file("ms0:/05.bmp","flash0:/vsh/resource/05.bmp");
			write_file("ms0:/06.bmp","flash0:/vsh/resource/06.bmp");
			write_file("ms0:/07.bmp","flash0:/vsh/resource/07.bmp");
			write_file("ms0:/08.bmp","flash0:/vsh/resource/08.bmp");
			write_file("ms0:/09.bmp","flash0:/vsh/resource/09.bmp");
			write_file("ms0:/10.bmp","flash0:/vsh/resource/10.bmp");
			write_file("ms0:/11.bmp","flash0:/vsh/resource/11.bmp");
			write_file("ms0:/12.bmp","flash0:/vsh/resource/12.bmp");
		}			

		if (pad.Buttons & PSP_CTRL_SQUARE) {
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/01.bmp","flash0:/vsh/resource/01.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/02.bmp","flash0:/vsh/resource/02.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/03.bmp","flash0:/vsh/resource/03.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/04.bmp","flash0:/vsh/resource/04.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/05.bmp","flash0:/vsh/resource/05.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/06.bmp","flash0:/vsh/resource/06.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/07.bmp","flash0:/vsh/resource/07.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/08.bmp","flash0:/vsh/resource/08.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/09.bmp","flash0:/vsh/resource/09.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/10.bmp","flash0:/vsh/resource/10.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/11.bmp","flash0:/vsh/resource/11.bmp");
			write_file("ms0:/PSP/GAME/FlashMod/DEFAULT_BG/12.bmp","flash0:/vsh/resource/12.bmp");
		}			

	}
	return 0;
}
		
