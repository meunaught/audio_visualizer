#include "utils.h"

void createDefaultRecData(int MAX_RECORDING_SECONDS, SDL_AudioSpec ReceivedRecordingSpec) {
      int bytesPerSample = ReceivedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(ReceivedRecordingSpec.format) / 8);
      int bytesPerSecond = ReceivedRecordingSpec.freq * bytesPerSample;
      recData.BufferByteSize = (1 + MAX_RECORDING_SECONDS) * bytesPerSecond;
      recData.BufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;
      recData.Buffer = (Uint8 *)malloc(sizeof(Uint8) * recData.BufferByteSize);
      memset(recData.Buffer, 0, recData.BufferByteSize);
      recData.BufferBytePosition = 0;
}

void startRecording() {
      clearRenderer();
      SDL_AudioSpec desiredRecordingSpec;
      setDefaultSpec(desiredRecordingSpec);
      desiredRecordingSpec.callback = RecordingCallBack;
      recordingDeviceId = SDL_OpenAudioDevice(NULL, SDL_TRUE, &desiredRecordingSpec, &ReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
      if (recordingDeviceId == 0) {
            printf("Failed to open recording device! SDL Error: %s", SDL_GetError());
            exit(1);
      }
      SDL_AudioSpec desiredPlaybackSpec;
      SDL_zero(desiredPlaybackSpec);
      setDefaultSpec(desiredPlaybackSpec);
      desiredPlaybackSpec.callback = PlaybackCallBack;
      playbackDeviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desiredPlaybackSpec, &ReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
      if (playbackDeviceId == 0) {
            printf("Failed to open recording device! SDL Error: %s", SDL_GetError());
            exit(1);
      }
      createDefaultRecData(MAX_RECORDING_SECONDS, ReceivedRecordingSpec);
      SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
}



void recordMode() {
      int currentState = 0, st = 0, timer = 0;
      bool quit = false, pause = false;
      load_rec_UI();
      while (!quit) {

            int height, width;
            SDL_GetWindowSize(window, &width, &height);

            if (currentState == 1) {
                  if (pause) rec_UI(-1, st);
                  else rec_UI(currentState, st);
            } else rec_UI(currentState, st);
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true;
                  if (currentState == 0) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
                              startRecording();
                              currentState++;
                        }
                  }
                  if (currentState == 2) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n) {
                              currentState = 1;
                              createDefaultRecData(MAX_RECORDING_SECONDS, ReceivedRecordingSpec);
                              SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
                        }
                        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) currentState++;
                  }     
                  if (currentState == 3) {
                        recData.BufferBytePosition = 0;
                        SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);
                        currentState++;
                  } 
                  if (currentState == 5) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) currentState = 3;
                  }
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) changeMode();
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                        
                        int xx, yy;
                        SDL_GetMouseState(&xx, &yy);
                        if (cir_intersects(xx,yy,pauserect)) {
                              pause ^= 1;
                              if(currentState==1) SDL_PauseAudioDevice(recordingDeviceId, pause);
                              else if(currentState==4) SDL_PauseAudioDevice(playbackDeviceId, pause);
                              else if(currentState==5) {
                                    currentState=3;
                                    pause=false;
                              }
                        }
                        if(cir_intersects(xx,yy,stoprect)){
                              if(currentState==1) goto outter;
                              else if(currentState==4){
                                    quit=1;
                                    break;
                              }
                        }
                  }
            }
            if (currentState == 1) {
                  SDL_LockAudioDevice(recordingDeviceId);
                  if (recData.BufferBytePosition > recData.BufferByteMaxPosition) {
                  outter:
                        recData.BufferByteMaxPosition = recData.BufferBytePosition;
                        SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);
                        pause = false;
                        currentState++;
                  }
                  SDL_UnlockAudioDevice(recordingDeviceId);
            }
            if (currentState == 4) {
                  SDL_LockAudioDevice(playbackDeviceId);
                  if (recData.BufferBytePosition > recData.BufferByteMaxPosition) {
                        SDL_PauseAudioDevice(playbackDeviceId, SDL_TRUE);
                        currentState++;
                  }
                  SDL_UnlockAudioDevice(playbackDeviceId);
            }
            if (pause) {
                  pauserect.x = width / 100, pauserect.y = height / 100;
                  pauserect.w = min(width, height) / 10, pauserect.h = min(width, height) / 10;

                  stoprect.x = width / 100 + pauserect.w + width/100, stoprect.y = height / 100;
                  stoprect.w = min(width, height) / 10, stoprect.h = min(width, height) / 10;

                  SDL_RenderCopy(renderer, tplay, NULL, &pauserect);
                  SDL_RenderCopy(renderer, tstop, NULL, &stoprect);
                  SDL_RenderPresent(renderer);
            }

            if (currentState==5){
                  //puts("replay render");
                  //SDL_RenderClear(renderer);
                  pauserect.x = width / 100, pauserect.y = height / 100;
                  pauserect.w = min(width, height) / 10, pauserect.h = min(width, height) / 10;
                  SDL_SetRenderDrawColor(renderer,0,0,0,255);
                  SDL_RenderFillRect(renderer,&pauserect);
                  SDL_RenderDrawRect(renderer,&pauserect);
                  SDL_RenderCopy(renderer,treplay,NULL,&pauserect);
                  SDL_RenderPresent(renderer);
            }


            timer = (timer + 1) % 25;
            if (timer == 24) st = (st + 1) % 4;
      }

      SDL_CloseAudioDevice(recordingDeviceId);
      SDL_CloseAudioDevice(playbackDeviceId);
}


