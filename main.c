#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <pspgu.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

#define printf	pspDebugScreenPrintf
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

PSP_MODULE_INFO("FlashMod", 0, 1, 1);


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

char write_buffer[128*1024];

void write_file(const char *readpath, const char *writepath)
{
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
}


int main() {
	int i = 0; SceCtrlData pad;
	int menu = 1;
        char menu_one_buffer[200];
        char menu_two_buffer[200];
        char menu_three_buffer[200];
        char menu_four_buffer[200];
        char menu_five_buffer[200];
	char flash_complete_buffer[200];
        Image* menu_one;	
        Image* menu_two;
        Image* menu_three;
        Image* menu_four;
        Image* menu_five;
	Image* flash_complete;

	SetupCallbacks();
	initGraphics();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	sceIoUnassign("flash0:"); 
	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);

	sprintf(menu_one_buffer,"menu1.png");
	menu_one = loadImage(menu_one_buffer);
	sprintf(menu_two_buffer,"menu2.png");
	menu_two = loadImage(menu_two_buffer);
	sprintf(menu_three_buffer,"menu3.png");
	menu_three = loadImage(menu_three_buffer);
	sprintf(menu_four_buffer,"menu4.png");
	menu_four = loadImage(menu_four_buffer);
	sprintf(menu_five_buffer,"menu5.png");
	menu_five = loadImage(menu_five_buffer);
	sprintf(flash_complete_buffer,"complete.png");
	flash_complete = loadImage(flash_complete_buffer);

	int x = 0;
        int y = 0;
        sceDisplayWaitVblankStart();

	while (x < 480) {
		while (y < 272) {
			blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_one, x, y);
			y += 272;
		}
		x += 480;
		y = 0;
	}
	flipScreen();
		
	while (1) {
		sceCtrlReadBufferPositive(&pad, 1);

		if (menu == 1) {
			x = 0;
    			y = 0;
      			sceDisplayWaitVblankStart();

			while (x < 480) {
				while (y < 272) {
					blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_one, x, y);
					y += 272;
				}
				x += 480;
				y = 0;
			}
			flipScreen();
		}
		
		if (menu == 2) {
			x = 0;
    			y = 0;
      			sceDisplayWaitVblankStart();
	
			while (x < 480) {
				while (y < 272) {
					blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_two, x, y);
					y += 272;
				}
				x += 480;
				y = 0;
			}
			flipScreen();
		}

		if (menu == 3) {
			x = 0;
    			y = 0;
      			sceDisplayWaitVblankStart();

			while (x < 480) {
				while (y < 272) {
					blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_three, x, y);
					y += 272;
				}
				x += 480;
				y = 0;
			}
			flipScreen();
		}
		
		if (menu == 4) {
			x = 0;
    			y = 0;
      			sceDisplayWaitVblankStart();
	
			while (x < 480) {
				while (y < 272) {
					blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_four, x, y);
					y += 272;
				}
				x += 480;
				y = 0;
			}
			flipScreen();
		}

		if (menu == 5) {
			x = 0;
    			y = 0;
      			sceDisplayWaitVblankStart();
	
			while (x < 480) {
				while (y < 272) {
					blitAlphaImageToScreen(0 ,0 ,480 , 272, menu_five, x, y);
					y += 272;
				}
				x += 480;
				y = 0;
			}
			flipScreen();
		}
		
		if (pad.Buttons & PSP_CTRL_UP) {
			if (menu > 1) {
				menu--;
			        for(i=0; i<5; i++) {
		                	sceDisplayWaitVblankStart();
				}
          		}
		}

		if (pad.Buttons & PSP_CTRL_DOWN) {
			if (menu < 5) {
				menu++;
			        for(i=0; i<5; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
		}

		if (pad.Buttons & PSP_CTRL_CROSS) {
			if (menu == 1) {
				write_file("./ASURA_EFFECT_OFF/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");

				x = 0;
    				y = 0;
	      			sceDisplayWaitVblankStart();

				while (x < 480) {
					while (y < 272) {
						blitAlphaImageToScreen(0 ,0 ,480 , 272, flash_complete, x, y);
						y += 272;
					}
					x += 480;
					y = 0;
				}
				flipScreen();

			        for(i=0; i<30; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 2) {
				write_file("./ASURA_EFFECT_ON/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");

				x = 0;
    				y = 0;
	      			sceDisplayWaitVblankStart();

				while (x < 480) {
					while (y < 272) {
						blitAlphaImageToScreen(0 ,0 ,480 , 272, flash_complete, x, y);
						y += 272;
					}
					x += 480;
					y = 0;
				}
				flipScreen();

			        for(i=0; i<30; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 3) {
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

				x = 0;
    				y = 0;
	      			sceDisplayWaitVblankStart();

				while (x < 480) {
					while (y < 272) {
						blitAlphaImageToScreen(0 ,0 ,480 , 272, flash_complete, x, y);
						y += 272;
					}
					x += 480;
					y = 0;
				}
				flipScreen();

			        for(i=0; i<30; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 4) {
				write_file("./DEFAULT_BG/01.bmp","flash0:/vsh/resource/01.bmp");
				write_file("./DEFAULT_BG/02.bmp","flash0:/vsh/resource/02.bmp");
				write_file("./DEFAULT_BG/03.bmp","flash0:/vsh/resource/03.bmp");
				write_file("./DEFAULT_BG/04.bmp","flash0:/vsh/resource/04.bmp");
				write_file("./DEFAULT_BG/05.bmp","flash0:/vsh/resource/05.bmp");
				write_file("./DEFAULT_BG/06.bmp","flash0:/vsh/resource/06.bmp");
				write_file("./DEFAULT_BG/07.bmp","flash0:/vsh/resource/07.bmp");
				write_file("./DEFAULT_BG/08.bmp","flash0:/vsh/resource/08.bmp");
				write_file("./DEFAULT_BG/09.bmp","flash0:/vsh/resource/09.bmp");
				write_file("./DEFAULT_BG/10.bmp","flash0:/vsh/resource/10.bmp");
				write_file("./DEFAULT_BG/11.bmp","flash0:/vsh/resource/11.bmp");
				write_file("./DEFAULT_BG/12.bmp","flash0:/vsh/resource/12.bmp");

				x = 0;
    				y = 0;
	      			sceDisplayWaitVblankStart();

				while (x < 480) {
					while (y < 272) {
						blitAlphaImageToScreen(0 ,0 ,480 , 272, flash_complete, x, y);
						y += 272;
					}
					x += 480;
					y = 0;
				}
				flipScreen();

			        for(i=0; i<30; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 5) {
				break;
			}
		}	
	}
	sceKernelExitGame();
	return 0;
}
		
