#include "LogReader.h"
#include <iostream>
#include <fstream> 
#include <stdexcept>

template<typename T>
void print(T input)
{
	std::cout << input << std::endl;
}

void printError(std::exception e)
{
	std::cerr << e.what() << std::endl;
}

std::ifstream LogReader::openFile()
{
	std::ifstream file;
	try
	{
		file.open(path); // opens log file
		return file;
	}
	catch (const std::exception& e)
	{
		print("Error ocurred while opening file");
	}
}


void LogReader::setPath(std::string input)
{
	path = input;
}

void LogReader::setSeparator(char sep)
{
	separator = sep;
}

void LogReader::saveLine(std::string timestamp, accelerometerReading reading)
{
}

accelerometerReading LogReader::translateLine(std::string line, int g_mode)
{
	accelerometerReading lineValue{}; // return value

	// set sensitivity value
	lineValue.refreshSensitivity(g_mode);

	std::string currentValue; // retrieved value from line
	// first value - > time stamp
	// second value - > gx
	// third value - > gy
	// fourth value - > gz

	// usage for switch/case structure
	enum read { time_stamp,gx,gy,gz };

	// iterate over the line
	int read_Counting = 0;
	for (int i = 0; i < line.size(); i++)
	{
		// get line values
		char element = line[i];
		
		if ((element == separator) or (i == line.size()-1)) // value finished
		{
			i++; // jumps space an goes to next value

			// checks current value position
			switch (read_Counting){

			case time_stamp: // at timestamp
				lineValue.TimeStamp = std::stoi(currentValue);
		
			case gx: // at gx reading
				lineValue.Gx = std::stod(currentValue) / lineValue.sensitivity;

			case gy: // at gy reading
				lineValue.Gy = std::stod(currentValue) / lineValue.sensitivity;

			case gz: // at gz reading
				lineValue.Gz = std::stod(currentValue+element) / lineValue.sensitivity; 
																// to prevent
																// list out of bounds issues
																// instead of making to else statement
																// just adds automatically last value
			}
			// steps to next value
			read_Counting++;
			// reset current value
			currentValue = "";
		}
		else 
		{
			// increment reading
			currentValue+=element;
		}
	}

	//print(lineValue.TimeStamp); print(lineValue.Gx); print(lineValue.Gy); print(lineValue.Gz);

	return lineValue;
}

void LogReader::readAndTranslate(int g_mode)
{
	std::ifstream file = openFile();// input file stream class
	std::string currentLine; // line to be read

	std::ofstream outputFile; // ouput file 
	outputFile.open(outputPath);

	accelerometerReading currentReading;

	if (file.is_open() and outputFile.is_open()) // if the file is opened
	{
		while (file) // while is open -> false when not reachable
		{
			std::getline(file, currentLine); // get line value

			if (currentLine.length() != 0) // checks if file not over
			{
				currentReading = translateLine(currentLine, g_mode); // save at readings struct
				currentReading.solveAngles(); // initialize theta and phi values
				outputFile << outputLineValue(currentReading);
			}		
		}
	}
	else
	{
		print("It was not possible to open the specified file, check if it exists.");
	}
}

std::string LogReader::autoPath()
{
	return "attitude_exam_out.log";
}


std::string LogReader::outputLineValue(accelerometerReading a)
{
	std::string line;
	std::string spacing = { char(59),char(32) };

	line = std::to_string(a.TimeStamp) + spacing + std::to_string(a.theta) + spacing + std::to_string(a.phi) +"\n";

	return line;
}

void LogReader::setOuputPath(std::string output)
{
	outputPath = output;
}

// initializers

LogReader::LogReader(std::string p)
{

	setPath(p);
	outputPath = autoPath();
	setOuputPath(outputPath);
	setSeparator(char(59));
}

LogReader::LogReader(std::string p, char sep)
{
	setPath(p);
	outputPath = autoPath();
	setOuputPath(outputPath);
	setSeparator(sep);
}