#ifndef VISUAL_H
#define VISUAL_H

#include <utils.h>
#include <visual_data.h>
#include <visual_struct.h>

extern complexData data;

extern TTF_Font *font, *nwfont;
extern SDL_Rect disp;
extern SDL_Surface *surf;
extern SDL_Texture *tx0, *tx10, *tx11, *tx12, *tx13, *tx2, *txp;

extern SDL_Texture *tpause, *tplay, *tstop, *treplay, *tstart;
extern SDL_Texture *wavetex, *barstex;
extern SDL_Texture *pauseTex;
extern SDL_Rect pauserect, stoprect, moderect;

void load();
void loadRecUI();
int UI();
void recUI(int,int);
char* musicUI();

bool rectangleIntersects(int, int, SDL_Rect);
bool circleIntersects(int,int,SDL_Rect);
void clearRenderer();


#endif