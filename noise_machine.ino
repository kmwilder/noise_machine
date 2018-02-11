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

//==========================================
void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  music.speakerPin = 9;
  music.setVolume(5);
  music.quality(1);

  setTime(0,0,0,0,0,0);
  
  // LED
  pinMode(LED_PIN, OUTPUT);
  
  // SD init
  //initSD();

  // Read config from SD card
  readConfig();

  // buzzer
  playSong();
}

//======================================
void loop() {
  digitalClockDisplay();
  Alarm.delay(1000);
}

void readConfig() {
  String waveFile = "";
  int hour = 0;
  int minute = 0;

  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect)) {
      Serial.println("SD init failed!");
  } else {
    Serial.println("SD init done!");
  }

  if(SD.exists("CFG.TXT"))
    Serial.println("CFG.TXT exists.");
  else
    Serial.println("CFG.TXT doesn't exist.");
  
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

void digitalClockDisplay()
{
 // digital clock display of the time
 Serial.print(hour());
 Serial.print(":");
 Serial.print(minute());
 Serial.print(":");
 Serial.print(second());
 Serial.println();
}

//======================================
void wakeUpAlarm() {
  playSong();
}

void playTone() {

  char notes[] = "cdfda ag cdfdg gf";
  const int noteDuration = 1000 / 4;
  
  Serial.println("Timer triggered...");
  for (int thisNote = 0; thisNote < 17; thisNote++) {
    int noteDuration = 1000 / 4;
    tone(9, notes[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    noTone(9);
  }
}

void playSong() {
  digitalWrite(LED_PIN, HIGH);
  music.play("noc.wav");
}

/*
 
//======================================
void initSD() {

  // set up variables using the SD utility library functions:
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}
 
 */

