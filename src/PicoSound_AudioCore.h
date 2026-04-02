/*
 * PicoSound_AudioCore.h
 * 
 * Setup and Loop for Core1 (Audio Engine)
 * 
 */

#ifndef PICOSOUND_AUDIOCORE_H
#define PICOSOUND_AUDIOCORE_H

#include <Arduino.h>
#include <PicoSem.h>
#include <PicoSound.h>
#include <LittleFS.h>

//=============================================================================
// GLOBAL AUDIO VARIABLES (shared between Core0 and Core1)
//=============================================================================
extern PicoSem AudioSem;
extern AudioCommand audioCmd;
extern PICOSOUND PicoSound;

//=============================================================================
// CORE1 FUNCTIONS (implemented in PicoSound_AudioCore.cpp)
//=============================================================================
/**
 * @brief Initialize the audio system on Core1
 * Call this function from setup1()
 */
void PicoSound_AudioCore_Setup1();

/**
 * @brief Main loop of the audio system on Core1
 * Call this function from loop1()
 */
void PicoSound_AudioCore_Loop1();

#endif // PICOSOUND_AUDIOCORE_H
