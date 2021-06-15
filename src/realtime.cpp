#include <visual.h>
#include <audio.h>

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
      bool quit = false, pause = false;
      while (!quit) {
            int height, width;
            SDL_GetWindowSize(window, &width, &height);
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true, thaam = true;
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int xx, yy;
                        SDL_GetMouseState(&xx, &yy);
                        if (circleIntersects(xx, yy, pauserect)) {
                              surf = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                              SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surf->pixels, surf->pitch);
                              pauseTex = SDL_CreateTextureFromSurface(renderer, surf);
                              pause ^= 1;
                              SDL_PauseAudioDevice(recordingDeviceId, pause);
                        }
                        if (circleIntersects(xx, yy, stoprect)) {
                              quit = 1;
                              break;
                        }
                        if (rectangleIntersects(xx, yy, moderect) && !pause) {
                              changeMode();
                        }
                  }
            }
            if (pause) {
                  pauserect.x = width / 100, pauserect.y = height / 100;
                  pauserect.w = min(width, height) / 10, pauserect.h = min(width, height) / 10;

                  stoprect.x = width / 100 + pauserect.w + width / 100, stoprect.y = height / 100;
                  stoprect.w = min(width, height) / 10, stoprect.h = min(width, height) / 10;

                  if(pauseTex == NULL) puts("pauseTex failed");
                  SDL_RenderClear(renderer);
                  SDL_RenderCopy(renderer, pauseTex, NULL, NULL);
                  SDL_RenderCopy(renderer, tplay, NULL, &pauserect);
                  SDL_RenderCopy(renderer, tstop, NULL, &stoprect);
                  SDL_RenderPresent(renderer);
            }
      }
      SDL_CloseAudioDevice(recordingDeviceId);
}