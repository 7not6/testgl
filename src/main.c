#include "common.h"

bool quit = false;

int main(int argc, char *argv[]) {

	if (argc !=1)
		die("usage: %s\n" , argv[0]);

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
   	{
       		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
       		exit(1);
    	}
	
	atexit(cleanup);

	init();
	
	while (!quit) {
		
		doFps();
		    
		doInput();
									
		doScene();	
	}

    return EXIT_SUCCESS;
}
