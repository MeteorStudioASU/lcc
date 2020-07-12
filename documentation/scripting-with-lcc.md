# Genneral Scripting with LCC

To work with lcc in any scripting language, just call lcc with 8 inputs and edit text files rw-param-status.txt,
choice.txt, and param.txt

The workflow for using lcc from any script or GUI interface is that the lcc program
when executed with 8 inputs executes a special function called setupAudioStreamsNoConsoleQuery.

The 8 inputs are {inputdevice_num} {outputdevice_num} {sample_rate} {inputgain} {centergain} {endgain} {LCC_Decaygain} {LCC_Delay_in_microseconds}

setupAudioStreamsNoConsoleQuery sets up the audio stream with the 8 inputs and is in a loop
that reads rw-param-status.txt.

If rw-param-status.txt indicates to start taking in new inputs,
then the next file read is choice.txt.

choice.txt will state which choice to make such as turning on lcc, turning off lcc, change settings, quit lcc.

Based on the choice read from choice.txt, the program will execute appropriate functions.

If choice.txt indicates that settings should be changed, then param.txt will be read for new settings
such as input device number, output device number, sample rate, input gain, center gain, end gain, decay gain, delay.

At the end of reading input, the rw-param-status.txt will be reset to 0 to stop reading inputs.

## rw-param-status.txt

This file determines if program should take in new inputs.

0 means don't read for new input. This is best used for when writing inputs to file param.txt.
1 means start reading for new input. This is best used for when innputs in file param.txt are finished for writing.

## choice.txt

This file determines which choice the program should make.

1 means turn on lcc.
2 means turn off lcc.
3 means change settings of lcc.
4 means quit lcc

## param.txt

There are 8 inputs that are set in a new line.

The 8 inputs are input device number, output device number, sample rate, input gain, center gain, end gain, decay gain, delay.
Order matters.

