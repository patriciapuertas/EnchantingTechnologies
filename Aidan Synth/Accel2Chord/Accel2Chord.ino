/*
Teensy Synthesis Example, built using the Audio Design Tool provided
by PJRC. This sketch requires the Teensy 3.x and Prop Shield with
speaker attached. The sketch will play sound automatically once a 
serial port is opened. Moving the Prop Shield around will change the
sound.

Fab-Cre8 Enchanting Technologies
by Aidan Taylor 2018
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

NXPMotionSense imu;
NXPSensorFusion filter;

// GUItool: begin automatically generated code
AudioSynthWaveformModulated wave6; //xy=138,470
AudioSynthWaveformModulated wave3;   //xy=139,356
AudioSynthWaveformModulated wave4; //xy=139,402
AudioSynthWaveformModulated wave1;   //xy=140,288
AudioSynthWaveformModulated wave5; //xy=140,436
AudioSynthWaveformModulated wave2;   //xy=141,322
AudioMixer4              mix2; //xy=300,442
AudioMixer4              mix1;         //xy=301,328
AudioFilterStateVariable filter2; //xy=439,448
AudioFilterStateVariable filter1;        //xy=440,334
AudioAmplifier           amp2; //xy=587,436
AudioAmplifier           amp1;           //xy=588,322
AudioMixer4              mix3; //xy=737,392
AudioOutputAnalog        dac1;           //xy=873,392
AudioConnection          patchCord1(wave6, 0, mix2, 2);
AudioConnection          patchCord2(wave3, 0, mix1, 2);
AudioConnection          patchCord3(wave4, 0, mix2, 0);
AudioConnection          patchCord4(wave1, 0, mix1, 0);
AudioConnection          patchCord5(wave5, 0, mix2, 1);
AudioConnection          patchCord6(wave2, 0, mix1, 1);
AudioConnection          patchCord7(mix2, 0, filter2, 0);
AudioConnection          patchCord8(mix1, 0, filter1, 0);
AudioConnection          patchCord9(filter2, 0, amp2, 0);
AudioConnection          patchCord10(filter1, 0, amp1, 0);
AudioConnection          patchCord11(amp2, 0, mix3, 1);
AudioConnection          patchCord12(amp1, 0, mix3, 0);
AudioConnection          patchCord13(mix3, dac1);
// GUItool: end automatically generated code

byte ampEnable = 5;
float semitone = 1.059;

void setup() {
  //Activate Serial Port
  Serial.begin(115200);
  while(!Serial);

  // Allow some memory for audio connections
  AudioMemory(8);
  delay(2000);

  // Enable the amplifier on the prop shield:
  pinMode(ampEnable, OUTPUT);
  digitalWrite(5, HIGH); // turn on the amplifier
  delay(10);             // allow time to wake up
  
  // Amin chord
  wave1.begin(0.6, 220.0, WAVEFORM_SAWTOOTH); // A3
  wave2.begin(0.6, 261.63, WAVEFORM_SAWTOOTH); // C4
  wave3.begin(0.6, 329.63, WAVEFORM_SAWTOOTH); // E4

  // Amin chord
  wave4.begin(0.6, 164.81, WAVEFORM_SAWTOOTH); // E3
  wave5.begin(0.6, 196.00, WAVEFORM_SAWTOOTH); // G3
  wave6.begin(0.6, 246.94, WAVEFORM_SAWTOOTH); // B3

  mix1.gain(0, 0.8);
  mix1.gain(1, 0.8);
  mix1.gain(2, 0.8);

  mix2.gain(0, 0.8);
  mix2.gain(1, 0.8);
  mix2.gain(2, 0.8);

  filter1.frequency(0);
  filter1.resonance(0.8);

  filter2.frequency(0);
  filter2.resonance(0.8);

  amp1.gain(0.2); // This can get loud!!
  amp2.gain(0.2); // This can get loud!!

  imu.begin();
  filter.begin(100);

  Serial.println("Let the drone begin...");
  
}

void loop() {
  float lpFreq = 0;
  float wav1F = 220.00;
  float wav2F = 261.63;
  float wav3F = 329.63;
  float wav4F = 164.81;
  float wav5F = 196.00;
  float wav6F = 246.94;
  float wavMod = 0;
  
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
//    Serial.print("Orientation: ");
//    Serial.print(heading);
//    Serial.print(" ");
//    Serial.print(pitch);
//    Serial.print(" ");
//    Serial.println(roll);
  }

  // Rectify the heading variable:
  heading = heading - 180.0;
  if(heading < 0) {
    heading = heading*-1.0;
  }
  Serial.println(heading);

  lpFreq = map(heading, 0, 180, 120, 10000);
  wavMod = ((pitch+80.0)/80.0)-1.0;
  //Serial.println(wavMod);

  wave1.frequency(wav1F+wavMod);
  wave2.frequency(wav2F+(wavMod*(semitone*3)));
  wave3.frequency(wav3F+(wavMod*(semitone*7)));
  wave4.frequency(wav4F+wavMod);
  wave5.frequency(wav5F+(wavMod*(semitone*3)));
  wave6.frequency(wav6F+(wavMod*(semitone*7)));

  filter1.frequency(lpFreq);
  filter2.frequency(10000-lpFreq);

}
