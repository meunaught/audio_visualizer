#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <preprocessors.h>

void wavCallBack(void*, Uint8*, int);
void RealTimeCallBack(void*, Uint8*, int);
void RecordingCallBack(void*, Uint8*, int);
void PlaybackCallBack(void*, Uint8*, int);

double Get16bitAudioSample(Uint8*, SDL_AudioFormat);

#endif