# pico-audio

PJRC Teensy Audio library ported to Pico 2/RP2350.

Updated March 14 2026: 

- fixed a bug in AudioOutputI2S - checks for valid input buffer pointers now - old code attempted to release invalid buffers to the pool. This was causing bad audio when the previous object passed null buffer pointers (it happens - by design). Should fix the the invalid memory reporting as well.

- added an AudioOutputPT8211 object

- added new AudioOutputi2S begin() function which initializes MCLK for PCM1808 audio ADC compatibility.

Requires RP2350 (Pico 2, Pico 2 W or compatible boards) - will NOT run on RP2040. RP2350 has DSP extensions which are required for this library to work correctly.

Most of the Teensy Audio functions have been ported. While not extensively tested seems to work as it does on Teensy 3.2 and Teensy 4. Functions that have not been ported are for the most part specific to Teensy hardware. Teensy sketches will require modifications to define the RP2350 I2S pins in use and likely other modifications to Teensy specific code. DSP intensive code may require overclocking the RP2350 - see comments about perfmance below.

Note that Teensy 4 and 4.1 are MUCH faster that the RP2350 (approx 5x) so if you need high performance DSP use Teensy 4.

This fork has been modifed from the original 96k/256 sample port to use 48k samples per second and 128 sample blocks to maximize compatibilty with Teensy code. Lower samplerate=lower CPU usage.

The AudioOutputI2S object uses 32 bit I2S audio input and output - 16 bit audio samples are scaled up and down as needed. It has been tested with the PCM1808 audio ADC and the PCM5102A audio DAC. 

An AudioOutputPT8211 16 bit I2S output object has been added to support the PT8211 audio DAC. The PT8211 uses non-standard I2S timing.

Library has two added modules: Synth_DaisySP and Effect_DaisySP which allow usage of Electrosmith's DaisySP DSP library with Teensy Audio. 
https://github.com/electro-smith/DaisySP
https://github.com/rheslip/DaisySP_Teensy


Tested with Raspberry Pi Pico 2 board, Arduino IDE 2.3.6 with Pico Arduino 4.5.3, PCM5102 I2S DAC module, PT8211 DAC module and PCM1808 audio ADC module.

To run the PolySynth example, select the Pico 2 board and USB stack Adafruit TinyUSB. Ladderfilter example may require overclocking. Most of the original Teensy Audio examples will run with minor modifications.

Known Issues:

Sometimes memory usage reports wildly wrong numbers - I think this is fixed as of March 2026

FFT functions still not working reliably