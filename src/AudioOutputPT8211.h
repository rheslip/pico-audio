
#ifndef AudioOutputPT8211_h
#define AudioOutputPT8211_h

#include <I2S.h>

#include "AudioStream.h"

class AudioOutputPT8211 : public AudioStream {
protected:
  audio_block_t *inputQueueArray[2];

public:
  static I2S i2s;
  AudioOutputPT8211();
  void begin(uint pBCLK, uint pWS, uint pDOUT);
  void update();
};


inline AudioOutputPT8211::AudioOutputPT8211()
  : AudioStream(2, inputQueueArray) {
}

extern void I2S_Transmitted(void);

#endif