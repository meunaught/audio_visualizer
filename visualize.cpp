#include "utils.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

complexData data(NSAMPLES);

bool MODE = true;

void clearRenderer() {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
}

bool init() {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) return false;
      
      window = SDL_CreateWindow("Musico", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
      if (window == NULL) return false;
      
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (renderer == NULL) return false;
      
      return true;
}

double Get16bitAudioSample(Uint8 *bytebuffer, SDL_AudioFormat format) {
      Uint16 val = 0x0;

      if (SDL_AUDIO_ISLITTLEENDIAN(format)) val = (uint16_t)bytebuffer[0] | ((uint16_t)bytebuffer[1] << 8);
      else val = ((uint16_t)bytebuffer[0] << 8) | (uint16_t)bytebuffer[1];

      if (SDL_AUDIO_ISSIGNED(format)) return ((int16_t)val) / 32768.0;

      return val / 65535.0;
}

void visualizerOutput(Uint8 *stream, SDL_AudioFormat format) {
      double MAX[BARS], freq_bin[BARS + 1];
      double CONSTANT = (double) NSAMPLES / WIDTH, freq;
      int startx = 0, starty = HEIGHT;

      for (int i = 0; i < BARS; i++) {
            MAX[i] = 0;
            freq_bin[i] = i * (SAMPLE_RATE / NSAMPLES) + i;
      }
      freq_bin[BARS] = SAMPLE_RATE / 2;
      
      for (int i = 0; i < NSAMPLES; i++) {
            double multiplier = 0.5 * (1 - cos(2 * M_PI * i / NSAMPLES));

            data.in[i][0] = Get16bitAudioSample(stream, format) * multiplier;
            data.in[i][1] = 0.0;
            stream += 2;
      }
	
      if (MODE) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Point *wave;
            wave = (SDL_Point*) malloc(sizeof(SDL_Point) * NSAMPLES);
            for (int i = 0; i < NSAMPLES; i++) {
                  wave[i].x = i / CONSTANT;
                  wave[i].y = 300 - data.in[i][0] * 70;
            }
            SDL_RenderDrawLines(renderer, wave, NSAMPLES);
      }
      else {
            fftw_execute(data.plan);

            for (int j = 0; j < NSAMPLES / 2; j++) {
                  double real = data.out[j][0];
                  double imaginary = data.out[j][1];

                  double magnitude = sqrt((real * real) + (imaginary * imaginary));

                  freq = j * ((double)SAMPLE_RATE / NSAMPLES);

                  for (int i = 0; i < BARS; i++)
                        if ((freq > freq_bin[i]) && (freq <= freq_bin[i + 1]))
                              MAX[i] = max(MAX[i], magnitude);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            for (int i = 0; i < BARS; i++) {
                  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

                  if (MAX[i] > 2.0) MAX[i] = log(MAX[i]);

                  for (int j = 0; j < THICKNESS; j++)
                        SDL_RenderDrawLine(renderer, (i * DISTANCE + j), HEIGHT, (i * DISTANCE + j), HEIGHT - (FIT_FACTOR * MAX[i]));
            }
      }
      SDL_RenderPresent(renderer);
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