#ifndef LCC_CPP_SCRIPT_H
#define LCC_CPP_SCRIPT_H

#include <fstream>
#include <string>

struct Parameters
{
	std::string inputDeviceStr;
	std::string outputDeviceStr;
	std::string sampleRateStr;
	std::string inputgainStr;
	std::string centergainStr;
	std::string endgainStr;
	std::string decaygainStr;
	std::string delay_usStr;
};

//function to start LCC program
void StartLCCExternally(std::string filePathExec,Parameters param);

//funtion to write new parameters in param.txt
void ChangeParameterValues(std::string filePathDataDir, Parameters param);

//function to indicate to lcc script interface to take in new input
void MakeLCCTakeInNewInput(std::string filePathDataDir);

//function to set choice.txt to indicate a change in settings
void SetChoiceToChangeSettings(std::string filePathDataDir);

//function to set choice.txt to indicate choice to quit program
void SetChoiceToQuitProgram(std::string filePathDataDir);

//function to set choice.txt to indicate choice to turn on lcc program
void SetChoiceToTurnOnLCC(std::string filePathDataDir);

//function to set choice.txt to indicate choice to turn off lcc program
void SetChoiceToTurnOffLCC(std::string filePathDataDir);

#endif
