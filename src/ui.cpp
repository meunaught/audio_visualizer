#include "tinyfiledialogs.h"
#include "utils.h"

int UI() {
      SDL_Surface *nwsurf = IMG_Load("res/stscr.png");
      if (nwsurf == NULL) {
            return 0;
      }
      SDL_Texture *nwtext = NULL;
      nwtext = SDL_CreateTextureFromSurface(renderer, nwsurf);

      font = TTF_OpenFont("res/pointy.ttf", 100);
      nwfont = TTF_OpenFont("res/rough.ttf", 100);

      if (font == NULL) {
            return 0;
      }
      SDL_Color col = {232, 232, 232};
      nwsurf = IMG_Load("res/cover.png");
      SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = IMG_Load("res/playmusic.png");
      SDL_Texture *text1 = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = IMG_Load("res/recordaudio.png");
      SDL_Texture *text2 = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = IMG_Load("res/realtime.png");
      SDL_Texture *text3 = SDL_CreateTextureFromSurface(renderer, nwsurf);

      SDL_Rect rect, txrect, rect2, opt1, opt2, opt3;
      int h, w;

      int st = 0, colst = 20;
      bool quit = false;
      int val = 0;
      while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true, thaam = true;

                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if (opt1.x <= x && x <= opt1.x + opt1.w && opt1.y <= y && y <= opt1.y + opt1.h)
                              val = 1, quit = true;
                        else if (opt2.x <= x && x <= opt2.x + opt2.w && opt2.y <= y && y <= opt2.y + opt2.h)
                              val = 2, quit = true;
                        else if (opt3.x <= x && x <= opt3.x + opt3.w && opt3.y <= y && y <= opt3.y + opt3.h)
                              val = 3, quit = true;
                  }
            }

            SDL_GetWindowSize(window, &w, &h);
            rect.x = -1912 + st, rect.y = 0;
            rect.w = 1912, rect.h = h;
            rect2.x = st, rect2.y = 0;
            rect2.w = 1912, rect2.h = h;
            st++;
            if (rect.x == 0) st = 0;
            txrect.x = w / 7, txrect.y = h / 20;
            txrect.w = 18 * w / 25, txrect.h = h / 7 + (h / 10 - h / 40);

            opt1.x = w / 3, opt1.y = h / 4 + h / 30;
            opt1.w = 10 * w / 30, opt1.h = h / 15;

            opt2.x = 3 * w / 10, opt2.y = 3 * h / 8 + h / 40;
            opt2.w = 2 * w / 5, opt2.h = h / 15;

            opt3.x = w / 4, opt3.y = h / 2 + h / 80;
            opt3.w = w / 2, opt3.h = h / 12;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            SDL_RenderCopy(renderer, nwtext, NULL, &rect);
            SDL_RenderCopy(renderer, nwtext, NULL, &rect2);
            SDL_RenderCopy(renderer, text, NULL, &txrect);

            SDL_RenderCopy(renderer, text1, NULL, &opt1);
            SDL_RenderCopy(renderer, text2, NULL, &opt2);
            SDL_RenderCopy(renderer, text3, NULL, &opt3);
            SDL_RenderPresent(renderer);

            colst = (colst + 1) % 360;
      }
      SDL_DestroyTexture(text);
      SDL_DestroyTexture(nwtext);
      SDL_FreeSurface(nwsurf);
      return val;
}

char *music_UI() {
      bool download = tinyfd_messageBox("Musico", "Do you want to download an audio file from youtube?", "yesno", "question", 1);
      char *file_stream = NULL;
      char const *lFilterPatterns[6] = {"*.wav", "*.txt", "*.text", "*.c", "*.h", "*.cpp"};
      if (download) {
            file_stream = tinyfd_inputBox("Youtube URL", "Please input a working youtube URL", NULL);
            char script[1000] = "sh wav-dl.sh ";
#ifdef OS2
            script[0] = 'w';
            script[1] = 'i';
            script[2] = 'n';
            script[3] = '.';
            script[4] = 'b';
            script[5] = 'a';
            script[6] = 't';
            script[7] = ' ';
            script[8] = '\0';
#endif
            strcat(script, file_stream);
            system(script);
      }
      file_stream = tinyfd_openFileDialog("Select file", "", 5, lFilterPatterns, NULL, 0);
      return file_stream;
}

void rec_UI(int curr, int st) {
      if (curr > 2) return;
      int h, w;
      SDL_GetWindowSize(window, &w, &h);
      SDL_RenderClear(renderer);

      pauserect.x = w / 100, pauserect.y = h / 100;
      pauserect.w = min(w, h) / 10, pauserect.h = min(w, h) / 10;

      stoprect.x = w / 100 + pauserect.w + w / 100, stoprect.y = h / 100;
      stoprect.w = min(w, h) / 10, stoprect.h = min(w, h) / 10;

      if (curr == 0) {
            disp.x = w / 10, disp.y = h / 3 + h / 14;
            disp.w = 4 * w / 5, disp.h = h / 7;
            SDL_RenderCopy(renderer, tx0, NULL, &disp);
      }
      if (curr == 1) {
            disp.x = w / 5, disp.y = h / 3 + h / 14;
            disp.w = w / 2, disp.h = h / 7;
            switch (st) {
                  case 0:
                        SDL_RenderCopy(renderer, tx10, NULL, &disp);
                        break;
                  case 1:
                        disp.w += (w / 18);
                        SDL_RenderCopy(renderer, tx11, NULL, &disp);
                        break;
                  case 2:
                        disp.w += (2 * w / 18);
                        SDL_RenderCopy(renderer, tx12, NULL, &disp);
                        break;
                  case 3:
                        disp.w += (3 * w / 18);
                        SDL_RenderCopy(renderer, tx13, NULL, &disp);
                        break;
                  default:
                        break;
            }
            SDL_RenderCopy(renderer, tpause, NULL, &pauserect);
      }
      if (curr == 2) {
            disp.x = 5 * w / 16, disp.y = h / 3 + h / 14;
            disp.w = 3 * w / 8, disp.h = h / 7;
            SDL_RenderCopy(renderer, tx2, NULL, &disp);
      }
      if (curr == -1) {
            disp.x = w / 10, disp.y = h / 3 + h / 14;
            disp.w = 4 * w / 5, disp.h = h / 7;
            SDL_RenderCopy(renderer, txp, NULL, &disp);
            SDL_RenderCopy(renderer, tplay, NULL, &pauserect);
      }

      if (abs(curr) == 1) SDL_RenderCopy(renderer, tstop, NULL, &stoprect);

      SDL_RenderPresent(renderer);
}
