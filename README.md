# LCC - An open source version of our spatial audio algorithm for binaural playback over closely-spaced stereo loudspeakers.
Developed by Robert LiKamWa (@roblkw_asu) and Matthew Lane (@mattlane66), with guidance from Ralph Glasgal (https://www.ambiophonics.org/). 

LCC (Localization Cue Correction) is a solution for spatialized audio through stereo speakers.
LCC is a lightweight implementation of crosstalk cancellation so your left ear hears the left channel of audio, and your right ear hears the right channel of audio.
This is our open source version of LCC.

LCC works with symmetric head placement with respect to the speakers (that is, the speakers should be placed symmetrically in front of the user). We are in the midst of fine-tuning a solution for crosstalk cancellation with arbitrary tracked head pose. Contact us for more details if you're interested.

These instructions are for macOS or OS X.

## Setup (macOS)

1. Install virtual audio device you can route audio inputs and outputs (e.g., https://github.com/mattingalls/Soundflower/releases/ found in the assists sections).

2. In your Apple System Preferences for Sound, set your desired speakers to play at a comfortably loud volume. You can further adjust the system volume later with your virtual audio device / Soundflower.

3. In the Apple System Preferences, select the virtual audio device (Soundflower) as your default audio output device. 

## Setup (Windows)

1. Install virtual audio device you can route audio inputs and outputs (e.g. https://www.vb-audio.com/Cable/index.htm).

2. Select your virtual audio device / `CABLE Input` as your playback device.

## Running LCC

1. Double-click on `lcc` on macOS or `lcc.exe` on Windows to run it. You may have to right-click it and open it in order to grant permissions to run the application.

2. When lcc asks you to `Select input device:`, type the numerical input value for your new audio device and press enter.
    * If you're using Soundflower, this will read `ingalls for Cycling ’74: Soundflower (2ch)`.
    * If you're using VB Cable, this will read `CABLE Output (VB-Audio Virtual Cable)`.

3. When lcc asks you to `Select output device:`, type the numerical value for the desired speakers. (For example, `Apple Inc.: Built-in Output` for your Macbook's internal speakers.)

4. Set your preferred sample rate, e.g., 44100.

5. Press 1 and then enter to turn on lcc. Press 2 and then enter to turn off lcc.

6. See below for advanced parameter tuning.

## Compiling code (macOS)
To compile the code on a Mac, navigate to the directory, open the Terminal, and run:
```
g++ -lpthread -framework CoreAudio -Wall -D__MACOSX_CORE__ -framework CoreFoundation lcc_rtaudio.cpp RtAudio.cpp  -lm -o lcc;
```

## Compiling code (Windows, MinGW)
You need to have MinGW installed:
```
choco install mingw
```
To compile the code on a Windows, navigate to the directory, open the PowerShell, and run:
```
g++ -Wall -D__WINDOWS_WASAPI__ .\lcc_rtaudio.cpp .\RtAudio.cpp -static-libstdc++ -static-libgcc -lole32 -loleaut32 -lmfplat -lmfuuid -lwmcodecdspuuid -lksuser -lm -static -o ./lcc.exe
```

## Advanced parameter tuning
(Coming soon)

## Theory
https://medium.com/@mattlane66/spatial-audio-for-speaker-connected-devices-cdf96d127f67?source=friends_link&sk=6c8494ea1eec58bd1458e156a8fa6418 

## Forum
https://news.ycombinator.com/item?id=23649673

## Attribution
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

We ask that if you use this software for anything that you please attribute to authors Robert LiKamWa (@roblkw_asu) and Matthew Lane (@mattlane66) at L&L Innovation Labs. 
