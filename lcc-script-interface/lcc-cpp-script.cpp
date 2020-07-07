#include "lcc-cpp-script.h"

#include <stdio.h>
#include <fstream>
#include <iostream>

void StartLCCExternally(std::string filePathExec,Parameters param)
{
	std::string command_one = filePathExec + " " + param.inputDeviceStr + " " + param.outputDeviceStr + " "
							  + param.sampleRateStr + " " + param.inputgainStr + " " + 
							  param.centergainStr + " " + param.endgainStr + " " + 
							  param.decaygainStr + " " + param.delay_usStr;
	
	std::cout << command_one << std::endl;
	
	char write = 'w';
	popen(command_one.c_str(),&write);
}

void ChangeParameterValues(std::string filePathDataDir, Parameters param)
{
	//changes values in param.txt
	std::ofstream param_out;
	std::string param_fpStr = filePathDataDir + "/param.txt";
	param_out.open(param_fpStr.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(param_out.is_open())
	{
		param_out << param.inputDeviceStr << "\n";
		param_out << param.outputDeviceStr << "\n";
		param_out << param.sampleRateStr << "\n";
		param_out << param.inputgainStr << "\n";
		param_out << param.centergainStr << "\n";
		param_out << param.endgainStr << "\n";
		param_out << param.decaygainStr << "\n";
		param_out << param.delay_usStr << "\n";
	}
}

void MakeLCCTakeInNewInput(std::string filePathDataDir)
{
	//set lcc program to start taking in new input through rw-param-stat.txt
	std::ofstream rw_param_stat_out;
	std::string param_status_fp = filePathDataDir + "/rw-param-status.txt";
	rw_param_stat_out.open (param_status_fp.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(rw_param_stat_out.is_open())
	{
		rw_param_stat_out << "1";
	}
}

static void SetChoice(std::string filePathDataDir,std::string choice)
{
	std::ofstream choice_out;
	std::string choice_fpStr = filePathDataDir + "/choice.txt";
	choice_out.open(choice_fpStr.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(choice_out.is_open())
	{
		choice_out << choice;
	}
}

void SetChoiceToChangeSettings(std::string filePathDataDir)
{
	std::string choice = "3";
	
	SetChoice(filePathDataDir,choice);
}

void SetChoiceToQuitProgram(std::string filePathDataDir)
{
	std::string choice = "4";
	
	SetChoice(filePathDataDir,choice);
}

void SetChoiceToTurnOnLCC(std::string filePathDataDir)
{
	std::string choice = "1";
	
	SetChoice(filePathDataDir,choice);
}

void SetChoiceToTurnOffLCC(std::string filePathDataDir)
{
	std::string choice = "2";
	
	SetChoice(filePathDataDir,choice);
}
