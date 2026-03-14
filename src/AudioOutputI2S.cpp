/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include "AudioOutputI2S.h"

I2S AudioOutputI2S::i2s(INPUT_PULLUP); // input and output

//I2S AudioOutputI2S::i2s(OUTPUT); // output

void AudioOutputI2S::begin(uint pBCLK = 20, uint pWS = 21, uint pDOUT = 22) {

  // ********** I2S **********
  i2s.setBCLK(pBCLK);
  //i2s.setMCLK(pWS);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(32);
  i2s.setFrequency(AUDIO_SAMPLE_RATE);
  i2s.setSysClk(AUDIO_SAMPLE_RATE); // tweek CPU clock for sample rate
  i2s.setBuffers(6, AUDIO_BLOCK_SAMPLES * 2 * sizeof(int32_t) / sizeof(uint32_t));
  
// pinMode(15,OUTPUT);  
  i2s.onTransmit(I2S_Transmitted);

  // start I2S at the sample rate with 32-bits per sample
  if (!i2s.begin()) {
    Serial.println("Failed to initialize I2S!");
    while (1)
      ;  // do nothing
  }
  update_setup();
}

void AudioOutputI2S::begin(uint pBCLK = 20, uint pWS = 21, uint pMCLK = 24,uint pDIN = 23,uint pDOUT = 22) {

  // ********** I2S **********
  i2s.setDOUT(pDOUT);
  i2s.setDIN(pDIN);
  i2s.setBCLK(pBCLK);
  i2s.setMCLK(pMCLK);
  i2s.setMCLKmult(256);
  i2s.setBitsPerSample(32);
  i2s.setFrequency(AUDIO_SAMPLE_RATE);
  i2s.setSysClk(AUDIO_SAMPLE_RATE); // tweek CPU clock for sample rate
 // i2s.setBuffers(6, AUDIO_BLOCK_SAMPLES * 2 * sizeof(int32_t) / sizeof(uint32_t));
  i2s.setBuffers(12, AUDIO_BLOCK_SAMPLES * 2);
  
//  pinMode(pDIN,INPUT);  
  i2s.onTransmit(I2S_Transmitted);

  // start I2S at the sample rate with 32-bits per sample
  if (!i2s.begin()) {
    Serial.println("Failed to initialize I2S!");
    while (1)
      ;  // do nothing
  }
  update_setup();
}

void AudioOutputI2S::update() {
  audio_block_t *inputLeftBlock = receiveReadOnly(0);
  audio_block_t *inputRightBlock = receiveReadOnly(1);
	static int32_t tmp[AUDIO_BLOCK_SAMPLES*2]; // 4*256*2 = 2kB; saves a lot of CPU, though!

  if (inputLeftBlock && inputRightBlock) { // 
	for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    // When sending 0, some DAC will power off causing a hearable jump from silence to floor noise and cracks,
    // sending 1 is a workaround
    // Tested only with the PCM5100A
		int16_t sampleL = inputLeftBlock == NULL || inputLeftBlock->data[i] == 0 ? 1 : inputLeftBlock->data[i];// * 0.1;
		int16_t sampleR = inputRightBlock == NULL || inputRightBlock->data[i] == 0 ? 1 : inputRightBlock->data[i];// * 0.1;

	// fill the temporary buffer - MUCH faster than
	// writing samples two at a time
		tmp[i*2]   = (int32_t)sampleL*65536;
		tmp[i*2+1] = (int32_t)sampleR*65536;
	}
  
  // doesn't fail on overrun, but what would we do if 
  // a check showed we didn't write all data?!
	i2s.write((uint8_t*) tmp, sizeof tmp);
  }
  
  if (inputLeftBlock) {
    release(inputLeftBlock);
  }
  if (inputRightBlock) {
    release(inputRightBlock);
  }
}
