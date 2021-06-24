#ifndef UTILS_H
#define UTILS_H


#include <preprocessors.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern bool MODE;
extern bool finish;

bool init();
void changeMode();
void quit();
void musicMode(char *);
void realTimeMode();
void recordMode();

#endif
