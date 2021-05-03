#include "visualize.h"

void visualizeWave(screen &window, const complexData &data) {
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

void visualizeBars(screen &window, const complexData &data) {
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
				Max[i] = max(magnitude, Max[i]);
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
}