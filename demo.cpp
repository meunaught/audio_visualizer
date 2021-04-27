#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <fftw3.h>

const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 1280;
const int SIZE = 100;
const int BARS = 70;
const int THICKNESS = 21;
const int RATE = 200; 

#define max(a, b) a > b ? a : b
#define pi acos(-1)

SDL_Window *win;
SDL_Renderer *rend;
Uint32 renderFlags;

SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;
fftw_complex *in, *out;

struct rgb {
      int r, g, b;
      rgb(int _r, int _g, int _b) {
            r = _r, g = _g, b = _b;
      }
};

bool init() {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
            printf("video and timer: %s\n",SDL_GetError());
            return false;
      }
      win = SDL_CreateWindow("Music_Visualizer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      if (!win) {
            printf("window: %s\n",SDL_GetError());
            SDL_Quit();
            return false;
      }
      renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
      rend = SDL_CreateRenderer(win, -1, renderFlags);
      if (!rend) {
            printf("renderer: %s\n",SDL_GetError());
            SDL_DestroyWindow(win);
            SDL_Quit();
            return false;
      }
      return true;
}

void bajao(char *s) {
      SDL_LoadWAV(s, &wavSpec, &wavBuffer, &wavLength);
      deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
      
      SDL_QueueAudio(deviceId, wavBuffer, wavLength);
      SDL_PauseAudioDevice(deviceId, 0);
}

void quit() {
      SDL_CloseAudioDevice(deviceId);
      SDL_FreeWAV(wavBuffer);
      SDL_DestroyRenderer(rend);
      SDL_DestroyWindow(win);
      free(in);
      free(out);
      SDL_Quit();
}

int main() {
      srand(time(0));
      printf("Please enter the song name which is in the cwd: ");
      char *song; 
      song = (char *) malloc(sizeof(char) * 100);
      scanf("%s", song);
      int n = 100;
      in = (fftw_complex *) malloc(sizeof(fftw_complex) * n);
      out = (fftw_complex *) malloc(sizeof(fftw_complex) * n);

      fftw_plan plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

      if (!init()) {
            printf("Failed to initialize\n");
            return 0;
      }

      bajao("Last_Surprise.wav");
      bool playing = true, mode = true, pause = false;

      while (playing) {
            SDL_Event event;

            for (int i = 0; i < n; ++i) {
                  double scale = 0.5 * (1 - cos(2 * pi * i / SIZE));
                  // in[i][0] = fmod(rand(), WINDOW_HEIGHT / 10);
                  in[i][0] = double(rand() % (WINDOW_HEIGHT / 15)) * scale;
                  if (rand() & 1) in[i][0] *= -1;
                  in[i][1] = 0;
                  // printf("%lf ", in[i][0]);
            }
            // printf("\n");

            if (mode) {
                  double freqq[BARS + 1], max[BARS + 1];

                  for (int i = 0; i < BARS; ++i) {
                        max[i] = 0;
                        freqq[i] = i * (RATE / SIZE + 1);
                  }
                  freqq[BARS] = RATE / 2;

                  fftw_execute(plan);
                  // for (int i = 0; i < n; ++i) {
                  //       printf("%lf ", sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]));
                  // }
                  for (int j = 0; j < SIZE; j++) {
                        double re = out[j][0];
                        double im = out[j][1];

                        double magnitude = sqrt((re * re) + (im * im));

                        double freq = j * ((double)RATE / SIZE);

                        for (int i = 0; i < BARS; i++) {
                              if ((freq > freqq[i]) && (freq <= freqq[i + 1])) {
                                    max[i] = max(magnitude, max[i]);
                              }
                        }
                  }

                  SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
                  SDL_RenderClear(rend);

                  for (int i = 0; i < BARS; i++) {
                        rgb col((int((double)i * 1.75)) % 140, i % 80 + 111, 255);
                        SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, 255);

                        max[i] = fmod(5 * max[i], WINDOW_HEIGHT);

                        for (int j = 0; j < THICKNESS; j++) {
                              SDL_RenderDrawLine(rend, (i * THICKNESS + j), WINDOW_HEIGHT, (i * THICKNESS + j), WINDOW_HEIGHT - (max[i]));
                        }
                  }
            } else {
                  SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
                  SDL_RenderClear(rend);
                  SDL_Point *wave;
                  wave = (SDL_Point *) malloc(sizeof(SDL_Point) * n);
                  SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
                  int x = 0;
                  for (int st = 0; st <= 4; ++st) {
                        for (int i = x; i < n - x; ++i) {
                              // double scale = 0.5 * (1 - cos(2 * M_PI * i / SIZE));
                              wave[i].x = i * 15;
                              wave[i].y = (WINDOW_HEIGHT / 2 - in[i][0] * 2 + st);
                              // wave[i].y = (double) wave[i].y * (sqrt(1 - scale * scale));
                        }
                        // x += 50;
                        SDL_RenderDrawLines(rend, wave, SIZE);
                        for (int i = x; i < n - x; ++i) {
                              // double scale = 0.5 * (1 - cos(2 * M_PI * i / SIZE));
                              wave[i].x = i * 15;
                              wave[i].y = WINDOW_HEIGHT / 2 - in[i][0] * 2 - st;
                              // wave[i].y = (double) wave[i].y * (sqrt(1 - scale * scale));
                        }
                        x += 10;
                        SDL_RenderDrawLines(rend, wave, SIZE);
                  }
            }
            SDL_RenderPresent(rend);
            SDL_Delay(75);

            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT or event.key.keysym.sym == SDLK_9) {
                        playing = false;
                  }
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) mode ^= 1;
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                        pause ^= 1;
                        SDL_PauseAudioDevice(deviceId, pause);
                  }
            }
      }

      quit();

	return 0;
}
