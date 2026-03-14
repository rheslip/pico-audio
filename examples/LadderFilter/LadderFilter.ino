// Ladder filter demo with continuous 3-saw drone into the filter
// with separate LFOs modulating filter frequency and resonance.
// By Richard van Hoesel
// https://forum.pjrc.com/threads/60488?p=269756&viewfull=1#post269756

// Ladder filter demo with continuous 3-saw drone into the filter
// with separate LFOs modulating filter frequency and resonance.
// By Richard van Hoesel
// https://forum.pjrc.com/threads/60488?p=269756&viewfull=1#post269756

#include <Adafruit_TinyUSB.h>
#include <pico-audio.h>

/*
// I2S pins for DAC
#define BCLK 13
#define WS 14  // this will always be 1 pin above BCLK - can't change it
#define I2S_DATA 15
*/

// for 2HPico DSP board
#define BCLK  12
#define WS    13
#define MCLK  11
#define I2S_DATA_OUT  14
#define I2S_DATA_IN  15

AudioSynthWaveform waveform1;
AudioSynthWaveform waveform2;
AudioSynthWaveform waveform3;
AudioMixer4 mixer1;
AudioFilterLadder filter1;
AudioSynthWaveform lfo1;
AudioSynthWaveform lfo2;
AudioOutputI2S i2s1;
// AudioOutputUSB         usb1;
// udioControlSGTL5000     sgtl5000_1;

AudioConnection patchCord1(waveform1, 0, mixer1, 0);
AudioConnection patchCord2(waveform2, 0, mixer1, 1);
AudioConnection patchCord3(waveform3, 0, mixer1, 2);
AudioConnection patchCord4(mixer1, 0, filter1, 0);
AudioConnection patchCord5(lfo1, 0, filter1, 1);
AudioConnection patchCord6(lfo2, 0, filter1, 2);
AudioConnection patchCord7(filter1, 0, i2s1, 0);
AudioConnection patchCord8(filter1, 0, i2s1, 1);
// AudioConnection patchCord9(filter1, 0, usb1, 0);
// AudioConnection patchCord10(filter1, 0, usb1, 1);

void setup()
{
  Serial.begin(115200);
  AudioMemory(10);
  //  sgtl5000_1.enable();
  //  sgtl5000_1.volume(0.6);
 // i2s1.begin(BCLK,WS,I2S_DATA_OUT);
  i2s1.begin(BCLK,WS,I2S_DATA_OUT,I2S_DATA_IN,MCLK);
  filter1.resonance(0.55);    // "lfo2" waveform overrides this setting
  filter1.frequency(800);     // "lfo1" modifies this 800 Hz setting
  filter1.octaveControl(2.6); // up 2.6 octaves (4850 Hz) & down 2.6 octaves (132 Hz)
  waveform1.frequency(50);
  waveform2.frequency(100.1);
  waveform3.frequency(150.3);
  waveform1.amplitude(0.1);
  waveform2.amplitude(0.1);
  waveform3.amplitude(0.1);
  waveform1.begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
  waveform2.begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
  waveform3.begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
  lfo1.frequency(0.2);
  lfo1.amplitude(0.99);
  lfo1.phase(270);
  lfo1.begin(WAVEFORM_TRIANGLE);
  lfo2.frequency(0.07);
  lfo2.amplitude(0.55);
  lfo2.phase(270);
  lfo2.begin(WAVEFORM_SINE);
}

void loop()
{
  Serial.print("Filter CPU Usage: ");
  Serial.print(filter1.processorUsageMax());
  Serial.print("%, Total CPU Usage: ");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("%");
  Serial.print(" Mem = ");
  Serial.print(AudioMemoryUsage());
  Serial.print(" (");
  Serial.print(AudioMemoryUsageMax());
  Serial.println(")");
  filter1.processorUsageMaxReset();
  AudioProcessorUsageMaxReset();
  delay(1000);
}
