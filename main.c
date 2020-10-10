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

#define printf	pspDebugScreenPrintf
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

PSP_MODULE_INFO("FlashMod", 0, 1, 1);

int exist = 0;

int exit_callback(int arg1, int arg2, void *common);

int CallbackThread(SceSize args, void *argp); 

int SetupCallbacks(void);

void screenblit(int posx, int posy, int sizex, int sizey, Image* imagevar);

void lookforumd();

char write_buffer[128*1024];

void check(const char* zFile);

void write_file(const char *readpath, const char *writepath);

int main() {
	SetupCallbacks();
	initGraphics();

	int C = 0;

	char copyright_buffer[200];
	Image* copyright;
	sprintf(copyright_buffer, "./Images/Copyright.png");
	copyright = loadImage(copyright_buffer);

	while(C < 50){
		screenblit(0, 0, 480, 272, copyright);
		flipScreen();
		C++;
	}

	int i = 0; int h = 0; SceCtrlData pad; int menu = 1; int fd;
	char menu_one_buffer[200]; char menu_two_buffer[200]; char menu_three_buffer[200]; char menu_four_buffer[200]; char menu_five_buffer[200]; char menu_six_buffer[200]; char menu_seven_buffer[200]; char menu_eight_buffer[200]; char menu_nine_buffer[200]; char flash_complete_buffer[200]; char insert_umd_buffer[200];
        Image* menu_one; Image* menu_two; Image* menu_three; Image* menu_four; Image* menu_five; Image* menu_six; Image* menu_seven; Image* menu_eight; Image* menu_nine; Image* flash_complete; Image* insert_umd;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	sceIoUnassign("flash0:"); 
	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);

	sprintf(menu_one_buffer,"./Images/Menu1.png"); sprintf(menu_two_buffer,"./Images/Menu2.png"); sprintf(menu_three_buffer,"./Images/Menu3.png"); sprintf(menu_four_buffer,"./Images/Menu4.png"); sprintf(menu_five_buffer,"./Images/Menu5.png"); sprintf(menu_six_buffer,"./Images/Menu6.png"); sprintf(menu_seven_buffer,"./Images/Menu7.png"); sprintf(menu_eight_buffer,"./Images/Menu8.png"); sprintf(menu_nine_buffer,"./Images/Menu9.png"); sprintf(flash_complete_buffer,"./Images/FlashComplete.png"); sprintf(insert_umd_buffer,"./Images/UMD.png");
	menu_one = loadImage(menu_one_buffer); menu_two = loadImage(menu_two_buffer); menu_three = loadImage(menu_three_buffer); menu_four = loadImage(menu_four_buffer); menu_five = loadImage(menu_five_buffer); menu_six = loadImage(menu_six_buffer); menu_seven = loadImage(menu_seven_buffer); menu_eight = loadImage(menu_eight_buffer); menu_nine = loadImage(menu_nine_buffer); flash_complete = loadImage(flash_complete_buffer); insert_umd = loadImage(insert_umd_buffer);

	screenblit(0, 0, 480, 272, menu_one);
		
	while (1) {
		sceCtrlReadBufferPositive(&pad, 1);

		if (menu == 1) {
			screenblit(0, 0, 480, 272, menu_one);
			flipScreen();
		}
		
		if (menu == 2) {
			screenblit(0, 0, 480, 272, menu_two);
			flipScreen();
		}

		if (menu == 3) {
			screenblit(0, 0, 480, 272, menu_three);
			flipScreen();
		}
		
		if (menu == 4) {
			screenblit(0, 0, 480, 272, menu_four);
			flipScreen();
		}

		if (menu == 5) {
			screenblit(0, 0, 480, 272, menu_five);
			flipScreen();
		}

		if (menu == 6) {
			screenblit(0, 0, 480, 272, menu_six);
			flipScreen();
		}

		if (menu == 7) {
			screenblit(0, 0, 480, 272, menu_seven);
			flipScreen();
		}

		if (menu == 8) {
			screenblit(0, 0, 480, 272, menu_eight);
			flipScreen();
		}

		if (menu == 9) {
			screenblit(0, 0, 480, 272, menu_nine);
			flipScreen();
		}
		
		if (pad.Buttons & PSP_CTRL_UP) {
			if (menu > 1) {
				menu--;
			        for(i=0; i<10; i++) {
		                	sceDisplayWaitVblankStart();
				}
          		}
		}

		if (pad.Buttons & PSP_CTRL_DOWN) {
			if (menu < 9) {
				menu++;
			        for(i=0; i<10; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
		}

		if (pad.Buttons & PSP_CTRL_CROSS) {
			if (menu == 1) {
				
				h = sceUmdCheckMedium(0);		
	
				if (h == 0) { 
			
					screenblit(0,0,480,272,insert_umd);
					flipScreen();
				}

				while (h == 0) {
					h = sceUmdCheckMedium(0);
				}

				h = sceUmdActivate(1, "disc0:");

				sceIoUnassign("flash0:");
				sceIoUnassign("flash1:");
				sceIoAssign("flash0:", "msstor0p1:/" , "fatms0:/Flash0/", IOASSIGN_RDWR, NULL, 0);
				sceIoAssign("flash1:", "msstor0p1:/" , "fatms0:/Flash1/", IOASSIGN_RDWR, NULL, 0);

				scePowerSetClockFrequency(333, 333, 166);

				fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);

				char game_id[10];
				sceIoRead(fd, game_id, 10);
				sceIoClose(fd);

				fd = sceIoOpen("./GAMES/UCUS-98615/UMD_DATA.BIN", PSP_O_RDONLY, 0777);

				char socom_game_id[10];
				sceIoRead(fd, socom_game_id, 10);
				sceIoClose(fd);

				if(game_id == socom_game_id) {
					sceKernelLoadExec("./GAMES/UCUS-98615/BOOT.BIN",0);
				}
				else
				{
					sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
			}
			if (menu == 2) {
				h = sceUmdCheckMedium(0);		
	
				if (h == 0) { 
			
					screenblit(0,0,480,272,insert_umd);
					flipScreen();
				}

				while (h == 0) {
					h = sceUmdCheckMedium(0);
				}

				h = sceUmdActivate(1, "disc0:");

				scePowerSetClockFrequency(333, 333, 166);
				sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
			}

			if (menu == 3) {
				write_file("./ASURA_EFFECT_ON/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 4) {
				write_file("./ASURA_EFFECT_OFF/system_plugin_bg.rco","flash0:/vsh/resource/system_plugin_bg.rco");

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 5) {
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

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 6) {
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

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 7) {
				write_file("ms0:/topmenu_plugin.rco","flash0:/vsh/resource/topmenu_plugin.rco");

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 8) {
				write_file("./DEFAULT_MENU/topmenu_plugin.rco","flash0:/vsh/resource/topmenu_plugin.rco");

				screenblit(0, 0, 480, 272, flash_complete);
				flipScreen();

			        for(i=0; i<50; i++) {
		                	sceDisplayWaitVblankStart();
				}
			}
			if (menu == 9) {
				break;
			}
		}	
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

void screenblit(int posx, int posy, int sizex, int sizey, Image* imagevar) {
	int x = 0;
        int y = 0;
        sceDisplayWaitVblankStart();

	while (x < sizex) {
		while (y < sizey) {
			blitAlphaImageToScreen(posx ,posy ,sizex , sizey, imagevar, x, y);
			y += sizey;
		}
		x += sizex;
		y = 0;
	}
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

void write_file(const char *readpath, const char *writepath) {
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
	}
}
