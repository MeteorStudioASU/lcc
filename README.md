# lcc

LCC is a localized cue correction solution for spatialized audio. 

These instructions are for Mac OS X.

## Setup

1. Acquire virtual audio device you can route audio inputs and outputs (e.g., https://github.com/mattingalls/Soundflower/releases/ found in the assists sections).

2. In your Apple system preferences for Sound, set your desired speakers to play at a comfortably loud volume. You can further adjust the system volume later with your virtual audio device / Soundflower.

3. In the Apple system preferences, select the virtual audio device (Soundflower) as your default audio output device. 

## Running lcc 

1. Double-click on `lcc_app` to run it. You may have to right-click it and open it in order to grant permissions to run the application.

2. When lcc_app asks you to `Select input device:`, type the numerical input value for your new audio device and press enter. If you're using Soundflower, this will read `ingalls for Cycling ’74: Soundflower (2ch)`)

3. When lcc_app asks you to `Select output device:`, type the numerical value for the desired speakers. (For example, `Apple Inc.: Built-in Output` for your Macbook's internal speakers.)

4. Set your preferred sample rate, e.g., 44100.

5. Press 1 to turn on LCC. Press 2 to turn off LCC.

6. See below for advanced parameter tuning.

## Compiling code
To compile the code on a Mac, navigate to the directory, open the Terminal, and run:
```
g++ -lpthread -framework CoreAudio -Wall -D__MACOSX_CORE__ -framework CoreFoundation lcc_buffer_rtaudio.cpp RtAudio.cpp  -lm -o lcc;
```

## Advanced parameter tuning
(Coming soon)
