#ifndef PREPROCESSORS_H
#define PREPROCESSORS_H

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

enum state {
      startscreen,
      recording,
      recorded,
      startplaying,
      playing,
      doneplaying
};

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#endif