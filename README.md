# LCC - An open-source version of our spatial audio algorithm for binaural playback over closely-spaced stereo loudspeakers.
Developed by Robert LiKamWa (@roblkw_asu) and Matthew Lane (@mattlane66), with guidance from Ralph Glasgal (https://www.ambiophonics.org/). 

LCC (Localization Cue Correction) is a solution for spatialized audio through stereo speakers.
LCC is a lightweight implementation of crosstalk cancellation so your left ear hears the left channel of audio, and your right ear hears the right channel of audio (LCC functions like a physical barrier positioned between left and right ears and speakers to purify stereo imaging by drastically eliminating comb filtering, center channel bass doubling, and crosstalk). These are undesirable anomalies which are found in nearly all loudspeaker playback environments.

LCC works with symmetric head placement with respect to the speakers (an isosceles triangle configuration, not an equilateral triangle arrangement). That is, the speakers should be placed symmetrically in front of the listener, which greatly reduces head shadow effects. 
Please read our white papers found in the [link](https://medium.com/@mattlane66/spatial-audio-for-speaker-connected-devices-cdf96d127f67) under the [Theory](https://github.com/MeteorStudioASU/lcc/blob/master/README.md#theory) section of this page for more detailed information and configuration suggestions to get the best possible results.

We are fine-tuning a solution for crosstalk cancellation with arbitrary tracked head pose. Contact us for more details if you're interested.


## Setup (macOS)

1. Install virtual audio device you can route audio inputs and outputs (e.g., https://github.com/mattingalls/Soundflower/releases/ found in the assists sections).

2. In your Apple System Preferences for Sound, set your desired speakers to play at a comfortably loud volume. You can further adjust the system volume later with your virtual audio device / Soundflower.

3. In the Apple System Preferences, select the virtual audio device (Soundflower) as your default audio output device. 

## Setup (Windows)

1. Install virtual audio device you can route audio inputs and outputs (e.g. https://www.vb-audio.com/Cable/index.htm).

2. Select your virtual audio device / `CABLE Input` as your playback device.

## Setup (Linux)


### Using pulseaudio


1. Make a virtual audio device , called a sink, with pacmd

	a. Get name of default sink
	
	` $ pacmd list sinks | grep "Default sink" `
	
	` Default sink name: alsa_output.pci-xxxx_xx_1b.x.analog-stereo` 
	       
	b. Use output of sink name, i.e alsa_output.pci-xxxx_xx_1b.x.analog-stereo, to make a new sink
	
	` $ pacmd load-module module-combine-sink sink_name="Game" slaves="alsa_output.pci-xxxx_xx_1b.x.analog-stereo"` 
	       
	c. Look for device description under the name Game of virtual audio device in sinks from output of pacmd list sinks. 
	
	` $ pacmd list sinks`
	
	You should see something like this which is what lcc will also show for querying input device.
	
	` device.description = "Simultaneous output to X Analog Stereo"` 
	   
	   
2. After running lcc and connecting it to sink Game, ie. "Monitor Source of Simultaneous output to X Analog Stereo", as input and stereo speakers as output, open PulseAudioVolumeControl
	   ` $ pavucontrol` 
	   
3. Check that the RtAudio, is connected to stereo speaker output in Playback tab and RtAudio is connected to "Simultaneous output to X Analog Stereo" in Recording tab.

4. The audio stream of the program in playback tab should be set to the device description "Simultaneous output to X Analog Stereo" in order to route audio input to the sink Game which is connected to the speakers.
 
## Running LCC

1. Double-click on `lcc` on macOS or `lcc.exe` on Windows to run it. You may have to right-click it and open it to grant permissions to run the application.

2. When lcc asks you to `Select input device:`, type the numerical input value for your new audio device and press enter.
    * If you're using Soundflower, this will read `ingalls for Cycling ’74: Soundflower (2ch)`.
    * If you're using VB Cable, this will read `CABLE Output (VB-Audio Virtual Cable)`.
    * If you are using pulseaudio, this will read `Monitor Source of Simultaneous output to X Audio Controller Digital Stereo`.

3. When lcc asks you to `Select output device:`, type the numerical value for the desired speakers. (For example, `Apple Inc.: Built-in Output` for your MacBook's internal speakers.)

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

## Compiling code (Linux, Unix)
To compile the code on a linux or unix distribution, open a terminal, and run:
```
mkdir build
cd build
cmake ..
make
sudo make install

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

We ask that if you use this software for anything that you please attribute to authors Robert LiKamWa (@roblkw_asu) and Matthew Lane (@mattlane66) at [L&L Innovation Labs](https://l2il.com). 
