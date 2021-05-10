#ifndef UTILS_H
#define UTILS_H

#include <fftw3.h>
#include "SDL2/SDL.h"
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <assert.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int RATE = 44100;
const int BARS = 60;
const int THICKNESS = 15;
const int DIST = 16;
const int SCALE = 30;
const int VSCALE = 100;
const int SAMPLES = 2048;
const int MAX_RECORDING_SECONDS = 600;
const double pi = acos(-1);

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

struct rgb {
      double r, g, b;
      rgb(double h) {
            double val = (1 - abs(fmod(h / 60.0, 2) - 1));
            if (h >= 0 && h < 60) {
                  r = 1, g = val, b = 0;
            } else if (h >= 60 && h < 120) {
                  r = val, g = 1, b = 0;
            } else if (h >= 120 && h < 180) {
                  r = 0, g = 1, b = val;
            } else if (h >= 180 && h < 240) {
                  r = 0, g = val, b = 1;
            } else if (h >= 240 && h < 300) {
                  r = val, g = 0, b = 1;
            } else{
                  r = 1, g = 0, b = val;
            }
            r *= 255.0;
            g *= 255.0;
            b *= 255.0;
      }
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