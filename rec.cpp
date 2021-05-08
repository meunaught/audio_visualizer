#include "utils.h"

SDL_AudioDeviceID recordingDeviceId=0;
SDL_AudioDeviceID playbackDeviceId=0;
SDL_AudioSpec ReceivedRecordingSpec;
SDL_AudioSpec ReceivedPlaybackSpec;

RecordedData recData;

void createDefaultRecData(int MAX_RECORDING_SECONDS, SDL_AudioSpec ReceivedRecordingSpec) {
	int bytesPerSample = ReceivedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(ReceivedRecordingSpec.format ) / 8);
	int bytesPerSecond = ReceivedRecordingSpec.freq * bytesPerSample;
	recData.BufferByteSize = (1 + MAX_RECORDING_SECONDS) * bytesPerSecond;
	recData.BufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;
    	recData.Buffer = (Uint8*)malloc(sizeof(Uint8)*recData.BufferByteSize);
	memset(recData.Buffer, 0, recData.BufferByteSize);
}

void RecordingCallBack(void *userdata, Uint8 *stream, int len) {
	memcpy(recData.Buffer + recData.BufferBytePosition, stream, len);
	recData.BufferBytePosition += len;
}

void PlaybackCallBack(void *userdata, Uint8 *stream, int len) {
	visualizerOutput(stream, ReceivedPlaybackSpec.format);
	memcpy(stream, recData.Buffer + recData.BufferBytePosition, len);
	recData.BufferBytePosition += len;
}

void RealTimeCallBack(void *userdata, Uint8 *stream, int len) {
	visualizerOutput(stream, ReceivedRecordingSpec.format);
}

void startRecording() {
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
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                 if (event.type == SDL_QUIT) quit = true;
                 if (event.type==SDL_KEYDOWN){
                        switch (event.key.keysym.sym){
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
            }
      }
	SDL_CloseAudioDevice(recordingDeviceId);
}

void recordMode() {
      int quit = 0,currentState = 0,pause = 0;
      while (!quit) {
            SDL_Event event;
		while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) quit = true;
                  if (currentState == 0) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {	
					startRecording();
                              currentState++;
                        }
                  }
                  else if (currentState == 1 && (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)) goto outter;
                  else if (currentState == 1 && (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)) {
                        pause ^= 1;
                        SDL_PauseAudioDevice(recordingDeviceId,pause);
                  }
                  else if (currentState == 2) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n) {
                              currentState = 1;
                              createDefaultRecData(MAX_RECORDING_SECONDS, ReceivedRecordingSpec);
    	                        SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
                        }
                        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) currentState++;
                  }
                  else if (currentState == 3) {
				recData.BufferBytePosition = 0;
                        SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);
                        currentState++;
                  }
                  else if (currentState == 5) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n) {
				      currentState = 1;
                    		clearRenderer();
                              createDefaultRecData(MAX_RECORDING_SECONDS, ReceivedRecordingSpec);
    	                        SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
                        }
                        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) currentState = 3;
                        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) quit = true;
                  }
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) changeMode();
            }
            if (currentState == 1) {
                  SDL_LockAudioDevice(recordingDeviceId);
                  if (recData.BufferBytePosition > recData.BufferByteMaxPosition) {
                        outter:
				recData.BufferByteMaxPosition = recData.BufferBytePosition;
                        puts("RECORDED");
                        SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);
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
      }
	
	SDL_CloseAudioDevice(recordingDeviceId);
	SDL_CloseAudioDevice(playbackDeviceId);
}