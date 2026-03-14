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


#include "AudioInputI2S.h"
#include <I2S.h>
#include "AudioOutputI2S.h"


void AudioInputI2S::begin(void)
{
	// nothing to do - I2S setup is done in the output module

}

union samplebuffer {
	uint8_t bytes[AUDIO_BLOCK_SAMPLES*2*4]; // a byte buffer for i2s.read(uint8_t *buf,bytes)
	int32_t sample[AUDIO_BLOCK_SAMPLES*2];  // 32 bit samples
} buf;

static int16_t samplesread=0;

void AudioInputI2S::update(void)
{
	audio_block_t *new_left=NULL, *new_right=NULL;
	
	uint32_t bytesread=AudioOutputI2S::i2s.read(buf.bytes,AUDIO_BLOCK_SAMPLES*8-samplesread*8); // read whats available
	samplesread+=bytesread/8;

	if (samplesread == AUDIO_BLOCK_SAMPLES) {
		samplesread=0;

		// allocate 2 new blocks, but if one fails, allocate neither
		new_left = allocate();
		if (new_left != NULL) {
			new_right = allocate();
			if (new_right == NULL) {
				release(new_left);
				new_left = NULL;
			}
		}

		if (new_left && new_right) {
			for (int16_t i=0; i < AUDIO_BLOCK_SAMPLES; i++) {  
				new_left->data[i] =buf.sample[i*2]>>16;  // scale 32 bit samples down to 16 bits
				new_right->data[i] =buf.sample[i*2+1]>>16;
			}
			transmit(new_left, 0);
			release(new_left);
			transmit(new_right, 1);
			release(new_right);
	//	Serial.print(".");
		} else {
		// we could not allocate
		// memory... the system is likely starving for memory!
		// Sadly, there's nothing we can do.
		}
	}
}


