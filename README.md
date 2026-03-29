# PicoSound

![PicoSound Audio Library](https://raw.githubusercontent.com/IWILZ/PicoSound/main/docs/images/PicoSound_banner.jpg)

[![RP2040](https://img.shields.io/badge/RP2040-Raspberry%20Pi%20Pico-C51A4A?logo=raspberrypi)](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)
[![Arduino](https://img.shields.io/badge/Arduino.IDE-Compatible-00979D?logo=arduino)](https://arduino-pico.readthedocs.io/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange?logo=platformio)](https://registry.platformio.org/platforms/platformio/raspberrypi)
[![License](https://img.shields.io/badge/License-GPL--3.0-blue)](LICENSE)

Dual-core audio engine for Raspberry Pi Pico (RP2040).

PicoSound delivers glitch-free audio for any RP2040 application by dedicating 
Core1 exclusively to audio processing. While your main program runs on Core0 
(handling graphics, I/O, calculations, or game logic), Core1 manages the audio 
mixer and output independently.

This architecture eliminates audio dropouts caused by blocking operations like 
SPI transfers, flash writes, or heavy computations. Audio continues uninterrupted 
regardless of what Core0 is doing.

Supports both **synthesized sounds** (waveforms, melodies, effects) and streaming 
from **WAV files**. Suitable for any project requiring reliable 
audio output: synthesizers, musical instruments, notification systems, interactive 
installations, games or educational tools.

Works seamlessly with both Arduino IDE and PlatformIO. The library uses 
standard C++ structure (.h/.cpp files) ensuring compatibility across 
development environments.

## Configuration Approach

Simple examples are provided and depending on the development environment (**Arduino IDE** or **PlatformIO**), they differ only in the place you can configure your sounds:

```
 * Arduino IDE ----→ into your *.ino file
 * PlatformIO  ----→ inside a dedicated config.h file   
```

## Features

- Non-blocking audio on Core1
- 8-channel mixer
- I2S (MAX98357A) or PWM output
- Synthesized sounds (sine, square, sawtooth, triangle, noise, explosion)
- Melody sequencer
- WAV streaming (LittleFS or PROGMEM)
- Frequency sweeps
- Arduino IDE and PlatformIO compatible


## Software Architecture

```
Core0: Program logic     Core1: Audio engine
        ↓                       ↓
SendAudioCommand() ----→ PicoSound.loop()
  (via PicoSem)          (mixer + output)
```

Audio runs independently. No blocking, no glitches.


## Overall resource consumption

| Resource | Used | Pi Pico | % |
|-------|-------|---------|---|
| RAM | 4KB | 264KB | 1.5% |  
| Flash | 20KB | 2MB | 1% |
| DMA channels | 1 | 12 | 8% |
| Cores | 1 | 2 | 50% |


- Audio latency: ~35µs  
- WAV files: ~32 seconds/MB (e.g. 4 sounds of ~4" and 8 of ~2", for each MB)

Above values ​​refer to a standard configuration: 16Khz - 16 bit/sample - Mono - 8 Channel Mixer  

If you need **more space for WAV sounds**, use a `Pi Pico with 16MB Flash` avaliable on Amazon or AliExpress

To experiment different sample frequency:

```
Arduino IDE:  edit PicoSound.h and change the value of PICOSOUND_DEFAULT_SAMPLE_RATE  

PlatformIO:   in your platformio.ini add -D PICOSOUND_DEFAULT_SAMPLE_RATE="your rate"
```

--------------

## Arduino IDE - quick start

### 1. Install

Download: Code → Download ZIP  
Arduino IDE: Sketch → Include Library → Add .ZIP Library

### 2. Code

```cpp
// ========== CONFIGURATION (before includes) ==========
#define USER_SND_OUT    OUT_I2S
#define USER_PIN_BCK    14
#define USER_PIN_WS     15
#define USER_PIN_DATA   16

typedef enum {
  SND_NONE = 0,
  SND_BEEP,
  SND_LASER,
} SoundID;

// ========== INCLUDES ==========
#include <PicoSound_AudioCore.h>
#include <PicoSound_DualCore.h>

// ========== SOUND TABLE ==========
const SoundDefinition PICOSOUND_TABLE[] = {
  {WAVE_NONE, 0, 0, 0, 0, 100, nullptr, 0, false, nullptr, false},
  {WAVE_SQUARE, 440, 0, 100, 12000, 80, nullptr, 0, false, nullptr, false},
  {WAVE_SQUARE, 1500, 200, 200, 12000, 70, nullptr, 0, false, nullptr, false},
};

// ============ YOUR PROGRAM ON CORE0 =============
void setup() {
  // Your setup here
}
void loop() {
  // ...
  // Your computing logic, graphics, I/O, etc
  // ...  

  SendAudioCommand(CMD_PLAY_SOUND, SND_BEEP, 80);
  delay(1000);    // Your program can be "blocking"
}

// ========== AUDIO ENGINE RUNS ON CORE1 ==========
void setup1() {
  PicoSound_AudioCore_Setup1();
}
// ======= NON-BLOCKING LOOP TO PLAY AUDIO ========
void loop1() {
  PicoSound_AudioCore_Loop1();
}  
```

See [examples](examples/) for complete code.

-------------------

## PlatformIO - quick start

### 1. Install Library

If you already have a `platformio.ini` into your project, just add:

```ini
lib_deps = 
    https://github.com/IWILZ/PicoSound.git
    https://github.com/IWILZ/PicoSem.git

; Memory config to reserve space for WAVs on LittleFS
board_build.filesystem_size = 1m
board_build.filesystem = littlefs
```
otherwise for a full example, see the README in the `example/` folder.

### 2. Configuration Templates

The library provides three **templates** to help you better organize your work by dividing (where appropriate) the definition of your sounds into:

- picosound_user_cfg_template.h --> synthesized sounds and LittleFS files
- picosound_melodies_template.h --> melody definitions
- picosound_samples_template.h --> WAV samples stored in PROGMEM

You can simply use `picosound_user_cfg_template.h` to create the **mandatory** `picosound_user_cfg.h` file, including the other 2 inside it when the number of your sounds increases and you want a better organization for your work.

```bash
cp lib/PicoSound/src/templates/picosound_user_cfg_template.h include/picosound_user_cfg.h
```

Or manually copy from `lib/PicoSound/src/templates/` to your project's `include/` folder.

### 3. Customize Configuration

Edit `include/picosound_user_cfg.h`:

```cpp
// Set your hardware pins
#define USER_SND_OUT    OUT_I2S
#define USER_PIN_BCK    14
#define USER_PIN_WS     15
#define USER_PIN_DATA   16

// Define your sounds
typedef enum {
  SND_NONE = 0,
  SND_BEEP,
  SND_LASER,
  SND_EXPLOSION,
  SND_MAX    // Always end with SND_MAX
} SoundID;

// Add sounds to table
inline const SoundDefinition PICOSOUND_TABLE[] = {
  {WAVE_NONE, 0, 0, 0, 0, 100, nullptr, 0, false, nullptr, false},
  {WAVE_SQUARE, 440, 0, 100, 12000, 80, nullptr, 0, false, nullptr, false},
  // ... your sounds ...
};
```


### 4. Write Your Code

```cpp
#include <Arduino.h>
#include <PicoSound_AudioCore.h>
#include <PicoSound_DualCore.h>

// ============ YOUR PROGRAM ON CORE0 =============
void setup() {
  // Your setup here
}
void loop() {
  // ...
  // Your computing logic, graphics, I/O, etc
  // ...  

  SendAudioCommand(CMD_PLAY_SOUND, SND_BEEP, 80);
  delay(1000);    // Your program can be "blocking"
}

// ========== AUDIO ENGINE RUNS ON CORE1 ==========
void setup1() {
  PicoSound_AudioCore_Setup1();
}
// ======= NON-BLOCKING LOOP TO PLAY AUDIO ========
void loop1() {
  PicoSound_AudioCore_Loop1();
}

```

Configuration is loaded automatically from your `include/picosound_user_cfg.h`.

### 5. Build and Upload

```bash
pio run --target upload
```

------------------------------

## API

High-level functions (recommended):

```cpp
PlayTone(frequency, duration, volume);
PlayMelody(melody_array, length, waveform, volume);
SendAudioCommand(CMD_PLAY_SOUND, sound_id, volume);
StopAllSounds();
SetMasterVolume(volume);
```

See [docs/API.md](docs/API.md) for complete reference.

## Hardware Setup

### I2S (MAX98357A)
```
Pico GP14 → BCK
Pico GP15 → WS (LRCLK)
Pico GP16 → DIN
MAX98357A SD → 3.3V (or GPIO for mute control)
```

### PWM (Speaker/Amplifier)
```
Pico GP17 → Speaker (+) via capacitor
GND → Speaker (-)
```

See [docs/HARDWARE.md](docs/HARDWARE.md) for detailed description.

## Configuration Styles Comparison

| Approach | Best For | Config Location |
|----------|----------|-----------------|
| **Inline** | Arduino IDE | In `.ino` file |
| **Separate files** | PlatformIO | `include/picosound_user_cfg.h` |

### Inline Configuration

**Advantages:**
- ✅ Works perfectly in Arduino IDE
- ✅ Configuration per-project
- ✅ Survives library updates
- ✅ Self-documenting code

**Disadvantages:**
- ⚠️ Large sound tables in main file
- ⚠️ Less modular for complex projects

### Separate Config Files

**Advantages:**
- ✅ Cleaner code organization
- ✅ Reusable config across files
- ✅ Better for large projects

**Disadvantages:**
- ❌ Arduino IDE doesn't support this well
- ⚠️ PlatformIO/VSCode only


## Examples

See `example/` folder for both type of development environments.

## Requirements

- Raspberry Pi Pico (RP2040)
- Arduino-Pico core (Earle Philhower)
- [PicoSem library](https://github.com/IWILZ/PicoSem)

Special thanks to **Earle Philhower** for his great work!

## License

GPL-3.0

## Author

IWILZ (2026)
