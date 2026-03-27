/*
 * PicoSound - WAV Player Example (PlatformIO)
 * 
 * Demonstrates playing WAV files from LittleFS filesystem.
 * 
 * SETUP:
 * 1. Place WAV files in data/ folder
 * 2. WAV files must be: 16kHz, 16-bit, Mono
 * 3. Upload filesystem: pio run --target uploadfs
 * 4. Upload program: pio run --target upload
 * 
 * HARDWARE:
 * - Button on GP18 (play sound1.wav)
 * - Button on GP19 (play sound2.wav)
 * - Button on GP20 (play sound3.wav - looping)
 * - I2S audio output on GP14/15/16
 */

#include <Arduino.h>
#include <LittleFS.h>
#include <PicoSound_AudioCore.h>
#include <PicoSound_DualCore.h>

//=============================================================================
// BUTTON PINS
//=============================================================================
#define BTN_SOUND1   18
#define BTN_SOUND2   19
#define BTN_SOUND3   20

// Enable next define if you are using an I2S module 
// (like a MAX98357A) controlled by a GPIO pin
// see: https://github.com/IWILZ/PicoSound/blob/main/docs/HARDWARE.md
// ----------------------------------------------------------------------------
// #define I2S_DS_PIN    0   // GPIO pin to mute/unmute MAX98357A

//=============================================================================
// SETUP
//=============================================================================
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== PicoSound WAV Player Example ===");
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("ERROR: LittleFS mount failed!");
    Serial.println("Run: pio run --target uploadfs");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("LittleFS mounted successfully");
  
  // List files in root directory
  Serial.println("\nFiles on LittleFS:");
  Dir dir = LittleFS.openDir("/");
  bool foundWav = false;
  while (dir.next()) {
    Serial.print("  ");
    Serial.print(dir.fileName());
    Serial.print(" (");
    Serial.print(dir.fileSize());
    Serial.println(" bytes)");
    if (dir.fileName().endsWith(".wav")) {
      foundWav = true;
    }
  }
  
  if (!foundWav) {
    Serial.println("\nWARNING: No WAV files found!");
    Serial.println("1. Place WAV files (16kHz, 16-bit, Mono) in data/ folder");
    Serial.println("2. Run: pio run --target uploadfs");
  }
  
  // Setup buttons
  pinMode(BTN_SOUND1, INPUT_PULLUP);
  pinMode(BTN_SOUND2, INPUT_PULLUP);
  pinMode(BTN_SOUND3, INPUT_PULLUP);

  if (USER_SND_OUT==OUT_I2S){
    #ifdef I2S_DS_PIN
    pinMode(I2S_DS_PIN, OUTPUT);      // MAX98357A DS control pin
    digitalWrite(I2S_DS_PIN, HIGH);   // enable MAX98357A
    #endif
  }

  Serial.println("\nReady! Press buttons:");
  Serial.println("  GP18 - Play sound1.wav");
  Serial.println("  GP19 - Play sound2.wav");
  Serial.println("  GP20 - Play sound3.wav (looping)");
  Serial.println("  (Press GP20 again to stop loop)");
}

//=============================================================================
// MAIN LOOP
//=============================================================================
void loop() {
  // Button 1 - Play sound1.wav
  if (digitalRead(BTN_SOUND1) == LOW) {
    Serial.println("Playing: sound1.wav");
    SendAudioCommand(CMD_PLAY_SOUND, SND_WAV_1, 80);
    delay(300);
  }
  
  // Button 2 - Play sound2.wav
  if (digitalRead(BTN_SOUND2) == LOW) {
    Serial.println("Playing: sound2.wav");
    SendAudioCommand(CMD_PLAY_SOUND, SND_WAV_2, 80);
    delay(300);
  }
  
  // Button 3 - Toggle sound3.wav (looping)
  static bool looping = false;
  if (digitalRead(BTN_SOUND3) == LOW) {
    if (!looping) {
      Serial.println("Playing: sound3.wav (loop)");
      SendAudioCommand(CMD_PLAY_SOUND, SND_WAV_3, 70);
      looping = true;
    } else {
      Serial.println("Stopping loop");
      StopAllSounds();
      looping = false;
    }
    delay(300);
  }
}

//=============================================================================
// CORE1: Audio engine
//=============================================================================
void setup1() {
  PicoSound_AudioCore_Setup1();
}

void loop1() {
  PicoSound_AudioCore_Loop1();
}
