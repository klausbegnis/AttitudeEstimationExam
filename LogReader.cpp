#include "LogReader.h"
#include <iostream>
#include <fstream> 
#include <stdexcept>

// print methods

template<typename T>
void print(T input) 
{
	// fast terminal print method
	// accepts every type
	// and fails when the type
	// doesn't allows std::cout method
	// use with caution
	std::cout << input << std::endl;
}

void printError(std::exception e)
{
	// prints excpetion received
	std::cerr << e.what() << std::endl;
}

// setters methods

void LogReader::setOuputPath(std::string output)
{
	// sets output path
	outputPath = output;
}

void LogReader::setPath(std::string input)
{
	// setter to classes path private attribute
	path = input;
}

void LogReader::setSeparator(char sep)
{
	// setter to classes separator private attribute
	separator = sep;
}

// file methods

std::ifstream LogReader::openFile()
{
	// open especified file
	// in case of failure 
	// the program stops
	// and error is thrown at
	// the command prompt

	std::ifstream file;
	try
	{
		// opens the log file
		file.open(path);
		return file;
	}
	catch (const std::exception& e)
	{
		// catch errors at opening file
		print("Error ocurred while opening file");
		return file;
	}
}

accelerometerReading LogReader::translateLine(std::string line, int g_mode)
{
	//input line -> line to be translated
	// g_mode -> setted g mode in accelerometer's register

	accelerometerReading lineValue{}; // return value initialization

	// set sensitivity value
	lineValue.refreshSensitivity(g_mode);

	std::string currentValue; // retrieved value from line
	// first value - > time stamp
	// second value - > gx
	// third value - > gy
	// fourth value - > gz
	char element;

	// usage for switch/case structure
	enum read { time_stamp,gx,gy,gz };

	// iterate over the line
	int read_Counting = 0;
	for (int i = 0; i < line.size(); i++)
	{
		// get line values
		element = line[i];

		// tests if one of the parameters has ended or if the file has ended
		if ((element == separator) || (i == line.size() - 1)) // value finished
		{
			i++; // jumps space an goes to next value

			// checks current value position
			switch (read_Counting) {

			case time_stamp: // at timestamp
				lineValue.TimeStamp = std::stoi(currentValue); // [ms]

			case gx: // at gx reading
				lineValue.Gx = std::stod(currentValue) / lineValue.sensitivity; // [g]

			case gy: // at gy reading
				lineValue.Gy = std::stod(currentValue) / lineValue.sensitivity; // [g]

			case gz: // at gz reading
				lineValue.Gz = std::stod(currentValue + element) / lineValue.sensitivity; // [g]
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
			currentValue += element;
		};
	};

	//print(lineValue.TimeStamp); print(lineValue.Gx); print(lineValue.Gy); print(lineValue.Gz);

	return lineValue;
}

void LogReader::readAndTranslate(int g_mode)
{
	// g_mode = 0,1 or 2 == mode setted 2g,4g or 8g


	std::ifstream file = openFile();// input file stream class
	std::string currentLine; // line to be read

	std::ofstream outputFile; // output file 
	outputFile.open(outputPath); // opens output file

	if (!outputFile.is_open()) // if output file not inserted
	{
		outputPath = autoPath(); // autogenerates outputpath
		outputFile.open(outputPath); // will overwrite existing files from previous readings
	}

	accelerometerReading currentReading; // initialize reading struct


	if (file.is_open() && outputFile.is_open()) // if the file is opened
	{
		// prints information to user

		print("Log file found, proceeding to generate values translation...\n");

		print("Translation in progress ...");

		while (file) // while is open -> false when not reachable
		{
			std::getline(file, currentLine); // get line value

			if (currentLine.length() != 0) // checks if file not over
			{
				currentReading = translateLine(currentLine, g_mode); // save at readings struct
				currentReading.solveAngles(); // initialize theta and phi values
				outputFile << outputLineValue(currentReading); // write the line in the output file
			}		
		}
		// removes translation in progress sentence
		printf("\33[A\r");
		printf("\33[2K\r");
		// inform that the code finished
		print("Translation completed ... [DONE]\nFile saved in: " + outputPath);
	}
	else
	{
		// if error occured while opening output file
		// input file errors are catched in  LogReader::OpenFile()
		print("It was not possible to open the specified file, check if it exists.");
	}
}

std::string LogReader::autoPath()
{
	// sets automatically the output path
	return "auto_out.log";
}

std::string LogReader::outputLineValue(accelerometerReading a)
{
	// declarates line variable
	std::string line;
	// creates the spacing string added between values
	std::string spacing = { char(59),char(32) };

	// add all values into one string
	line = std::to_string(a.TimeStamp) + spacing + std::to_string(a.theta) + spacing + std::to_string(a.phi) +"\n";

	// returns the string to be appended at the output file
	return line;
}


// initializers

LogReader::LogReader(std::string p, std::string outputpath)
{
	// this constructors accept a string that receives
	// the path from the logfile
	setPath(p); // sets logfile path
	setOuputPath(outputpath); // sets output file name
	setSeparator(char(59)); // sets logfile separator
}

LogReader::LogReader(std::string p, std::string outputpath, char sep)
{
	// this constructors accept a string that receives
	// the path from the logfile and the
	// desired character separator

	setPath(p);  // sets logfile path
	setOuputPath(outputpath); // sets output file name
	setSeparator(sep); // sets logfile separator
}