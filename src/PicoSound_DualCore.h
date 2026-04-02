/*
 * PicoSound_DualCore.h
 * 
 * Helper functions for dual-core usage of PicoSound
 * High-level wrapper for Core0 → Core1 communication via PicoSem
 * 
 * DEPENDENCIES:
 * - PicoSound.h
 * - PicoSem.h
 * 
 * REQUIRED SETUP:
 * You must define in your project (main.cpp or AudioCore.cpp):
 *   extern PicoSem AudioSem;
 *   extern AudioCommand audioCmd;
 */

#ifndef PICOSOUND_DUALCORE_H
#define PICOSOUND_DUALCORE_H

#include <PicoSound.h>
#include <PicoSem.h>

//=============================================================================
// EXTERNAL VARIABLES (to be defined in main project)
//=============================================================================
extern PicoSem AudioSem;
extern AudioCommand audioCmd;
extern PICOSOUND PicoSound;  

//=============================================================================
// DUAL-CORE WRAPPER FUNCTIONS
//=============================================================================
/**
 * @brief Send audio command to Core1
 * @param cmdType Command type (CMD_PLAY_SOUND, CMD_PLAY_CUSTOM, etc.)
 * @param soundID Sound ID (from table)
 * @param volume Volume 0-100
 * @param wave Wave type (WAVE_SQUARE, WAVE_SINE, etc.)
 * @param freq_start Start frequency (Hz)
 * @param freq_end End frequency (Hz) for sweep
 * @param duration Duration (ms)
 * @param notes Array of notes (for melodies)
 * @param numNotes Number of notes
 */
void SendAudioCommand(AudioCommandType cmdType, uint8_t soundID = 0, 
                      uint8_t volume = 80, WaveType wave = WAVE_SQUARE,
                      float freq_start = 0, float freq_end = 0, uint16_t duration = 0,
                      const Note* notes = nullptr, uint8_t numNotes = 0,
                      uint8_t channel = 255);

/**
 * @brief Play a melody on Core1
 * @param notes Array of notes
 * @param numNotes Number of notes
 * @param wave Wave type (default: WAVE_SQUARE)
 * @param volume Volume 0-100 (default: 80)
 */
void PlayMelody(const Note* notes, uint8_t numNotes, 
                WaveType wave = WAVE_SQUARE, uint8_t volume = 80);

/**
 * @brief Play a simple tone (compatibility with tone())
 * @param frequency Frequency in Hz
 * @param duration Duration in ms
 * @param volume Volume 0-100 (default: 80)
 */
void PlayTone(int frequency, int duration, uint8_t volume = 80);

/**
 * @brief Stop all sounds
 */
void StopAllSounds();

/**
 * @brief Stop a specific sound
 * @param soundID Sound ID to stop
 */
void StopSound(uint8_t soundID);

/**
 * @brief Set master volume
 * @param volume Volume 0-100
 */
void SetMasterVolume(uint8_t volume);

#endif // PICOSOUND_DUALCORE_H
