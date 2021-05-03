#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <fftw3.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SIZE = WINDOW_WIDTH / 2 + 1;
const int BARS = WINDOW_WIDTH / 10;
const int THICKNESS = WINDOW_WIDTH / BARS + 1;
const int RATE = 200; 
const int FIT_FACTOR = WINDOW_WIDTH / (SIZE - 1);

#define Max(a, b) a > b ? a : b
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
            SDL_LoadWAV(s, &wavSpec, &wavBuffer, &wavLength);
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
            free(in);
            free(out);
      }
};

struct rgb {
      int r, g, b;
      rgb(int _r, int _g, int _b) {
            r = _r, g = _g, b = _b;
      }
};

bool init(screen &window) {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
            printf("video and timer: %s\n",SDL_GetError());
            return false;
      }
      window.win = SDL_CreateWindow("Musico",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      if (!window.win) {
            printf("window: %s\n",SDL_GetError());
            SDL_Quit();
            return false;
      }
      window.renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
      window.rend = SDL_CreateRenderer(window.win, -1, window.renderFlags);
      if (!window.rend) {
            printf("renderer: %s\n",SDL_GetError());
            SDL_DestroyWindow(window.win);
            SDL_Quit();
            return false;
      }
      return true;
}

int main() {
      srand(time(0));
      printf("Please enter the song name which is in the cwd: ");
      char *songName; 
      songName = (char *) malloc(sizeof(char) * 100);
      scanf("%s", songName);

      screen window;
      complexData data(SIZE);

      if (!init(window)) {
            printf("Failed to initialize\n");
            return 0;
      }

      audioData song(songName);

      bool playing = true, mode = true, pause = true;

      song.play(pause);

      while (playing) {
            SDL_Event event;

            // temporary random data 
            for (int i = 0; i < SIZE; ++i) {
                  double scale = 1 - cos(2 * pi * i / SIZE);
                  data.in[i][0] = double(rand() % (WINDOW_HEIGHT / 15)) * scale;
                  if (rand() & 1) data.in[i][0] *= -1;
                  data.in[i][1] = 0;
            }
            // temporary random data 

            if (mode) {
                  double *actualFreq, *Max;

                  actualFreq = (double *) malloc(sizeof(double) * (BARS + 1));
                  Max = (double *) malloc(sizeof(double) * (BARS + 1));

                  for (int i = 0; i < BARS; ++i) {
                        Max[i] = 0;
                        actualFreq[i] = i * (RATE / SIZE + 1);
                  }
                  actualFreq[BARS] = RATE / 2;

                  fftw_execute(data.plan);

                  for (int j = 0; j < SIZE; j++) {
                        double real = data.out[j][0];
                        double imaginary = data.out[j][1];

                        double magnitude = sqrt((real * real) + (imaginary * imaginary));

                        double freq = j * ((double)RATE / SIZE);

                        for (int i = 0; i < BARS; i++) {
                              if ((freq > actualFreq[i]) && (freq <= actualFreq[i + 1])) {
                                    Max[i] = Max(magnitude, Max[i]);
                              }
                        }
                  }

                  SDL_SetRenderDrawColor(window.rend, 0, 0, 0, 0);
                  SDL_RenderClear(window.rend);

                  for (int i = 0; i < BARS; i++) {
                        rgb col((int((double)i * 1.75)) % 140, i % 80 + 111, 255);
                        SDL_SetRenderDrawColor(window.rend, col.r, col.g, col.b, 255);

                        Max[i] = fmod(5 * Max[i], WINDOW_HEIGHT / 2);

                        for (int j = 0; j < THICKNESS; j++) {
                              SDL_RenderDrawLine(window.rend, (i * THICKNESS + j), WINDOW_HEIGHT, (i * THICKNESS + j), WINDOW_HEIGHT - (Max[i]));
                        }
                  }
            } else {
                  SDL_SetRenderDrawColor(window.rend, 0, 0, 0, 0);
                  SDL_RenderClear(window.rend);
                  SDL_Point *wave;
                  wave = (SDL_Point *) malloc(sizeof(SDL_Point) * SIZE);
                  SDL_SetRenderDrawColor(window.rend, 255, 0, 0, 255);
                  int move = 0;
                  for (int start = 0; start <= 4; ++start) {
                        for (int i = move; i < SIZE - move; ++i) {
                              wave[i].x = i * FIT_FACTOR;
                              wave[i].y = (WINDOW_HEIGHT / 2 - data.in[i][0] * 2 + start);
                        }
                        SDL_RenderDrawLines(window.rend, wave, SIZE);
                        for (int i = move; i < SIZE - move; ++i) {
                              wave[i].x = i * FIT_FACTOR;
                              wave[i].y = WINDOW_HEIGHT / 2 - data.in[i][0] * 2 - start;
                        }
                        move += 5;
                        SDL_RenderDrawLines(window.rend, wave, SIZE);
                  }
            }
            SDL_RenderPresent(window.rend);
            SDL_Delay(75);

            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT or (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9)) {
                        playing = false;
                  }
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) mode ^= 1;
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                        pause ^= 1;
                        song.play(pause);
                  }
            }
      }

      window.quit();
      data.quit();
      song.quit();
      SDL_Quit();

      return 0;
}
