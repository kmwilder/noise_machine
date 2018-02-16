// Music Libraries
#include <pcmConfig.h>
#include <pcmRF.h>
#include <TMRpcm.h>

// Time / Alarm Libraries
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

// SD Card Libraries
#include <SPI.h>
#include <SD.h>

// Globals
const int BUZZER_PIN = 9;
const int LED_PIN = 8;
const int ON_BUTTON = 1;
const int NEXT_BUTTON = 2;
// SD card reader uses pins 10-13

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int chipSelect = 10;

// Audio
String waveFile;
TMRpcm music;
bool isPlaying = false;

//==========================================
// SETUP
//==========================================
void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // SD card init
  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect)) {
      Serial.println("SD init failed!");
  } else {
    Serial.println("SD init done!");
  }

  // Music init
  music.speakerPin = 9;
  music.setVolume(5);
  music.quality(1);
  music.loop(1);

  setTime(0,0,0,0,0,0); // default, if CFG file not read properly
  
  pinMode(ON_BUTTON, INPUT);
  pinMode(NEXT_BUTTON, INPUT);

  // Read config from SD card, set alarms accordingly
  readConfig();
}

//==========================================
// LOOP
//==========================================
void loop() {
  digitalClockDisplay(); // TODO remove
  Alarm.delay(1000);

  // TODO replace this with the buttons to start/stop, knobs to select track
  int onButtonState = digitalRead(ON_BUTTON);
  int nextButtonState = digitalRead(NEXT_BUTTON);
  if(onButtonState == LOW) {
    Serial.println("Button pressed!");
    if(isPlaying == true) {
      isPlaying = false;
      music.pause();
    } else {
      isPlaying = true;
      music.play("noc.wav");
    }
  }
}

//==========================================
// HELPERS
//==========================================
void readConfig() {
  int hour = 0;
  int minute = 0;
  
  File myFile;
  myFile = SD.open("CFG.TXT", FILE_READ);
  if(myFile) {
      Serial.println("Opened config.txt!");
  } else {
      Serial.println("Failed to open config...");
      setTime(0,0,0,0,0,0);
      return;
  }
  
  String temp;
  temp = readLine(myFile);
  if(temp != "") {
    hour = temp.toInt();
  }
  temp = readLine(myFile);
  if(temp != "") {
    minute = temp.toInt();
  }
  setTime(hour, minute, 0, 0, 0, 0);
  Serial.print("Set time to ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);

  // Set waveFile path from 3rd line of file
  waveFile = readLine(myFile);
 
  // Set alarms based on next few lines
  temp = readLine(myFile);
  if(temp != "") {
    hour = temp.toInt();
  }
  temp = readLine(myFile);
  if(temp != "") {
    minute = temp.toInt();
  }
  Alarm.alarmRepeat(hour,minute,0,wakeUpAlarm);  
  Serial.print("Set alarm for ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);

  myFile.close();
}

String readLine(File file) {
    String received = "";
    char ch;
    while(file.available()) {
      ch = file.read();
      if(ch == '\n') {
        return String(received);
      } else {
        received += ch;
      }
    }
    return received;
}

void digitalClockDisplay() {
 // digital clock display of the time
 Serial.print(hour());
 Serial.print(":");
 Serial.print(minute());
 Serial.print(":");
 Serial.print(second());
 Serial.println();
}

void wakeUpAlarm() {
  music.play("noc.wav");
  isPlaying = true;
}
