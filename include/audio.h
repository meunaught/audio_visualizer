#ifndef AUDIO_H
#define AUDIO_H

#include <utils.h>
#include <audio_struct.h>
#include <audio_data.h>

extern SDL_AudioDeviceID recordingDeviceId;
extern SDL_AudioDeviceID playbackDeviceId;
extern SDL_AudioSpec ReceivedRecordingSpec;
extern SDL_AudioSpec ReceivedPlaybackSpec;

extern RecordedData recData;

void createDefaultRecData(int, SDL_AudioSpec);
void setDefaultSpec(SDL_AudioSpec&);
void startRecording();
void playRecordedData();

#endif