#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <fftw3.h>
#include <stdio.h>
#include <getopt.h>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SIZE = WINDOW_WIDTH / 2 + 1;
const int BARS = WINDOW_WIDTH / 10;
const int THICKNESS = WINDOW_WIDTH / BARS + 1;
const int RATE = 200; 
const int FIT_FACTOR = WINDOW_WIDTH / (SIZE - 1);

#define max(a, b) a > b ? a : b
#define pi acos(-1)

struct screen {
      SDL_Window *win;
      SDL_Renderer *rend;
      Uint32 renderFlags;

      void quit() {
            SDL_DestroyRenderer(rend);
            SDL_DestroyWindow(win);
      }
};

struct audioData {
      SDL_AudioSpec wavSpec;
      Uint32 wavLength;
      Uint8 *wavBuffer;
      SDL_AudioDeviceID deviceId;

      audioData(char *s) {
            if(NULL == SDL_LoadWAV(s, &wavSpec, &wavBuffer, &wavLength)) {
			puts("NO SUCH FILE EXISTS");
			SDL_Quit();
			exit(0);
		}
            deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
            SDL_QueueAudio(deviceId, wavBuffer, wavLength);
      };

      void play(bool flag) {
            SDL_PauseAudioDevice(deviceId, !flag);
      }

      void quit() {
            SDL_CloseAudioDevice(deviceId);
            SDL_FreeWAV(wavBuffer);
      }
};

struct complexData {
      fftw_complex *in, *out;
      fftw_plan plan;

      complexData(int size) {
            in = (fftw_complex *) malloc(sizeof(fftw_complex) * size);
            out = (fftw_complex *) malloc(sizeof(fftw_complex) * size);
            plan = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
      };

      void quit() {
            fftw_destroy_plan(plan);
            fftw_cleanup();
            fftw_free(in);
            fftw_free(out);
      }
};

struct rgb {
      int r, g, b;
      rgb(int _r, int _g, int _b) {
            r = _r, g = _g, b = _b;
      }
};

bool init(screen &window);
void getAudioData(audioData song, complexData &data); // have to be implemented

#endif