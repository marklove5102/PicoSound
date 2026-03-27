/*
 * PicoSound User Configuration - WAV Player Example
 *
 * Copy this file to your project's include/ folder as:
 * picosound_user_cfg.h
 * 
 * Then customize pin assignments and sound definitions.
 */

#ifndef PICOSOUND_USR_CFG_H
#define PICOSOUND_USR_CFG_H

#include <PicoSound.h>

//=============================================================================
// HARDWARE CONFIGURATION
//=============================================================================
// Output mode: OUT_I2S (MAX98357A) or OUT_PWM (speaker/amplifier)
#define USER_SND_OUT    OUT_I2S

// I2S pins (for MAX98357A)
#define USER_PIN_BCK    14
#define USER_PIN_WS     15
#define USER_PIN_DATA   16

// PWM pin (for speaker/amplifier)
#define USER_PIN_SPKR   17

//=============================================================================
// MIXER SETTINGS
//=============================================================================
#define PICOSOUND_USER_NUM_CHANNELS     4
#define PICOSOUND_USER_MASTER_VOLUME   80

//=============================================================================
// LOUDNESS COMPENSATION
//=============================================================================
inline const float PICOSOUND_LOUDNESS_FACTOR[] = {
  1.0f,   // WAVE_NONE
  2.0f,   // WAVE_SINE
  0.9f,   // WAVE_SQUARE
  1.5f,   // WAVE_SAWTOOTH
  1.5f,   // WAVE_TRIANGLE
  0.7f,   // WAVE_NOISE
  1.0f,   // WAVE_WAV
  1.0f    // WAVE_EXPLOSION
};

//=============================================================================
// SOUND IDs
//=============================================================================
typedef enum {
  SND_NONE = 0,
  SND_BEEP,         // Synthesized fallback
  SND_WAV_1,        // sound1.wav from LittleFS
  SND_WAV_2,        // sound2.wav from LittleFS
  SND_WAV_3,        // sound3.wav from LittleFS (continuous looping)
  SND_MAX
} SoundID;

//=============================================================================
// SOUND DEFINITION TABLE
//=============================================================================
inline const SoundDefinition PICOSOUND_TABLE[] = {
  // SND_NONE
  {WAVE_NONE, 0, 0, 0, 0, 100, nullptr, 0, false, nullptr, false},
  
  // SND_BEEP - Fallback synthesized sound
  {WAVE_SQUARE, 440, 0, 100, 12000, 80, nullptr, 0, false, nullptr, false},
  
  // SND_WAV_1 - Play sound1.wav from LittleFS
  {WAVE_WAV, 0, 0, 0, 0, 80, nullptr, 0, false, "/sound1.wav", true},
  
  // SND_WAV_2 - Play sound2.wav from LittleFS
  {WAVE_WAV, 0, 0, 0, 0, 80, nullptr, 0, false, "/sound2.wav", true},
  
  // SND_WAV_3 - Play sound3.wav from LittleFS (looping)
  {WAVE_WAV, 0, 0, 0, 0, 70, nullptr, 0, true, "/sound3.wav", true},
};

#endif
