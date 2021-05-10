#include "utils.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

complexData data(SAMPLES);

bool MODE = true;

void clearRenderer() {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
}

bool init() {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) return false;
      
      window = SDL_CreateWindow("Musico", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
      if (window == NULL) return false;
      
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (renderer == NULL) return false;
      
      return true;
}

double Get16bitAudioSample(Uint8 *bytebuffer, SDL_AudioFormat format) {
      Uint16 val = 0x0;
      int32_t mask = 1 << 15;

      if (format & 1 << 12) val = ((uint16_t)bytebuffer[0] << 8) | (uint16_t)bytebuffer[1];
      else val = (uint16_t)bytebuffer[0] | ((uint16_t)bytebuffer[1] << 8);

      if (format & 1 << 15) return ((int16_t)val) / (double) (mask);

      return val / (double) ((mask << 1) - 1);
}

void visualizerOutput(Uint8 *stream, SDL_AudioFormat format) {
      double *MAX, *actFreq;
      MAX = (double *) malloc(sizeof(double) * BARS);
      actFreq = (double *) malloc(sizeof(double) * (BARS + 1));

      double CONSTANT = (double) SAMPLES / WINDOW_WIDTH, freq;
      static int start = 150;

      for (int i = 0; i < BARS; i++) {
            MAX[i] = 0;
            actFreq[i] = i * (RATE / SAMPLES) + i;
      }
      actFreq[BARS] = RATE / 2;
      
      for (int i = 0; i < SAMPLES; i++) {
            double multiplier = sin(pi * i / SAMPLES);
            multiplier *= multiplier;

            data.in[i][0] = Get16bitAudioSample(stream, format) * multiplier;
            data.in[i][1] = 0.0;
            stream += 2;
      }
	
      if (MODE) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            rgb col(start);
            SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
            SDL_Point *wave;
            wave = (SDL_Point*) malloc(sizeof(SDL_Point) * SAMPLES);
            for (int i = 0; i < SAMPLES; i++) {
                  wave[i].x = i / CONSTANT;
                  wave[i].y = WINDOW_HEIGHT / 2 - data.in[i][0] * VSCALE;
            }
            SDL_RenderDrawLines(renderer, wave, SAMPLES);
      }
      else {
            fftw_execute(data.plan);

            for (int j = 0; j < SAMPLES / 2; j++) {
                  double real = data.out[j][0];
                  double imaginary = data.out[j][1];

                  double magnitude = sqrt((real * real) + (imaginary * imaginary));

                  freq = j * ((double)RATE / SAMPLES);

                  for (int i = 0; i < BARS; i++) {
                        if ((freq > actFreq[i]) && (freq <= actFreq[i + 1]))
                              MAX[i] = max(MAX[i], magnitude);
                  }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            for (int i = 0; i < BARS; i++) {
                  rgb col((start + i * 2) % 360);
                  SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);

                  if (MAX[i] > 2.0) MAX[i] = log(MAX[i]);

                  for (int j = 0; j < THICKNESS; j++) {
                        SDL_RenderDrawLine(renderer, (i * DIST + j), WINDOW_HEIGHT, (i * DIST + j), WINDOW_HEIGHT - (SCALE * MAX[i]));
                  }
            }
      }
      start = (start + 1) % 360;
      SDL_RenderPresent(renderer);
      free(MAX), free(actFreq);
}

void changeMode() {
      MODE ^= 1;
}

void quit() {
      fftw_destroy_plan(data.plan);
      fftw_cleanup();
      fftw_free(data.in);
      fftw_free(data.out);
      
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      window = NULL;
      renderer = NULL;
      
      SDL_Quit();
}