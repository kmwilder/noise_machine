# noise_machine
"Noise machine" for Arduino.

Goal: Choose one of N "white noise" sounds to auto-play around 5am each morning.

Features:
* Buttons to start/stop/choose next sound.
* Base configuration via SD card: set alarm time in CFG file, default sound, available sounds, etc.

Parts:
  1 Arduino Uno (v???)
  1 Seeed Studio SD Card Shield (v3.1)
  
  Audio Amplifier: (cribbed from https://www.youtube.com/watch?v=4ObzEft2R_g)
    1 Speaker
    1 LM386N-4 audio amplifier IC
    1 1kOhm resistor
    Capacitors:
      1+1 100nF
      2 100uF, 25V
      1 1000uF, 25V
      1 10uF cap
    Volume control:
      1 10kOhm variable resistor
  
Thanks:
  https://circuitdigest.com/microcontroller-projects/arduino-audio-music-player
  http://www.circuitbasics.com/wp-content/uploads/2015/04/Build-a-Great-Sounding-Audio-Amplifier-with-Bass-Boost-from-the-LM386-Amplifier-With-Gain-Schematic.png
  https://forum.arduino.cc/index.php?topic=37693.0
