#include "visualize.h"

int main(int argc, char *argv[]) {
      srand(time(0));
      char *songName = NULL; 
	
      int opt = 0;
      while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
		case 'f':
			songName = optarg;
			break;
		default:
			puts("NO FILE SPECIFIED");
			return 0;
			break;
		}
	}
      
      screen window;
      complexData data(SIZE);

      if (!init(window)) {
            puts("Failed to initialize");
            exit(0);
      }

      audioData song(songName);

      bool playing = true, mode = true, pause = true;

      song.play(pause);

      while (playing) {
            SDL_Event event;

            getAudioData(song, data);

            if (mode) visualizeWave(window, data);
            else visualizeBars(window, data);
                  
            SDL_RenderPresent(window.rend);
            SDL_Delay(75);

            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                        playing = false;
                  }
                  if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                              playing = false;
                              break;
                        case SDLK_m:
                              mode ^= 1;
                              break;
                        case SDLK_p:
                              pause ^= 1;
                              song.play(pause);
                              break;
                        default:
                              break;
                        }
                  }
            }
      }

      window.quit();
      data.quit();
      song.quit();
      SDL_Quit();

      return 0;
}