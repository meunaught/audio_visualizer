#include "utils.h"


bool intersects(int x, int y, SDL_Rect recc) {
      return recc.x <= x && x <= recc.x + recc.w && recc.y <= y && y <= recc.y + recc.h;
}

void setDefaultSpec(SDL_AudioSpec &spec) {
      SDL_zero(spec);
      spec.freq = RATE;
      spec.channels = 2;
      spec.samples = SAMPLES;
      spec.format = AUDIO_S16;
}

void wavCallBack(void *userData, Uint8 *stream, int len) {
      WavData *audio = (WavData *) userData;
      if (audio -> BufferByteSize == 0) return;
      
      Uint32 length = len;
      visualizerOutput(stream, audio->format);

      length = (length > audio -> BufferByteSize ? audio -> BufferByteSize : length);

      memcpy(stream, audio -> Buffer, length);

      audio -> Buffer += length;
      audio -> BufferByteSize -= length;
}

void musicMode(const char *file_stream) {
      if (file_stream == NULL) {
            SDL_Surface *surf;
            SDL_Texture *text[50];
            char *name[50];
            font = TTF_OpenFont("res/etaValo.ttf", 1000);
            SDL_Color col = {232, 232, 232};
            int cnt = 0;
            DIR *dir;
            dirent *ent;
            char huh[50];
            if ((dir = opendir("wav")) != NULL) {
                  while ((ent = readdir(dir)) != NULL) {
                        int len = strlen(ent -> d_name);
                        if (len > 3 && ent -> d_name[len - 1] == 'v' 
                              && ent -> d_name[len - 2] == 'a' 
                                    && ent -> d_name[len - 3] == 'w' 
                                          && ent -> d_name[len - 4] == '.') {
                              name[cnt] =  ent -> d_name;
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
            printf("cnt = %d\n", cnt);
            bool nquit = false;
            int h, w;
            SDL_Rect recc[cnt];
            while (!nquit) {
                  SDL_GetWindowSize(window, &w, &h);
                  int val = h / 8;
                  for (int i = 0; i < cnt; ++i) {
                        recc[i].x = w / 5, recc[i].y = (i + 1) * (val + val / cnt);
                        int len = strlen(name[i]) - 4;
                        recc[i].w = (w * len) / 20, recc[i].h = val;
                  }
                  SDL_RenderClear(renderer);
                  for (int i = 0; i < cnt; ++i) {
                        SDL_RenderCopy(renderer,text[i],NULL,&recc[i]);
                  }
                  SDL_RenderPresent(renderer);
                  SDL_Event eve;
                  while (SDL_PollEvent(&eve)) {
                        if (eve.type == SDL_QUIT) {
                              nquit = true;
                              return;
                        }
                        if (eve.type == SDL_MOUSEBUTTONDOWN) {
                              int x, y; 
                              SDL_GetMouseState(&x, &y);
                              int id = -1;
                              for (int i = 0; i < cnt; ++i) {
                                    if (intersects(x, y, recc[i])) {
                                          id = i;
                                          file_stream = name[i];
                                          nquit = true;
                                    }
                              }
                        }
                  }
            }
            chdir("wav");
      }

      SDL_AudioSpec wav_spec;
      SDL_AudioSpec obtained;
      Uint8 *wav_buffer;
      Uint32 wav_length;
      WavData *audio = (WavData *) malloc(sizeof(WavData));
      if (SDL_LoadWAV(file_stream, &wav_spec, &wav_buffer, &wav_length) == NULL) {
            fprintf(stderr, "FILE ERROR: %s\n", SDL_GetError());
            exit(1);
      }

      audio -> Buffer = wav_buffer;
      audio -> BufferByteSize = wav_length;
      setDefaultSpec(wav_spec);
      wav_spec.callback = wavCallBack;
      wav_spec.userdata = audio;
      audio -> format = wav_spec.format;
      SDL_AudioDeviceID playDeviceId = 0;
      playDeviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

	if (playDeviceId == 0) {
            printf("Failed to open recording device! SDL Error: %s", SDL_GetError());
            exit(1);
      }

      SDL_PauseAudioDevice(playDeviceId, SDL_FALSE);
      bool quit = false, pause = false, stop = false;
      while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true;
                  if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym){
                        case SDLK_p:
                              pause ^= 1;
                              SDL_PauseAudioDevice(playDeviceId, pause);
                              break;
                        case SDLK_q:
                              quit = 1;
                              break;
                        case SDLK_m:
                              changeMode();
                              break;
                        case SDLK_r:
                              if (stop) {
                                    stop = false;
                                    audio -> Buffer = wav_buffer;
                                    audio -> BufferByteSize = wav_length;
                                    setDefaultSpec(wav_spec);
                                    wav_spec.callback = wavCallBack;
                                    wav_spec.userdata = audio;
                                    audio -> format = wav_spec.format;
                                    playDeviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
                                    SDL_PauseAudioDevice(playDeviceId, SDL_FALSE);
                              }
                        default:
                             break;
                        }
                  }
            }
            if (((WavData *)wav_spec.userdata) -> BufferByteSize == 0) {
                  SDL_PauseAudioDevice(playDeviceId, true);
                  stop = true;
            }
      }
      SDL_FreeWAV(wav_buffer);
      SDL_CloseAudioDevice(playDeviceId);
}

