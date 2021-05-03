#include "utils.h"

bool init(screen &window) {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
            printf("video and timer: %s\n",SDL_GetError());
            return false;
      }
      window.win = SDL_CreateWindow("Musico",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      if (window.win == NULL) {
            printf("window: %s\n",SDL_GetError());
            SDL_Quit();
            return false;
      }
      window.renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
      window.rend = SDL_CreateRenderer(window.win, -1, window.renderFlags);
      if (window.rend == NULL) {
            printf("renderer: %s\n",SDL_GetError());
            SDL_DestroyWindow(window.win);
            SDL_Quit();
            return false;
      }
      return true;
}

void getAudioData(audioData song, complexData &data){
	// temporary random data 
	for (int i = 0; i < SIZE; ++i) {
		double scale = 1 - cos(2 * pi * i / SIZE);
		data.in[i][0] = double(rand() % (WINDOW_HEIGHT / 15)) * scale;
		if (rand() & 1) data.in[i][0] *= -1;
		data.in[i][1] = 0;
	}
	// temporary random data 	
}




