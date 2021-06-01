#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int RATE = 44100;
const int THICKNESS = 15;
const int DIST = 16;
const int SCALE = 60;
const int VSCALE = 100;
const int SAMPLES = 2048;
const int MAX_RECORDING_SECONDS = 600;
const double pi = acos(-1);

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_AudioDeviceID recordingDeviceId;
extern SDL_AudioDeviceID playbackDeviceId;
extern SDL_AudioSpec ReceivedRecordingSpec;
extern SDL_AudioSpec ReceivedPlaybackSpec;
extern TTF_Font *font, *nwfont;
extern SDL_Rect disp;
extern SDL_Surface *surf;
extern SDL_Texture *tx0, *tx10, *tx11, *tx12, *tx13, *tx2, *txp;

extern SDL_Texture *tpause, *tplay, *tstop, *treplay, *tstart;
extern SDL_Texture *wavetex, *barstex;
extern SDL_Texture *pauseTex;
extern SDL_Rect pauserect, stoprect, moderect;

extern bool MODE;
extern bool thaam;

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

struct complexData {
      fftw_plan plan;
      fftw_complex *in;
      fftw_complex *out;

      complexData(int SIZE) {
            in = (fftw_complex* ) malloc(sizeof(fftw_complex) * SIZE);
            out = (fftw_complex* ) malloc(sizeof(fftw_complex) * SIZE); 
            plan = fftw_plan_dft_1d(SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
      }
} extern data;

struct RecordedData {
      Uint8 *Buffer;
      Uint32 BufferByteSize;
      Uint32 BufferBytePosition;
      Uint32 BufferByteMaxPosition;
} extern recData;

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

void load();
void loadRecUI();
void recUI(int,int);
char* musicUI();
int UI();
void clearRenderer();
bool init();
bool rectangleIntersects(int, int, SDL_Rect);
bool circleIntersects(int,int,SDL_Rect);
void createDefaultRecData(int, SDL_AudioSpec);
double Get16bitAudioSample(Uint8*, SDL_AudioFormat);
void visualizerOutput(Uint8*, SDL_AudioFormat);
void wavCallBack(void*, Uint8*, int);
void RealTimeCallBack(void*, Uint8*, int);
void RecordingCallBack(void*, Uint8*, int);
void PlaybackCallBack(void*, Uint8*, int);
void changeMode();
void setDefaultSpec(SDL_AudioSpec&);
void musicMode(char *);
void realTimeMode();
void startRecording();
void playRecordedData();
void recordMode();
void quit();

#endif
