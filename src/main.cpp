#include "utils.h"

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

      if (useMode == 1) musicMode(file_stream);
      else if (useMode == 2) recordMode();
      else if (useMode == 3) realTimeMode();
      else printf("%s\n", SDL_GetError());

      quit();
      return 0;
}
