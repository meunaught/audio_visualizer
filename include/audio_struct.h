#ifndef AUDIO_STRUCT_H
#define AUDIO_STRUCT_H

#include <preprocessors.h>

struct RecordedData {
      Uint8 *Buffer;
      Uint32 BufferByteSize;
      Uint32 BufferBytePosition;
      Uint32 BufferByteMaxPosition;
};

struct WavData {
      Uint8 *Buffer;
      Uint32 BufferByteSize;
      SDL_AudioFormat format;
};


#endif