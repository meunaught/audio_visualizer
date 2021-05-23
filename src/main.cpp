#include "utils.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

complexData data(SAMPLES);
RecordedData recData;

SDL_AudioDeviceID recordingDeviceId = 0;
SDL_AudioDeviceID playbackDeviceId = 0;
SDL_AudioSpec ReceivedRecordingSpec;
SDL_AudioSpec ReceivedPlaybackSpec;

TTF_Font *font;
TTF_Font *nwfont;
SDL_Rect disp;
SDL_Surface *surf;
SDL_Texture *tx0, *tx10, *tx11, *tx12, *tx13, *tx2, *txp;

SDL_Texture *tpause, *tplay, *tstop, *treplay;
SDL_Texture *pauseTex;
SDL_Rect pauserect, stoprect;

bool MODE = true;

int main(int argc, char **argv) {
#ifdef OS2
      putenv("SDL_AUDIODRIVER=DirectSound");
      chdir("..");
#endif
      int option_index = 0, useMode = -1;
      char *file_stream = NULL;

      if (!init()) {
            printf("%s\n", SDL_GetError());
            exit(1);
      }

      load();

      while ((option_index = getopt(argc, argv, ":f:rR")) != -1) {
            switch (option_index) {
                  case 'f':
                        file_stream = optarg;
                        useMode = 1;
                        break;
                  case 'r':
                        useMode = 2;
                        break;
                  case 'R':
                        useMode = 3;
                        break;
                  default:
                        break;
            }
      }

      if (useMode == -1) useMode = UI();

      if (useMode == 1)
            musicMode(file_stream);
      else if (useMode == 2)
            recordMode();
      else if (useMode == 3)
            realTimeMode();
      else
            printf("%s\n", SDL_GetError());

      quit();
      return 0;
}
