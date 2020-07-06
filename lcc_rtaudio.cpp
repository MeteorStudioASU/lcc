#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define BUFFER_LEN 4096

static int samplerate = 44100;
static int buflen = 8;
static double in_LR [BUFFER_LEN];
static double out_LR [BUFFER_LEN];
static double prevOut_LR [BUFFER_LEN];
static double delay_us = 32;
static double delay = 3.7;

static double inputgain = 0.70; //-3 dB
static double decaygain = 0.74989420933; //-2.5dB
static double centergain = 0.70; //-3 dB
static double endgain = 1.4285; //+3 dB

static bool lcc_toggle = true;
static double delaymod;
static double delaymodinv;

static double dBconv(double db){
  return pow(10,db/20);
}

static void lcc(){
  for (int i=0; i<buflen; i += 2) {
    in_LR[i]   = in_LR[i]   * inputgain;
    in_LR[i+1] = in_LR[i+1] * inputgain;
    
		out_LR[i]   = in_LR[i]   - decaygain * prevOut_LR[i+1]; //Left channel
		out_LR[i+1] = in_LR[i+1] - decaygain * prevOut_LR[i]; //Right channel

		if (i==0) {
			prevOut_LR[buflen-2] += out_LR[0]*delaymod;
			prevOut_LR[buflen-1] += out_LR[1]*delaymod;
		}
	}	
	
	for (int i=0; i<buflen; i += 2){
		if (i<buflen-2) {
			prevOut_LR[i]   = out_LR[i]*delaymodinv  + out_LR[i+2]*delaymod;
			prevOut_LR[i+1] = out_LR[i+1]*delaymodinv + out_LR[i+3]*delaymod;
		} else {
			prevOut_LR[i]   = out_LR[i]*delaymodinv;
			prevOut_LR[i+1] = out_LR[i+1]*delaymodinv;
		}
		out_LR[i] = endgain*(out_LR[i]     + in_LR[i]*centergain*inputgain/2 +
     in_LR[i+1]*centergain*inputgain/2);
		out_LR[i+1] = endgain*(out_LR[i+1] + in_LR[i]*centergain*inputgain/2 +
     in_LR[i+1]*centergain*inputgain/2);
	}
}

int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
 double streamTime, RtAudioStreamStatus status, void *data ) {
  if ( status ) std::cout << "Stream over/underflow" << std::endl;
  float * in_RT_LR = (float *)inputBuffer;
  float * out_RT_LR = (float *)outputBuffer;
  
  if (lcc_toggle){
    for (unsigned int i=0;i<nBufferFrames*2;i+=buflen){
      for (int j=0;j<buflen;j++){
        in_LR[j]=in_RT_LR[i+j];
      }

      lcc();

      for (int j=0;j<buflen;j++){
        out_RT_LR[i+j]=out_LR[j];
      }

    }
    
  } else {
    // If LCC is not activated, simply copy input -> output.
   memcpy( outputBuffer, inputBuffer, sizeof(float)*nBufferFrames*2 );
 }
 return 0;
} 

int setupAudioStreams(){
  RtAudio audio;
  RtAudio::DeviceInfo info;
  if ( audio.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices.\n";
    exit( 0 );
  }
  for (unsigned int i=0;i<audio.getDeviceCount();i++){
  	info = audio.getDeviceInfo( i );
  	std::cout << i << ":" << info.name << ", rates:";
  	for(size_t i = 0; i < info.sampleRates.size(); ++i)
   {
    if(i != 0) {
     std::cout << ",";
   }
   std::cout << info.sampleRates[i];
 }
 std::cout <<"\n";
}

int inputDevice, outputDevice;
std::cout <<"Select Input Device: ";
std::cin >> inputDevice;
std::cout <<"Select Output Device: ";
std::cin >> outputDevice;
std::cout <<"Select Sample Rate: ";
std::cin >> samplerate;
RtAudio::StreamParameters iParams, oParams;

delay=delay_us*1e-6*samplerate;
buflen=ceil(delay)*2;
delaymod = buflen/2-delay;
delaymodinv= 1-delaymod;

printf("input gain: %f\n", inputgain);
printf("center channel gain: %f\n", centergain);
printf("LCC decay gain: %f\n", decaygain);
printf("LCC delay in us: %f\n", delay_us);
printf("LCC delay in samples: %f\n", delay);


  iParams.deviceId = inputDevice; // first available device
  iParams.nChannels = 2;
  oParams.deviceId = outputDevice; // first available device
  oParams.nChannels = 2;
  unsigned int bufferFrames=(BUFFER_LEN/buflen)*buflen/2;
  printf("Debug only: bufferframes: %i\n", bufferFrames);

  try {
    audio.openStream( &oParams, &iParams, RTAUDIO_FLOAT32, samplerate, &bufferFrames, &inout, NULL);
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
  try {
    audio.startStream();
    int inputchoice;
    while(1){
     if (lcc_toggle){
      std::cout << "\nLCC ON... press 1 to turn LCC on, 2 to turn LCC off, 3 to change settings, 4 to quit.\n";
    }else{
      std::cout << "\nLCC OFF... press 1 to turn LCC on, 2 to turn LCC off, 3 to change settings, 4 to quit.\n";
    }
    std::cin >> inputchoice;
    if (inputchoice == 4){
        // Stop the stream.
      audio.stopStream();
      goto cleanup;
      
    } else if (inputchoice == 3){
    	std::cout<<"Enter new settings\n";
    	printf("{inputgain} {centergain} {endgain} {LCC_Decaygain} {LCC_Delay_in_microseconds} \n");
    	printf("Example: -3 -3 3 -2.5 32 \n");
      std::cin.ignore();
      std::string s;
      getline(std::cin, s);
      std::stringstream is(s);
      double n;
      double args[5]; 
      int ii=0;
      while( is >> n ) {
        args[ii]=n;
        ii++;
        if (ii == 5){
         break;
       }
     }
     
     if (ii==5){
      inputgain=dBconv(args[0]);
      centergain=dBconv(args[1]);
      endgain=dBconv(args[2]);
      decaygain=dBconv(args[3]);
      delay_us=args[4];
      
      delay=delay_us*1e-6*samplerate;
      buflen=ceil(delay)*2;
      delaymod = buflen/2-delay;
      delaymodinv= 1-delaymod;
      
      for (int i=0; i<buflen; i ++){
        prevOut_LR[i]=0;
      }
      
    } else{
      printf("Invalid input. \n");
    }
    
  } else if (inputchoice==2){
    lcc_toggle=false;
  } else{
    lcc_toggle=true;
  }
  
}
}
catch ( RtAudioError& e ) {
  e.printMessage();
  goto cleanup;
}
cleanup:
if ( audio.isStreamOpen() ) audio.closeStream();
return 0;
}

int main ( int argc, char *argv[] )
{	
	printf("./lcc_audio {inputgain} {centergain} {endgain} {LCC_Decaygain} {LCC_Delay_in_microseconds} \n");
  printf("version 0.8\n");

  if( argc != 6 ) {
   printf("Did not receive 6 arguments. Using defaults: ./lcc_audio -3 -3 3 -2.5 32 \n");
 }else{
   inputgain=dBconv(strtod(argv[1], NULL));
   centergain=dBconv(strtod(argv[2], NULL));
   endgain=dBconv(strtod(argv[3], NULL));
   decaygain=dBconv(strtod(argv[4], NULL));
   		delay_us=strtod(argv[5], NULL);//12;   
    }

    for (int i=0; i<buflen; i ++){
      prevOut_LR[i]=0;
    }

    setupAudioStreams();

    return 0 ;
} /* main */
