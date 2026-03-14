// MemoryAndCpuUsage
//
// This example demonstrates how to monitor CPU and memory
// usage by the audio library.  You can see the total memory
// used at any moment, and the maximum (worst case) used.
//
// The total CPU usage, and CPU usage for each object can
// be monitored.  Reset functions clear the maximums.
//
// Use the Arduino Serial Monitor to view the usage info
// and control ('F', 'S', and 'R' keys) this program.
//
// This example code is in the public domain.


//#include <Adafruit_TinyUSB.h>
#include <pico-audio.h>
#include <Wire.h>
#include <SPI.h>


#define BCLK  12
#define WS    13
#define MCLK  11
#define I2S_DATA_OUT  14
#define I2S_DATA_IN  15

AudioInputI2S           input;
AudioOutputI2S           DAC;           //xy=640,161
AudioConnection          patchCord6(input, 0, DAC, 0);
AudioConnection          patchCord7(input, 1, DAC, 1);


void setup() {

  Serial.begin(115200);


	DAC.begin(BCLK,WS,MCLK,I2S_DATA_IN,I2S_DATA_OUT);
//	DAC.begin(BCLK,WS,I2S_DATA_OUT);
  // give the audio library some memory.  We'll be able
  // to see how much it actually uses, which can be used
  // to reduce this to the minimum necessary.
  AudioMemory(10);

}


int count = 0;
int speed = 60;


void loop() {

/*
  // print a summary of the current & maximum usage

  Serial.print("all=");
  Serial.print(AudioProcessorUsage());
  Serial.print(",");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("    ");
  Serial.print("Memory: ");
  Serial.print(AudioMemoryUsage());
  Serial.print(",");
  Serial.print(AudioMemoryUsageMax());
  Serial.print("    ");

  Serial.println();


  delay(speed);
*/
}

