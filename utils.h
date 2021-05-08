#ifndef UTILS_H
#define UTILS_H

#include <fftw3.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <assert.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const int SAMPLE_RATE = 44100;
const int BARS = 60;
const int THICKNESS = 15;
const int DISTANCE = 16;
const int FIT_FACTOR = 30;
const int NSAMPLES = 2048;
const int MAX_RECORDING_SECONDS = 600;

#define max(a, b) a > b ? a : b

struct complexData {
	fftw_plan plan;
      fftw_complex *in;
      fftw_complex *out;

      complexData(int SIZE) {
            in = (fftw_complex* ) malloc(sizeof(fftw_complex) * SIZE);
            out = (fftw_complex* ) malloc(sizeof(fftw_complex) * SIZE); 
            plan = fftw_plan_dft_1d(SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
      }
};

struct RecordedData {
      Uint8 *Buffer;
      Uint32 BufferByteSize;
      Uint32 BufferBytePosition;
      Uint32 BufferByteMaxPosition;
};

struct WavData {
      Uint8 *Buffer;
      Uint32 BufferByteSize;
      SDL_AudioFormat format;
};

void clearRenderer();
bool init();
void createDefaultRecData(int MAX_RECORDING_SECONDS, SDL_AudioSpec ReceivedRecordingSpec);
double Get16bitAudioSample(Uint8 *bytebuffer, SDL_AudioFormat format);
void visualizerOutput(Uint8 *stream, SDL_AudioFormat format);
void wavCallBack(void *userData, Uint8 *stream, int len);
void RealTimeCallBack(void *userdata, Uint8 *stream, int len);
void RecordingCallBack(void *userdata, Uint8 *stream, int len);
void PlaybackCallBack(void *userdata, Uint8 *stream, int len);
void changeMode();
void setDefaultSpec(SDL_AudioSpec &spec);
void musicMode(const char *file_stream);
void realTimeMode();
void startRecording();
void playRecordedData();
void recordMode();
void quit();

#endif