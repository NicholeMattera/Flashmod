#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <pspgu.h>
#include <pspumd.h>
#include <psppower.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

PSP_MODULE_INFO("FlashMod", 0, 1, 1);

int exist = 0;
char write_buffer[128*1024];

int exit_callback(int arg1, int arg2, void *common);

int CallbackThread(SceSize args, void *argp); 

int SetupCallbacks(void);

void check(const char* zFile);

void write_file(const char *readpath, const char *writepath, Image* write, Image* writecomplete, Image* writefailed);

int main() {
	SetupCallbacks();
	initGraphics();

	int cwait;
	char copyright_buffer[200]; Image* copyright;
	sprintf(copyright_buffer, "./Images/Copyright.png"); copyright = loadImage(copyright_buffer);
	blitAlphaImageToScreen(0, 0, 480, 272, copyright, 0, 0);
	flipScreen();

	for(cwait=0; cwait<50; cwait++) {
	}

	int UMDcheck = 0; int XMBmenu = 1; int xmbselect = 30; int UMDmenu = 1; int umdselect = 30; int PSIXmenu = 1; int psixselect = 30; int dialogbox = 1; int taskbarx = 480; int statusbary = 272; int mainmenu2 = 1; int mainmenu = 1; int wait = 0; SceCtrlData pad;

	scePowerSetClockFrequency(333, 333, 166);

	char desktop_buffer[200]; Image* desktop;

	sprintf(desktop_buffer, "./Images/Desktop.png"); desktop = loadImage(desktop_buffer);
	blitAlphaImageToScreen(0, 0, 480, 272, desktop, 0, 0);
	flipScreen();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	sceIoUnassign("flash0:"); 
	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);

	char flashingstatusbar_buffer[200]; Image* flashingstatusbar;
	char failedstatusbar_buffer[200]; Image* failedstatusbar;
	char completestatusbar_buffer[200]; Image* completestatusbar;
	char statusbar_buffer[200]; Image* statusbar;
	char exiton_buffer[200]; Image* exiton;
	char exit_buffer[200]; Image* exit;
	char taskbar0_buffer[200]; Image* taskbar0;
	char taskbar1_buffer[200]; Image* taskbar1;
	char taskbar2_buffer[200]; Image* taskbar2;
	char taskbar3_buffer[200]; Image* taskbar3;
	char umd_buffer[200]; Image* umd;
	char umdmenu_buffer[200]; Image* umdmenu;
	char xmb_buffer[200]; Image* xmb;
	char xmbmenu_buffer[200]; Image* xmbmenu;
	char psix_buffer[200]; Image* psix;
	char psixmenu_buffer[200]; Image* psixmenu;
	char select_buffer[200]; Image* select;
	char insertumd_buffer[200]; Image* insertumd;

	sprintf(flashingstatusbar_buffer, "./Images/FlashingStatusbar.png"); flashingstatusbar = loadImage(flashingstatusbar_buffer);
	sprintf(failedstatusbar_buffer, "./Images/FailedStatusbar.png"); failedstatusbar = loadImage(failedstatusbar_buffer);
	sprintf(completestatusbar_buffer, "./Images/CompleteStatusbar.png"); completestatusbar = loadImage(completestatusbar_buffer);
	sprintf(statusbar_buffer, "./Images/Statusbar.png"); statusbar = loadImage(statusbar_buffer);
	sprintf(exiton_buffer, "./Images/Exiton.png"); exiton = loadImage(exiton_buffer);  
	sprintf(exit_buffer, "./Images/Exit.png"); exit = loadImage(exit_buffer); 
	sprintf(taskbar0_buffer, "./Images/Taskbar0.png"); taskbar0 = loadImage(taskbar0_buffer); 	
	sprintf(taskbar1_buffer, "./Images/Taskbar1.png"); taskbar1 = loadImage(taskbar1_buffer);		 
	sprintf(taskbar2_buffer, "./Images/Taskbar2.png"); taskbar2 = loadImage(taskbar2_buffer); 
	sprintf(taskbar3_buffer, "./Images/Taskbar3.png"); taskbar3 = loadImage(taskbar3_buffer);
	sprintf(umd_buffer, "./Images/UMD.png"); umd = loadImage(umd_buffer);		 
	sprintf(umdmenu_buffer, "./Images/UMDMenu.png"); umdmenu = loadImage(umdmenu_buffer);
	sprintf(xmb_buffer, "./Images/XMB.png"); xmb = loadImage(xmb_buffer); 
	sprintf(xmbmenu_buffer, "./Images/XMBMenu.png"); xmbmenu = loadImage(xmbmenu_buffer); 
	sprintf(psix_buffer, "./Images/PSIX.png"); psix = loadImage(psix_buffer);
	sprintf(psixmenu_buffer, "./Images/PSIXMenu.png"); psixmenu = loadImage(psixmenu_buffer);
	sprintf(select_buffer, "./Images/Select.png"); select = loadImage(select_buffer);
	sprintf(insertumd_buffer, "./Images/InsertUMD.png"); insertumd = loadImage(insertumd_buffer);  

	while(1){
		sceCtrlReadBufferPositive(&pad, 1);
		blitAlphaImageToScreen(0, 0, 480, 272, desktop, 0, 0);
		if(dialogbox == 1) {
			if(taskbarx > 374) {
				if(statusbary == 244) { blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, statusbary); blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, statusbary);}
				if(statusbary > 244) {
					blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, statusbary);
					blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, statusbary);
					statusbary--;
				}
				blitAlphaImageToScreen(0, 0, 106, 272, taskbar1, taskbarx, 0);
				taskbarx--;
			}
			if(taskbarx == 374) {	
				if(mainmenu2 == 1) {
					blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244);
					blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);
					if(mainmenu == 1)blitAlphaImageToScreen(0, 0, 106, 272, taskbar1, 374, 0);
					if(mainmenu == 2)blitAlphaImageToScreen(0, 0, 106, 272, taskbar2, 374, 0);
					if(mainmenu == 3)blitAlphaImageToScreen(0, 0, 106, 272, taskbar3, 374, 0);
				}
				if(mainmenu2 == 2) {
					blitAlphaImageToScreen(0, 0, 100, 272, taskbar0, 380, 0);
					blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244); 
					blitAlphaImageToScreen(0, 0, 50, 25, exiton, 269, 244);
				}
				
				if (pad.Buttons & PSP_CTRL_UP) {
					if (mainmenu2 == 1) {
						if (mainmenu > 1) {
							mainmenu--;
							for(wait=0; wait<7; wait++) {
								sceDisplayWaitVblankStart();
							}
						}
					}
				}	

				if (pad.Buttons & PSP_CTRL_DOWN) {
					if (mainmenu2 == 1) {
						if (mainmenu < 3) {
							mainmenu++;
							for(wait=0; wait<7; wait++) {
								sceDisplayWaitVblankStart();
							}
						}
					}
				}
				if (pad.Buttons & PSP_CTRL_LEFT) {
					mainmenu2 = 2;
					for(wait=0; wait<7; wait++) {
						 sceDisplayWaitVblankStart();
					}
				}
				if (pad.Buttons & PSP_CTRL_RIGHT) {
					mainmenu2 = 1;
					for(wait=0; wait<7; wait++) {
						 sceDisplayWaitVblankStart();
					}
				}
				if (pad.Buttons & PSP_CTRL_CROSS) {
					if (mainmenu2 == 2) break;
					if (mainmenu2 == 1) {
						if (mainmenu == 1) dialogbox = 2;
						if (mainmenu == 2) dialogbox = 3;
						if (mainmenu == 3) dialogbox = 4;
					}
				}
			}
		}
		if(dialogbox == 2) {
			if(UMDmenu == 1) {
				umdselect = 29;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(UMDmenu == 2) {
				umdselect = 39;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(UMDmenu == 3) {
				umdselect = 52;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(UMDmenu == 4) {
				umdselect = 62;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(UMDmenu == 5) {
				umdselect = 480;
				blitAlphaImageToScreen(0, 0, 50, 25, exiton, 269, 244);
			}

			blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244);
			blitAlphaImageToScreen(0, 0, 106, 272, taskbar1, 374, 0);
			blitAlphaImageToScreen(0, 0, 374, 243, umd, 3, 3);
			blitAlphaImageToScreen(0, 0, 215, 10, select, 24, umdselect);
			blitAlphaImageToScreen(0, 0, 374, 243, umdmenu, 3, 3);

			if (pad.Buttons & PSP_CTRL_DOWN) {
				if (UMDmenu < 5) {
					UMDmenu++;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_UP) {
				if (UMDmenu > 1) {
					UMDmenu--;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}

			if (pad.Buttons & PSP_CTRL_CROSS) {
				if (UMDmenu == 1) {
					UMDcheck = sceUmdCheckMedium(0);

					if (UMDcheck == 0) { 		
						blitAlphaImageToScreen(0, 0, 480, 272, insertumd, 0, 0);
						flipScreen();
					}
					
					while (UMDcheck == 0) {
						UMDcheck = sceUmdCheckMedium(0);
					}
	
					UMDcheck = sceUmdActivate(1, "disc0:");
					
					
					sceIoUnassign("flash0:");
					sceIoUnassign("flash1:");
					sceIoAssign("flash0:", "msstor0p1:/" , "fatms0:/f0/", IOASSIGN_RDWR, NULL, 0);
					sceIoAssign("flash1:", "msstor0p1:/" , "fatms0:/f1/", IOASSIGN_RDWR, NULL, 0);

					scePowerSetClockFrequency(333, 333, 166);
					
					sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
				if (UMDmenu == 2) {
					UMDcheck = sceUmdCheckMedium(0);

					if (UMDcheck == 0) { 		
						blitAlphaImageToScreen(0, 0, 480, 272, insertumd, 0, 0);
						flipScreen();
					}
					
					while (UMDcheck == 0) {
						UMDcheck = sceUmdCheckMedium(0);
					}
	
					UMDcheck = sceUmdActivate(1, "disc0:");
					
					
					sceIoUnassign("flash0:");
					sceIoUnassign("flash1:");
					sceIoAssign("flash0:", "msstor0p1:/" , "fatms0:/f0/", IOASSIGN_RDWR, NULL, 0);
					sceIoAssign("flash1:", "msstor0p1:/" , "fatms0:/f1/", IOASSIGN_RDWR, NULL, 0);

					scePowerSetClockFrequency(266, 266, 133);

					sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
				if (UMDmenu == 3) {
					UMDcheck = sceUmdCheckMedium(0);

					if (UMDcheck == 0) { 		
						blitAlphaImageToScreen(0, 0, 480, 272, insertumd, 0, 0);
						flipScreen();
					}
					
					while (UMDcheck == 0) {
						UMDcheck = sceUmdCheckMedium(0);
					}
	
					UMDcheck = sceUmdActivate(1, "disc0:");

					scePowerSetClockFrequency(333, 333, 166);

					sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
				if (UMDmenu == 4) {
					UMDcheck = sceUmdCheckMedium(0);

					if (UMDcheck == 0) { 		
						blitAlphaImageToScreen(0, 0, 480, 272, insertumd, 0, 0);
						flipScreen();
					}
					
					while (UMDcheck == 0) {
						UMDcheck = sceUmdCheckMedium(0);
					}
	
					UMDcheck = sceUmdActivate(1, "disc0:");

					scePowerSetClockFrequency(266, 266, 133);

					sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
				if (UMDmenu == 5) break;
			}

			if (pad.Buttons & PSP_CTRL_CIRCLE) dialogbox = 5;
		}
   
		if(dialogbox == 3) {
			if(XMBmenu == 1) {
				xmbselect = 29;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 2) {
				xmbselect = 39;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 3) {
				xmbselect = 52;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 4) {
				xmbselect = 62;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 5) {
				xmbselect = 75;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 6) {
				xmbselect = 85;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 7) {
				xmbselect = 98;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 8) {
				xmbselect = 108;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(XMBmenu == 9) {
				xmbselect = 480;
				blitAlphaImageToScreen(0, 0, 50, 25, exiton, 269, 244);
			}

			blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244);
			blitAlphaImageToScreen(0, 0, 106, 272, taskbar2, 374, 0);
			blitAlphaImageToScreen(0, 0, 374, 243, xmb, 3, 3);
			blitAlphaImageToScreen(0, 0, 215, 10, select, 24, xmbselect);
			blitAlphaImageToScreen(0, 0, 374, 243, xmbmenu, 3, 3);

			if (pad.Buttons & PSP_CTRL_DOWN) {
				if (XMBmenu < 9) {
					XMBmenu++;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_UP) {
				if (XMBmenu > 1) {
					XMBmenu--;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}

			if (pad.Buttons & PSP_CTRL_CROSS) {
				if (XMBmenu == 1) write_file("./WAVE_EFFECT/ON/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				if (XMBmenu == 2) write_file("./WAVE_EFFECT/OFF/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				if (XMBmenu == 3) {
					write_file("ms0:/01.bmp","flash0:/vsh/resource/01.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/02.bmp","flash0:/vsh/resource/02.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/03.bmp","flash0:/vsh/resource/03.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/04.bmp","flash0:/vsh/resource/04.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/05.bmp","flash0:/vsh/resource/05.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/06.bmp","flash0:/vsh/resource/06.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/07.bmp","flash0:/vsh/resource/07.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/08.bmp","flash0:/vsh/resource/08.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/09.bmp","flash0:/vsh/resource/09.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/10.bmp","flash0:/vsh/resource/10.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/11.bmp","flash0:/vsh/resource/11.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/12.bmp","flash0:/vsh/resource/12.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 4) {
					write_file("./DEFAULT_BG/01.bmp","flash0:/vsh/resource/01.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/02.bmp","flash0:/vsh/resource/02.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/03.bmp","flash0:/vsh/resource/03.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/04.bmp","flash0:/vsh/resource/04.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/05.bmp","flash0:/vsh/resource/05.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/06.bmp","flash0:/vsh/resource/06.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/07.bmp","flash0:/vsh/resource/07.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/08.bmp","flash0:/vsh/resource/08.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/09.bmp","flash0:/vsh/resource/09.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/10.bmp","flash0:/vsh/resource/10.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/11.bmp","flash0:/vsh/resource/11.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("./DEFAULT_BG/12.bmp","flash0:/vsh/resource/12.bmp", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 5) {
					write_file("ms0:/topmenu_plugin.rco","flash0:/vsh/resource/topmenu_plugin.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 6) {
					write_file("./DEFAULT_MENU/topmenu_plugin.rco","flash0:/vsh/resource/topmenu_plugin.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 7) {
					write_file("./CORRUPTED_ICON/ON/game_plugin.rco","flash0:/vsh/resource/game_plugin.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 8) {
					write_file("./CORRUPTED_ICON/OFF/game_plugin.rco","flash0:/vsh/resource/game_plugin.rco", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (XMBmenu == 9) break;
			}

			if (pad.Buttons & PSP_CTRL_CIRCLE) dialogbox = 5;
		}
		if(dialogbox == 4) {
			if(PSIXmenu == 1) {
				psixselect = 29;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(PSIXmenu == 2) {
				psixselect = 39;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(PSIXmenu == 3) {
				psixselect = 52;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(PSIXmenu == 4) {
				psixselect = 62;
				blitAlphaImageToScreen(0, 0, 50, 25, exit, 269, 244);	
			}
			if(PSIXmenu == 5) {
				psixselect = 480;
				blitAlphaImageToScreen(0, 0, 50, 25, exiton, 269, 244);
			}
			blitAlphaImageToScreen(0, 0, 374, 25, statusbar, 3, 244);
			blitAlphaImageToScreen(0, 0, 106, 272, taskbar3, 374, 0);
			blitAlphaImageToScreen(0, 0, 374, 243, psix, 3, 3);
			blitAlphaImageToScreen(0, 0, 215, 10, select, 24, psixselect);
			blitAlphaImageToScreen(0, 0, 374, 243, psixmenu, 3, 3);
			if (pad.Buttons & PSP_CTRL_DOWN) {
				if (PSIXmenu < 5) {
					PSIXmenu++;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_UP) {
				if (PSIXmenu > 1) {
					PSIXmenu--;
					for(wait=0; wait<7; wait++) {
						sceDisplayWaitVblankStart();
					}
				}
			}

			if (pad.Buttons & PSP_CTRL_CROSS) {
				if (PSIXmenu == 1) {
					write_file("./PSIX_VERSION/100_PSP/EBOOT.PBP","ms0:/PSP/GAME/psix/EBOOT.PBP", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (PSIXmenu == 2) {
					write_file("./PSIX_VERSION/150_PSP/EBOOT.PBP","ms0:/PSP/GAME/psix/EBOOT.PBP", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (PSIXmenu == 3) {
					write_file("ms0:/background.png","ms0:/PSP/GAME/psix/background.png", flashingstatusbar, completestatusbar, failedstatusbar);
					write_file("ms0:/background.jpg","ms0:/PSP/GAME/psix/background.jpg", flashingstatusbar, completestatusbar, failedstatusbar);
				}
				if (PSIXmenu == 4) {
					sceIoRemove("ms0:/PSP/GAME/psix/background.png");
					sceIoRemove("ms0:/PSP/GAME/psix/background.jpg");
				}
				if (PSIXmenu == 5) break;
			}

			if (pad.Buttons & PSP_CTRL_CIRCLE) dialogbox = 5;
		}
		if(dialogbox == 5) {
			if(mainmenu2 == 1) {
				blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244);
				blitAlphaImageToScreen(0, 0, 50, 28, exit, 269, 244);
				if(mainmenu == 1)blitAlphaImageToScreen(0, 0, 106, 272, taskbar1, 374, 0);
				if(mainmenu == 2)blitAlphaImageToScreen(0, 0, 106, 272, taskbar2, 374, 0);
				if(mainmenu == 3)blitAlphaImageToScreen(0, 0, 106, 272, taskbar3, 374, 0);
			}

			if(mainmenu2 == 2) {
					blitAlphaImageToScreen(0, 0, 100, 272, taskbar0, 380, 0);
					blitAlphaImageToScreen(0, 0, 266, 25, statusbar, 3, 244); 
					blitAlphaImageToScreen(0, 0, 50, 25, exiton, 269, 244);
			}
			
			if (pad.Buttons & PSP_CTRL_UP) {
				if (mainmenu2 == 1) {
					if (mainmenu > 1) {
						mainmenu--;
						for(wait=0; wait<7; wait++) {
							sceDisplayWaitVblankStart();
						}
					}
				}
			}	

			if (pad.Buttons & PSP_CTRL_DOWN) {
				if (mainmenu2 == 1) {
					if (mainmenu < 3) {
						mainmenu++;
						for(wait=0; wait<7; wait++) {
							sceDisplayWaitVblankStart();
						}
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_LEFT) {
				if (mainmenu2 < 2) {
					mainmenu2++;
					for(wait=0; wait<7; wait++) {
						 sceDisplayWaitVblankStart();
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_RIGHT) {
				if (mainmenu2 > 1) {
					mainmenu2--;
					for(wait=0; wait<7; wait++) {
						 sceDisplayWaitVblankStart();
					}
				}
			}
			if (pad.Buttons & PSP_CTRL_CROSS) {
				if (mainmenu2 == 2) break;
				if (mainmenu2 == 1) {
					if (mainmenu == 1) dialogbox = 2;
					if (mainmenu == 2) dialogbox = 3;
					if (mainmenu == 3) dialogbox = 4;
				}
			}
		}
		flipScreen();
	}
		
	sceKernelExitGame();
	return 0;
}

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

void check(const char* zFile){
	int fd3;
	fd3 = sceIoOpen(zFile, PSP_O_RDONLY, 0);
	if(fd3 < 0) {
		exist = 0;
	   }
	else {
			 exist = 1;
	   }
	sceIoClose(fd3);
} 

void write_file(const char *readpath, const char *writepath, Image* write, Image* writecomplete, Image* writefailed) {
	int wait = 0;

	blitAlphaImageToScreen(0, 0, 266, 25, write, 3, 244);
	flipScreen();
	check(readpath);
	if(exist == 1) {
		int fdin;
		int fdout;	

		fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
		if(fdin >= 0)
		{
			int bytesRead = 1;
			fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
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
		blitAlphaImageToScreen(0, 0, 266, 25, writecomplete, 3, 244); 
		flipScreen();
	}
	if(exist == 0) { 
		blitAlphaImageToScreen(0, 0, 266, 25, writefailed, 3, 244); 
		flipScreen();
	}
	for(wait=0; wait<15; wait++) {
		sceDisplayWaitVblankStart();
	}
}
