#ifndef AUDIO_H
#define AUDIO_H

#include <utils.h>

extern SDL_AudioDeviceID recordingDeviceId;
extern SDL_AudioDeviceID playbackDeviceId;
extern SDL_AudioSpec ReceivedRecordingSpec;
extern SDL_AudioSpec ReceivedPlaybackSpec;

 extern RecordedData recData;

void createDefaultRecData(int, SDL_AudioSpec);
void setDefaultSpec(SDL_AudioSpec&);
void wavCallBack(void*, Uint8*, int);
void RealTimeCallBack(void*, Uint8*, int);
void RecordingCallBack(void*, Uint8*, int);
void PlaybackCallBack(void*, Uint8*, int);
void startRecording();
void playRecordedData();

#endif