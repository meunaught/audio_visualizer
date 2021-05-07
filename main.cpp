#include "utils.h"

int main(int argc, char *argv[]) {
      int option_index = 0, useMode = 2;
      char *file_stream = NULL;

      while ((option_index = getopt(argc, argv, ":f:R")) != -1) {
            switch (option_index) {
            case 'f':
                  file_stream = optarg;
                  useMode = 1;
                  break;
            case 'R':
                  useMode = 3;
                  break;
            default:
                  break;
            }
      }

      if(!init()) {
            printf("%s\n", SDL_GetError());
            exit(1);
      }


      if(useMode == 1) musicMode(file_stream);
	else if(useMode == 2) recordMode();
      else if(useMode == 3) realTimeMode();

      quit();
      return 0;
}
