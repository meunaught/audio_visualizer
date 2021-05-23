#include "utils.h"

void realTimeMode() {
      SDL_AudioSpec desiredRecordingSpec;
      setDefaultSpec(desiredRecordingSpec);
      desiredRecordingSpec.callback = RealTimeCallBack;
      recordingDeviceId = SDL_OpenAudioDevice(NULL, SDL_TRUE, &desiredRecordingSpec, &ReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
      if (recordingDeviceId == 0) {
            printf("Failed to open recording device! SDL Error: %s", SDL_GetError());
            exit(1);
      }
      SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
      bool quit = false,pause = false;
      while (!quit) {
            int height, width;
            SDL_GetWindowSize(window, &width, &height);
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true;
                  if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                              case SDLK_p:
                                    pause ^= 1;
                                    SDL_PauseAudioDevice(recordingDeviceId, pause);
                                    break;
                              case SDLK_q:
                                    quit = 1;
                                    break;
                              case SDLK_m:
                                    changeMode();
                                    break;
                              default:
                                    break;
                        }
                  }
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                        
                        int xx, yy;
                        SDL_GetMouseState(&xx, &yy);
                        if (cir_intersects(xx,yy,pauserect)) {
                              pause ^= 1;
                              SDL_PauseAudioDevice(recordingDeviceId, pause);
                        }
                        if(cir_intersects(xx,yy,stoprect)){
                              quit=1;
                              break;
                        }
                  }
            }
            if (pause) {
                  pauserect.x = width / 100, pauserect.y = height / 100;
                  pauserect.w = min(width, height) / 10, pauserect.h = min(width, height) / 10;

                  stoprect.x = width / 100 + pauserect.w + width/100, stoprect.y = height / 100;
                  stoprect.w = min(width, height) / 10, stoprect.h = min(width, height) / 10;

                  SDL_RenderCopy(renderer, tplay, NULL, &pauserect);
                  SDL_RenderPresent(renderer);
            }
      }
      SDL_CloseAudioDevice(recordingDeviceId);
}