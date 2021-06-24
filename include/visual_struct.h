#ifndef VISUAL_STRUCT_H
#define VISUAL_STRUCT_H

#include <preprocessors.h>

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



#endif
