#include "utils.h"

double Get16bitAudioSample(Uint8 *bytebuffer, SDL_AudioFormat format) {
      Uint16 val = 0x0;

      if (format & 1 << 12)
            val = ((Uint16)bytebuffer[0] << 8) | (Uint16)bytebuffer[1];
      else
            val = (Uint16)bytebuffer[0] | ((Uint16)bytebuffer[1] << 8);

      if (format & 1 << 15) return ((int16_t)val) / (double)(1 << 15);

      return val / (double)((1 << 16) - 1);
}

void visualizerOutput(Uint8 *stream, SDL_AudioFormat format) {
      double *MAX, *actFreq;
      int height, width;
      SDL_GetWindowSize(window, &width, &height);

      pauserect.x = width / 100, pauserect.y = height / 100;
      pauserect.w = min(width, height) / 10, pauserect.h = min(width, height) / 10;

      stoprect.x = width / 100 + pauserect.w + width / 100, stoprect.y = height / 100;
      stoprect.w = min(width, height) / 10, stoprect.h = min(width, height) / 10;

      moderect.w = 3 * width / 10, moderect.h = min(width, height) / 10;
      moderect.x = width - moderect.w - width / 100, moderect.y = height / 100;

      int BARS = width / THICKNESS;
      MAX = (double *)malloc(sizeof(double) * BARS);
      actFreq = (double *)malloc(sizeof(double) * (BARS + 1));

      double CONSTANT = (double)SAMPLES / width, freq;
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
            wave = (SDL_Point *)malloc(sizeof(SDL_Point) * SAMPLES);
            for (int i = 0; i < SAMPLES; i++) {
                  wave[i].x = i / CONSTANT;
                  wave[i].y = height / 2 - data.in[i][0] * VSCALE;
            }
            SDL_RenderDrawLines(renderer, wave, SAMPLES);
      } else {
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
                        SDL_RenderDrawLine(renderer, (i * DIST + j), height, (i * DIST + j), height - (SCALE * MAX[i]));
                  }
            }
      }
      start = (start + 1) % 360;
      
      SDL_RenderCopy(renderer, tpause, NULL, &pauserect);
      SDL_RenderCopy(renderer, tstop, NULL, &stoprect);
      if (!MODE) SDL_RenderCopy(renderer, wavetex, NULL, &moderect);
      else SDL_RenderCopy(renderer, barstex, NULL, &moderect);
      SDL_RenderPresent(renderer);
      free(MAX), free(actFreq);
}

void changeMode() {
      MODE ^= 1;
}
