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
      // const char *title = "Audio Visualizer";
      const char *op1 = "Play Music";
      const char *op2 = "Record Audio";
      const char *op3 = "Real Time Playback";
      // nwsurf = TTF_RenderText_Solid(font, title, col);
      nwsurf = IMG_Load("res/logo.png");
      SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = TTF_RenderText_Solid(nwfont, op1, col);
      SDL_Texture *text1 = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = TTF_RenderText_Solid(nwfont, op2, col);
      SDL_Texture *text2 = SDL_CreateTextureFromSurface(renderer, nwsurf);
      nwsurf = TTF_RenderText_Solid(nwfont, op3, col);
      SDL_Texture *text3 = SDL_CreateTextureFromSurface(renderer, nwsurf);

      SDL_Rect rect, txrect, rect2, opt1, opt2, opt3;
      int h, w;

      int st = 0, colst = 20;
      bool quit = false;
      int val = 0;
      while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true;

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
            txrect.w = 18 * w / 25, txrect.h = h / 7 + (h / 10 - h / 20);

            opt1.x = w / 3, opt1.y = h / 4 + h / 30;
            opt1.w = 10 * w / 30, opt1.h = h / 15;

            opt2.x = w / 3, opt2.y = 3 * h / 8 + h / 30;
            opt2.w = w / 3, opt2.h = h / 15;

            opt3.x = w / 4, opt3.y = h / 2 + h / 30;
            opt3.w = w / 2, opt3.h = h / 15;

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
      char *file_stream = NULL;
      SDL_Surface *surf;
      SDL_Texture *text[50];
      char name[50][100];
      font = TTF_OpenFont("res/firaL.ttf", 800);
      SDL_Color col = {232, 232, 232};
      int cnt = 0;
      DIR *dir;
      dirent *ent;
      char huh[50];
      chdir("wav");
      if ((dir = opendir(".")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                  int len = strlen(ent->d_name);
                  if (len > 3 && ent->d_name[len - 1] == 'v' && ent->d_name[len - 2] == 'a' && ent->d_name[len - 3] == 'w' && ent->d_name[len - 4] == '.') {
                        strcpy(name[cnt], ent->d_name);
                        strcpy(huh, name[cnt]);
                        huh[len - 4] = '\0';
                        char s[50] = "1) ";
                        s[0] += cnt;
                        strcat(s, huh);
                        surf = TTF_RenderText_Solid(font, s, col);
                        text[cnt] = SDL_CreateTextureFromSurface(renderer, surf);
                        cnt++;
                  }
            }
            closedir(dir);
      }
      bool nquit = false;
      int h, w;
      SDL_Rect recc[cnt];

      const char *head = "Select one of the following:";
      SDL_Surface *newsurf = TTF_RenderText_Solid(font, head, col);
      SDL_Texture *htext = SDL_CreateTextureFromSurface(renderer, newsurf);
      if (htext == NULL) puts("Htext failed");
      SDL_Rect hrect;

      while (!nquit) {
            SDL_GetWindowSize(window, &w, &h);
            int val = h / 8;
            for (int i = 0; i < cnt; ++i) {
                  recc[i].x = w / 100, recc[i].y = (i + 1) * (val + val / cnt);
                  int len = strlen(name[i]) - 4;
                  recc[i].w = (w * len) / 20, recc[i].h = val;
            }

            hrect.x = w / 100, hrect.y = h / 100;
            hrect.w = (w * 29) / 40, hrect.h = val;

            SDL_RenderClear(renderer);

            SDL_RenderCopy(renderer, htext, NULL, &hrect);

            for (int i = 0; i < cnt; ++i) {
                  SDL_RenderCopy(renderer, text[i], NULL, &recc[i]);
            }

            SDL_RenderPresent(renderer);
            SDL_Event eve;
            while (SDL_PollEvent(&eve)) {
                  if (eve.type == SDL_QUIT) {
                        nquit = true;
                  }
                  if (eve.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        int id = -1;
                        for (int i = 0; i < cnt; ++i) {
                              if (rect_intersects(x, y, recc[i])) {
                                    id = i;
                                    file_stream = name[i];
                                    nquit = true;
                              }
                        }
                  }
            }
      }
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
