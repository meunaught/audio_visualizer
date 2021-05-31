#include <utils.h>

bool init() {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

      window = SDL_CreateWindow("Musico", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
      if (window == NULL) return false;

      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (renderer == NULL) return false;

      if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

      if (TTF_Init() == -1) return false;

      return true;
}

void clearRenderer() {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
}

void RecordingCallBack(void *userdata, Uint8 *stream, int len) {
      memcpy(recData.Buffer + recData.BufferBytePosition, stream, len);
      recData.BufferBytePosition += len;
}

void PlaybackCallBack(void *userdata, Uint8 *stream, int len) {
      visualizerOutput(stream, ReceivedPlaybackSpec.format);
      memcpy(stream, recData.Buffer + recData.BufferBytePosition, len);
      recData.BufferBytePosition += len;
}

void RealTimeCallBack(void *userdata, Uint8 *stream, int len) {
      visualizerOutput(stream, ReceivedRecordingSpec.format);
}

bool rect_intersects(int x, int y, SDL_Rect recc) {
      return recc.x <= x && x <= recc.x + recc.w && recc.y <= y && y <= recc.y + recc.h;
}

bool cir_intersects(int xx, int yy, SDL_Rect recc) {
      int rad = recc.w / 2;
      int x = recc.x + rad, y = recc.y + rad;
      return (x - xx) * (x - xx) + (y - yy) * (y - yy) <= rad * rad;
}


void load_rec_UI() {
      const char *r0 = "Click to start recording";
      const char *r10 = "Recording";
      const char *r11 = "Recording.";
      const char *r12 = "Recording..";
      const char *r13 = "Recording...";
      const char *r2 = "Recorded";
      const char *rp = "Recording Paused";
      SDL_Color col = {232, 232, 232};
      font = TTF_OpenFont("res/firaL.ttf", 300);
      surf = TTF_RenderText_Solid(font, r0, col);
      tx0 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, r10, col);
      tx10 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, r11, col);
      tx11 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, r12, col);
      tx12 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, r13, col);
      tx13 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, r2, col);
      tx2 = SDL_CreateTextureFromSurface(renderer, surf);
      surf = TTF_RenderText_Solid(font, rp, col);
      txp = SDL_CreateTextureFromSurface(renderer, surf);
}

void load() {
      surf = IMG_Load("res/pause.png");
      tpause = SDL_CreateTextureFromSurface(renderer, surf);
      surf = IMG_Load("res/play.png");
      tplay = SDL_CreateTextureFromSurface(renderer, surf);
      surf = IMG_Load("res/stop.png");
      tstop = SDL_CreateTextureFromSurface(renderer, surf);
      surf = IMG_Load("res/replay.png");
      treplay = SDL_CreateTextureFromSurface(renderer, surf);
      if (treplay == NULL) puts("Treplay Failed");
      surf = IMG_Load("res/start.png");
      tstart = SDL_CreateTextureFromSurface(renderer, surf);

      surf = IMG_Load("res/wave.png");
      if (surf == NULL) puts("surf failed");
      wavetex = SDL_CreateTextureFromSurface(renderer, surf);
      if (wavetex == NULL) puts("wavetex failed");
      surf = IMG_Load("res/bars.png");
      if (surf == NULL) puts("surf failed");
      barstex = SDL_CreateTextureFromSurface(renderer, surf);
      if (barstex == NULL) puts("barstex failed");
      load_rec_UI();
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
