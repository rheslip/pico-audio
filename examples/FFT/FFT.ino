// FFT Test
//
// Compute a 1024 point Fast Fourier Transform (spectrum analysis)
// on audio connected to the Left Line-In pin.  By changing code,
// a synthetic sine wave can be input instead.
//
// The first 40 (of 512) frequency analysis bins are printed to
// the Arduino Serial Monitor.  Viewing the raw data can help you
// understand how the FFT works and what results to expect when
// using the data to control LEDs, motors, or other fun things!
//
// This example code is in the public domain.

#include <Adafruit_TinyUSB.h>
#include <pico-audio.h>

#define AUDIO_STATS    // shows audio library CPU utilization etc on serial console

// I2S pins for DAC
#define BCLK 13
#define WS 14  // this will always be 1 pin above BCLK - can't change it
#define I2S_DATA 15

// const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
//AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT256    myFFT;
//AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         i2s1;        // 

// Connect either the live input or synthesized sine wave
//AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioConnection patchCord1(sinewave, 0, myFFT, 0);
// monitor the input signal
AudioConnection patchCord7(sinewave, 0, i2s1, 0);
AudioConnection patchCord8(sinewave, 0, i2s1, 1);

// stats timer
int32_t statstimer;
int32_t ffttimer;
#define STATSTIME 5000 // timer in ms
#define FFTTIME 1000 // FFT report time in ms

void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
 // audioShield.enable();
//  audioShield.inputSelect(myInput);
//  audioShield.volume(0.5);
  i2s1.begin(BCLK,WS,I2S_DATA);
  
  // Configure the window algorithm to use
//  myFFT.windowFunction(AudioWindowHanning1024);
  myFFT.windowFunction(AudioWindowHanning256);
  //myFFT.windowFunction(NULL);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(0.7);
  sinewave.frequency(500);
}

void loop() {
  float n;
  int i;

  if (myFFT.available() && ((millis()-ffttimer)>FFTTIME)) {
    // each time new FFT data is available
    // print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (i=0; i<40; i++) {
      n = myFFT.read(i);
      if (n >= 0.01) {
        Serial.print(n);
        Serial.print(" ");
      } else {
        Serial.print("  -  "); // don't print "0.00"
      }
    }
    Serial.println();
    ffttimer=millis();
  }

#ifdef AUDIO_STATS

    if ((millis() - statstimer) > STATSTIME)
    {
      Serial.print("Proc = ");
      Serial.print(AudioProcessorUsage());
      Serial.print(" (");    
      Serial.print(AudioProcessorUsageMax());
      Serial.print("),  Mem = ");
      Serial.print(AudioMemoryUsage());
      Serial.print(" (");    
      Serial.print(AudioMemoryUsageMax());
      Serial.println(")");
      statstimer=millis();
    }
#endif
}


