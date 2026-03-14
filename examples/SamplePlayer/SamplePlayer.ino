
#include <Adafruit_TinyUSB.h>
#include <pico-audio.h>
#include <Bounce2.h> // version 2.55

// I2S pins for DAC
#define BCLK 13
#define WS 14  // this will always be 1 pin above BCLK - can't change it
#define I2S_DATA 15

// WAV files converted to code by wav2sketch
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
//AudioOutputI2S     headphones;
AudioOutputI2S i2s1;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
AudioConnection c6(sound4, 0, mix2, 1);
AudioConnection c7(sound5, 0, mix2, 2);
//AudioConnection c8(mix2, 0, headphones, 0);
//AudioConnection c9(mix2, 0, headphones, 1);
AudioConnection c10(mix2, 0, i2s1, 0);
AudioConnection c11(mix2, 0, i2s1, 1);

// Create an object to control the audio shield.
// 
// AudioControlSGTL5000 audioShield;

// Bounce objects to read six pushbuttons (pins 0-5)
//
Bounce2::Button button0 = Bounce2::Button();
Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();
Bounce2::Button button5 = Bounce2::Button();


void setup() {
  Serial.begin(115200);

  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  button0.attach(0, INPUT_PULLUP );
  button1.attach(1, INPUT_PULLUP );
  button2.attach(2, INPUT_PULLUP );
  button3.attach(3, INPUT_PULLUP );
  button4.attach(4, INPUT_PULLUP );
  button5.attach(5, INPUT_PULLUP );

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button0.interval(5);
  button1.interval(5); 
  button2.interval(5); 
  button3.interval(5); 
  button4.interval(5); 
  button5.interval(5);  

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button0.setPressedState(LOW);
  button1.setPressedState(LOW);
  button2.setPressedState(LOW);
  button3.setPressedState(LOW);
  button4.setPressedState(LOW);
  button5.setPressedState(LOW); 

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // turn on the output
  // audioShield.enable();
  // audioShield.volume(0.5);

  i2s1.begin(BCLK,WS,I2S_DATA);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);

//  headphones.begin();
}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();

  // When the buttons are pressed, just start a sound playing.
  // The audio library will play each sound through the mixers
  // so any combination can play simultaneously.
  //
  if (button0.pressed()) {
    sound0.play(AudioSampleSnare);
    Serial.println("AudioSampleSnare");
  }
  if (button1.pressed()) {
    sound1.play(AudioSampleTomtom);
    Serial.println("AudioSampleTomtom");
  }
  if (button2.pressed()) {
    sound2.play(AudioSampleHihat);
    Serial.println("AudioSampleHihat");
  }
  if (button3.pressed()) {
    sound3.play(AudioSampleKick);
    Serial.println("AudioSampleKick");
  }
  if (button4.pressed()) {
    // comment this line to work with Teensy 3.0.
    // the Gong sound is very long, too much for 3.0's memory
    sound4.play(AudioSampleGong);
    Serial.println("AudioSampleGong");
  }
  if (button5.pressed()) {
    sound5.play(AudioSampleCashregister);
    Serial.println("AudioSampleCashregister");
  }

}
