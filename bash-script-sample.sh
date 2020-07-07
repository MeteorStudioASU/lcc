#!/usr/bin/env bash

#start lcc program

#run this script after starting an instance of lcc_audio executable in build folder
#also run this script while an instance of lcc_audio is running

#change settings

INPUT_DEVICE=5
OUTPUT_DEVICE=2
SAMPLE_RATE=44000
INPUT_GAIN=-2
CENTER_GAIN=-1
END_GAIN=2
DECAY_GAIN=-2.0
DELAY_US=44

#erase contents of param.txt and set a new paramenter on a new line

nline=$'\n'
params="$INPUT_DEVICE$nline$OUTPUT_DEVICE$nline$SAMPLE_RATE$nline$INPUT_GAIN$nline$CENTER_GAIN$nline$END_GAIN$nline$DECAY_GAIN$nline$DELAY_US"
export params
perl -e ' open DATA, "+>./data/param.txt" or die "Could not open file ./data/param.txt, $!"; say DATA $ENV{params}; ';

#erase contents of choice.txt and indicate change settings i.e. 3
perl -e ' open DATA, "+>./data/choice.txt" or die "Could not open file ./data/choice.txt, $!"; say DATA "3"; ';

#erase contents of rw-param-status.txt and indicate action to take in new input
perl -e ' open DATA, "+>./data/rw-param-status.txt" or die "Could not open file /data/rw-param-status.txt, $!"; say DATA "1"; ';

cat ./data/output-message.txt
