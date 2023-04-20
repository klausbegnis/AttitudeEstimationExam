#include "LogReader.h"
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
				lineValue.TimeStamp = std::stod(currentValue);
		
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

	accelerometerReading currentReading;

	if (file.is_open()) // if the file is opened
	{
		int counter = 1;
		while (file) // while is open -> false when not reachable
		{
			std::getline(file, currentLine); // get line value

			if (currentLine.length() != 0) // checks if file not over
			{
				currentReading = translateLine(currentLine, g_mode); // save at readings struct
				currentReading.solveAngles(); // initialize theta and phi values
				print(counter);
				print(' ');
				print(currentReading.theta);
				print(currentReading.phi);
				counter++;
			}		
		}
	}
}

// initializers

LogReader::LogReader(std::string p)
{
	setPath(p);
	setSeparator(char(59));
}

LogReader::LogReader(std::string p, char sep)
{
	setPath(p);
	setSeparator(sep);
}