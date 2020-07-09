#include "lcc-cpp-script.h"

#include <stdio.h>
#include <fstream>
#include <iostream>

FILE* fp = nullptr;

void StartLCCExternally(std::string filePathExec,Parameters param)
{
	std::string command_one = filePathExec + " " + param.inputDeviceStr + " " + param.outputDeviceStr + " "
							  + param.sampleRateStr + " " + param.inputgainStr + " " + 
							  param.centergainStr + " " + param.endgainStr + " " + 
							  param.decaygainStr + " " + param.delay_usStr;
	
	std::cout << command_one << std::endl;
	
	
	fp = popen(command_one.c_str(),"r");
	
	if(fp == nullptr)
	{
		std::cout << "Error. popen failed!\n";
		return;
	}
}

void EndLCCExternally()
{
	if(fp)
	{
		pclose(fp);
	}
}

void SafelyQuitLCC(std::string filePathDataDir)
{
	SetChoiceToQuitProgram(filePathDataDir);
	
	MakeLCCTakeInNewInput(filePathDataDir);
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

std::string GetOutputSTR(std::string filePathDataDir)
{
	std::string outString = "";
	
	std::ifstream output_msg_read_file;
	std::string output_msg_fp = filePathDataDir + "/output-message.txt";
	output_msg_read_file.open (output_msg_fp.c_str(), std::ifstream::in);
	if(output_msg_read_file.is_open())
	{
		std::string line;
		while (std::getline(output_msg_read_file, line)) 
		{
			outString.append(line);
		}
	}
	else
	{
		outString = "Failed to read output-message.txt";
	}
	
	return outString;
}
