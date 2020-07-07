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
#include <fstream>      // std::ifstream

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

//DATADIR macro used by automake for path to data files
//if DATADIR macro is not defined, define it as data
#ifndef DATADIR
    #define DATADIR "../data"
#endif

#define DATADIR_NAME STR(DATADIR)

std::string DATADIR_STR = DATADIR_NAME;
std::string param_status_fp = DATADIR_STR + "/rw-param-status.txt";
std::string choice_fp = DATADIR_STR + "/choice.txt";
std::string param_fp = DATADIR_STR + "/param.txt";
std::string output_msg_fp = DATADIR_STR + "/output-message.txt";

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


int setupAudioStreamsNoConsoleQuery(int inputDevice, int outputDevice, int samplerate ){
  RtAudio audio;
  RtAudio::DeviceInfo info;
  
  RtAudio::StreamParameters iParams, oParams;
	
    

	delay=delay_us*1e-6*samplerate;//(sfinfo.samplerate);
	buflen=ceil(delay)*2;
	delaymod = buflen/2-delay;
	delaymodinv= 1-delaymod;



	printf("input gain: %f\n", inputgain);
	printf("center channel gain: %f\n", centergain);
	printf("LCC decay gain: %f\n", decaygain);
	printf("LCC delay in us: %f\n", delay_us);
	printf("LCC delay in samples: %f\n", delay);
	printf("Debug only: delaymod: %f\n", delaymod);
	printf("Debug only: delaymodinv: %f\n", delaymodinv);

  

  iParams.deviceId = inputDevice; // first available device
  iParams.nChannels = 2;
  oParams.deviceId = outputDevice; // first available device
  oParams.nChannels = 2;
  unsigned int bufferFrames=(BUFFER_LEN/buflen)*buflen/2;
  	printf("Debug only: bufferframes: %i\n", bufferFrames);
  	
  //open output message
	std::ofstream output_msg_file;
	output_msg_file.open (output_msg_fp.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(output_msg_file.is_open())
	{
		std::cout << "Failed to open output message!\n";
	}

try {
    audio.openStream( &oParams, &iParams, RTAUDIO_FLOAT32, samplerate, &bufferFrames, &inout, NULL);
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
try {
    audio.startStream();
    
    std::string line;
    
    bool quit = false;
    
    
    while(!quit){
		
		//open rw-param-status file and read it
		 
		 std::ifstream rw_param_stat_file (param_status_fp.c_str(), std::ifstream::in);
		 
		 bool readInput = false;
		 
		 if (rw_param_stat_file.is_open())
		 {
			while ( std::getline (rw_param_stat_file,line) )
			{
			  if(line == "1")
			  {
				  output_msg_file << "reading input..\n";
				  readInput = true;
			  }
			 
			}
			rw_param_stat_file.close();
			
		 }
		
		//if rw-param-status indicates that parameters can be read from i.e. 1
		if(readInput)
		{
			line.clear();
			
			//read input parameters file
			
			std::ifstream choice_file (choice_fp.c_str(), std::ifstream::in);
			 
			int inputchoice;
			 
			if (choice_file.is_open())
			{
				while ( std::getline (choice_file,line) )
				{
				    inputchoice = std::stoi(line);
				    output_msg_file << "input chosen is " << inputchoice << std::endl;
				}
				
				choice_file.close();
			}
			
			if (lcc_toggle){
				output_msg_file << "\nLCC ON... press 1 to turn LCC on, 2 to turn LCC off, 3 to change settings, 4 to quit.\n";
			}else{
				output_msg_file << "\nLCC OFF... press 1 to turn LCC on, 2 to turn LCC off, 3 to change settings, 4 to quit.\n";
			}
			
			if (inputchoice == 4){
			
				quit = true;
			} else if (inputchoice == 3){
				std::cout<<"Enter new settings\n";
				printf("{inputgain} {centergain} {endgain} {LCC_Decaygain} {LCC_Delay_in_microseconds} \n");
				
				std::vector <std::string> inputParamStrVec;
				
				//read parameters from param.txt
				std::ifstream param_file (param_fp.c_str(), std::ifstream::in);
				 
				if (param_file.is_open()) {
					while ( std::getline (param_file,line) )
					{
						inputParamStrVec.push_back(line);
						
					}
					
					param_file.close();
				}
					
				if(inputParamStrVec.size() == 8) {
					int inputDevice = strtod(inputParamStrVec[0].c_str(),NULL);
					int outputDevice = strtod(inputParamStrVec[1].c_str(),NULL);
					int samplerate = strtod(inputParamStrVec[2].c_str(),NULL);
					inputgain=dBconv(strtod(inputParamStrVec[3].c_str(), NULL));
					centergain=dBconv(strtod(inputParamStrVec[4].c_str(), NULL));
					endgain=dBconv(strtod(inputParamStrVec[5].c_str(), NULL));
					decaygain=dBconv(strtod(inputParamStrVec[6].c_str(), NULL));
					delay_us=strtod(inputParamStrVec[7].c_str(), NULL);
					
					for (int i=0; i<buflen; i ++){
							prevOut_LR[i]=0;
					}
					
				}
				else {
					output_msg_file << "Invalid input!\n";
				}
				
				
			} else if (inputchoice==2){
				lcc_toggle=false;
			} else{
				lcc_toggle=true;
			}
			
			//put back to state of not reading input
			std::ofstream rw_param_stat_out;
			rw_param_stat_out.open (param_status_fp.c_str(), std::ofstream::out | std::ofstream::trunc);
			if(rw_param_stat_out.is_open())
			{
				rw_param_stat_out << "0";
			}
				
		}
		
	}
	
	output_msg_file.close();
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
	printf("./lcc_audio {inputdevice_num} {outputdevice_num} {sample_rate} {inputgain} {centergain} {endgain} {LCC_Decaygain} {LCC_Delay_in_microseconds} \n");
    printf("version 0.8\n");
    
    if( argc == 9) {
		
		int inputDevice = strtod(argv[1],NULL);
		int outputDevice = strtod(argv[2],NULL);
		int samplerate = strtod(argv[3],NULL);
		inputgain=dBconv(strtod(argv[4], NULL));
		centergain=dBconv(strtod(argv[5], NULL));
		endgain=dBconv(strtod(argv[6], NULL));
		decaygain=dBconv(strtod(argv[7], NULL));
		delay_us=strtod(argv[8], NULL);
		
		for (int i=0; i<buflen; i ++){
			prevOut_LR[i]=0;
		}
		
		setupAudioStreamsNoConsoleQuery(inputDevice, outputDevice, samplerate);
	}
	else {
		
		if( argc != 6 ) {
      	printf("Did not receive 6 arguments. Using defaults: ./lcc -3 -3 3 -2.5 32 \n");
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
	}

    return 0 ;
} /* main */
