#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SDL_Surface *screen = NULL;
SDL_Surface *holder = NULL;
SDL_Surface *copyright = NULL; 
SDL_Surface *background = NULL; 
SDL_Surface *taskbar = NULL;
SDL_Surface *taskbarumd = NULL; 
SDL_Surface *taskbarxmb = NULL;
SDL_Surface *taskbarpsix = NULL;
SDL_Surface *statusbar = NULL; 
SDL_Surface *escape = NULL; 
SDL_Surface *escapeon = NULL; 
SDL_Surface *dialogboxumd = NULL; 

int screenblit(int x, int y, SDL_Surface *source, SDL_Surface *dest, SDL_Rect *clip = NULL);
int FadeEffect(SDL_Surface *source, SDL_Surface *dest, Uint32 time, bool fadeout);
int CrossFadeEffect (SDL_Surface* dest, SDL_Surface* source1, int x, int y, SDL_Surface* source2, int x2, int y2, Uint32 time);
int FadeUMD (SDL_Surface* source, SDL_Surface* dest, int x, int y, Uint32 time);

int main( int argc, char* args[] ) {
    int taskbarx = 480;
    int statusbary = 272;
    int selectionAppeared = 0;
    int mainmenu1 = 1;
    int mainmenu2 = 1;
    int breakfromwhile = 0;
    int dialogbox = 1;
    int umdmenu = 1;
     
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Event event; 

    screen = SDL_SetVideoMode(480,272,32, SDL_HWSURFACE); 
    copyright = IMG_Load("images/copyright.png");
    background = IMG_Load("images/background.png");


    SDL_Delay(1000);
    screenblit(0,0,copyright,screen,NULL);
    FadeEffect(copyright,screen,2000,false,NULL);
    
    screenblit(0,0,copyright,screen,NULL);
    SDL_Flip(screen);
    
    SDL_Delay(5000);
      
    CrossFadeEffect(screen,copyright,0,0,background,0,0,2000);
    
    holder = IMG_Load("images/holder.png");
    taskbar = IMG_Load("images/taskbar.png");   
    taskbarumd = IMG_Load("images/taskbar-umd.png");
    taskbarxmb = IMG_Load("images/taskbar-xmb.png");
    taskbarpsix = IMG_Load("images/taskbar-psix.png");
    statusbar = IMG_Load("images/statusbar.png");
    escape = IMG_Load("images/exit.png");    
    escapeon = IMG_Load("images/exiton.png");   
    dialogboxumd = IMG_Load("images/dialogbox-umd.png"); 
    
    screenblit(0,0,background,screen,NULL);
    SDL_Flip(screen);
    
    while(breakfromwhile == 0){
             if(dialogbox == 1) {
                          if(taskbarx > 374) {
                                      if(statusbary > 246) statusbary--; 
                                      taskbarx--;
                          }
             
                          screenblit(0,0,background,screen);

                          if(mainmenu2 == 1) {
                                      if(mainmenu1 == 1) screenblit(taskbarx,0,taskbarumd,screen,NULL);
                                      if(mainmenu1 == 2) screenblit(taskbarx,0,taskbarxmb,screen,NULL);
                                      if(mainmenu1 == 3) screenblit(taskbarx,0,taskbarpsix,screen,NULL);
                                      screenblit(274,statusbary,escape,screen);
                                      }
                          if(mainmenu2 == 2) {
                                      screenblit(380,0,taskbar,screen);
                                      screenblit(274,statusbary,escapeon,screen);
                          }
             
                          screenblit(3,statusbary,statusbar,screen);
             
                          while(SDL_PollEvent(&event)) {
                                      if(event.type == SDL_KEYDOWN) {
                                          if (event.key.keysym.sym == SDLK_UP) {
	                                         if (mainmenu2 == 1) {
						                        if (mainmenu1 > 1) {
							                       mainmenu1--;
                                                }
					                         }
                                          }
                                          if (event.key.keysym.sym == SDLK_DOWN) {
	                                         if (mainmenu2 == 1) {
						                        if (mainmenu1 < 3) {
							                       mainmenu1++;
                                                }
					                         }
                                          }
                                          if (event.key.keysym.sym == SDLK_LEFT) {
                                             mainmenu2 = 2;
                                          }
                                          if (event.key.keysym.sym == SDLK_RIGHT) {
                                             mainmenu2 = 1;
                                          }
                                          if (event.key.keysym.sym == SDLK_RETURN) {
                                             if(mainmenu2 == 1) {
                                                if(mainmenu1 == 1) {
                                                   FadeUMD (dialogboxumd,screen,3,3,2000);
                                                   dialogbox = 2;
                                                }
                                             }
                                             if(mainmenu2 == 2) breakfromwhile = 1;
                                          }
                                       
                                       }
                          }
             }
             if(dialogbox == 2) {
                          screenblit(0,0,background,screen);
                          screenblit(taskbarx,0,taskbarumd,screen);
                          screenblit(3,statusbary,statusbar,screen);
                          screenblit(3,3,dialogboxumd,screen);
                          
                          if(umdmenu == 1) {
                                     screenblit(274,statusbary,escape,screen);
                          }
                          if(umdmenu == 2) {
                                     screenblit(274,statusbary,escape,screen);
                          }
                          if(umdmenu == 3) {
                                     screenblit(274,statusbary,escape,screen);
                          }
                          if(umdmenu == 4) {
                                     screenblit(274,statusbary,escape,screen);
                          }
                          if(umdmenu == 5) {
                                     screenblit(274,statusbary,escape,screen);
                          }
                                       
             }            
             SDL_Flip(screen);                    
    }
    
    SDL_FreeSurface(screen);
    
    SDL_Quit();
    
    return 0; 
}

int screenblit(int x, int y, SDL_Surface *source, SDL_Surface *dest) {
     SDL_Rect *clip = NULL;
     SDL_Rect Offset;
     Offset.x = x;
     Offset.y = y;
     SDL_BlitSurface(source,clip,dest,&Offset);
}

int FadeEffect(SDL_Surface* source, SDL_Surface* dest, Uint32 time, bool fadeout)
{	
 		SDL_Surface *black = NULL; // Surface for fading
 		SDL_Surface *screencopy = NULL; // Surface for copying the screen
 		Uint32 OldTime, CurrentTime, TimeMSec;
 		double alpha; // Variable for alpha number

 		Uint32 flags = SDL_SRCALPHA | SDL_HWSURFACE;
 					 (dest->flags & SDL_HWSURFACE);
					 black = SDL_CreateRGBSurface (flags, dest->w, dest->h, dest->format->BitsPerPixel, dest->format->Rmask, dest->format->Gmask, dest->format->Bmask, dest->format->Amask);					
		
		SDL_FillRect (black, NULL, SDL_MapRGB (dest->format, 0,0,0)); // Fills surface black with black-> #000000
	 					screencopy = SDL_CreateRGBSurface (flags, dest->w, dest->h, dest->format->BitsPerPixel, dest->format->Rmask, dest->format->Gmask, dest->format->Bmask, dest->format->Amask);
						
						
		screenblit(0,0,dest,screencopy);
		source = screencopy;
		
		OldTime = SDL_GetTicks();
		CurrentTime = OldTime;
		
		TimeMSec = time;
		
		if(fadeout == true)
		{
		 					 alpha = 0;
		 					 			 while (alpha < 256)
										 {
                           screenblit(0,0,source,dest);
                           SDL_SetAlpha(black,SDL_SRCALPHA, (Uint8) alpha);
                           screenblit(0,0,black,dest);
                           
                           OldTime = CurrentTime;
                           CurrentTime = SDL_GetTicks();
                           
                           SDL_Flip(screen);
                           
                           alpha += 255 *((double) (CurrentTime-OldTime) / TimeMSec);
                      }
    }
    else
    {
               alpha = 256;           
		 			 while (alpha > 0)
			 		 {
                           screenblit(0,0,source,dest);
                           SDL_SetAlpha(black,SDL_SRCALPHA, (Uint8) alpha);
                           screenblit(0,0,black,dest);
                           
                           OldTime = CurrentTime;
                           CurrentTime = SDL_GetTicks();
                           
                           SDL_Flip(screen);
                           
                           alpha -= 255 *((double) (CurrentTime-OldTime) / TimeMSec);
                      }  
    }   
    
}

int CrossFadeEffect (SDL_Surface* dest, SDL_Surface* source1, int x, int y, SDL_Surface* source2, int x2, int y2, Uint32 time) 
{  
   Uint32 OldTime, CurrentTime, TimeMS;
   double alpha ;

   OldTime = SDL_GetTicks () ;
   CurrentTime = OldTime;

   TimeMS = time;
   alpha = 0.0 ;
  
		while (alpha < 256.0) 
  		{  
					screenblit(x,y,source1,dest);
					SDL_SetAlpha (source2, SDL_SRCALPHA, (Uint8) alpha) ;
					screenblit(x2,y2,source2,dest);
					OldTime = CurrentTime ;
					CurrentTime = SDL_GetTicks () ;

       				SDL_Flip(screen);

                    alpha += 255 * ((double) (CurrentTime - OldTime) / TimeMS) ;
        }
}

int FadeUMD (SDL_Surface* source, SDL_Surface* dest, int x, int y, Uint32 time) {
    Uint32 OldTime, CurrentTime, TimeMS;
    double alpha ;
    
    OldTime = SDL_GetTicks() ;
    CurrentTime = OldTime;
    
    TimeMS = time;
    alpha = 0.0;
    
    while(alpha > 256.0) {
                screenblit(0,0,background,screen);
                screenblit(374,0,taskbarumd,screen);
                screenblit(3,246,statusbar,screen);
                SDL_SetAlpha(source, SDL_SRCALPHA, (Uint8) alpha);
                screenblit(x,y,source,dest);
                OldTime = CurrentTime ;
				CurrentTime = SDL_GetTicks () ;

       			SDL_Flip(screen);

                alpha += 255 * ((double) (CurrentTime - OldTime) / TimeMS) ;
    }
}
