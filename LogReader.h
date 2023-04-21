#pragma once
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

struct accelerometerReading
{

	// struct designed to operate and save values from each read

	int TimeStamp; // [ms]
	// reading from aceleation portion on x,y and z axis
	double Gx; // [g]
	double Gy; // [g]
	double Gz; // [g]

	// steering wheel angles
	double theta;
	double phi;

	double senseBaseValue = 2048;
	// registers values 00; 01; 10;
	enum scale		{	g2,	g4,	g8	};

	// multiplier from the acceleration reading value
	double sensitivity;
	
	void refreshSensitivity(int g_mode)
	{
		// checks MMA8451Q configuration register value
		// and updates the sensitivity used to collect
		// the sample

		switch (g_mode)
		{
		case (g2):
			sensitivity = senseBaseValue * 2;
		case (g4):
			sensitivity = senseBaseValue;
		case (g8):
			sensitivity = senseBaseValue / 2;
		}
	}

	void solveAngles(){
		// solve both angles values
		solveTheta();
		solvePhi();
	}

	void solveTheta() {
		// pitch angle -180deg <= theta <= +180deg
		
		// initialize variables
		double solution1DEG;
		double solution2DEG;
		double solution1RAD;
		double solution2RAD;
		double quadrant;

		bool sol1OK = false;
		bool sol2OK = false;

		// calculate solutions

		solution1RAD = atan( - Gx / (sqrt(pow(Gy, 2) + pow(Gz, 2)))); // rad
		solution2RAD = atan( - Gx / Gz); // rad

		solution1DEG = solution1RAD * 180 / M_PI; // degrees
		solution2DEG = solution2RAD * 180 / M_PI; // degrees

		// check conditions

		if (-180 <= solution1DEG <= 180)
		{
			sol1OK = true;
		}
		if (-180 <= solution2DEG <= 180)
		{
			sol2OK = true;
		}

		// choose final theta value

		// both solution in the determined range
		if (sol1OK && sol2OK) {
			quadrant = atan2(Gx, Gz); // the solution is at the same quadrant from this result

			if (quadrant / abs(quadrant) > 0) // means that is the positive solution
			{
				if (solution1DEG >= 0)
				{
					theta = solution1DEG;
				}
				else
				{
					theta = solution2DEG;
				}
			}
			else // negative solution
			{
				if (solution1DEG <= 0)
				{
					theta = solution1DEG;
				}
				else
				{
					theta = solution2DEG;
				}
			}
		}
		// only first solution in the defined range
		else if (sol1OK)
			theta = solution1DEG;
		// only second solution in the defined range
		else if (sol2OK)
			theta = solution2DEG;
		else // no solutions available - instability zone
			theta = INFINITY;

	}

	void solvePhi() {
		// roll angle -90deg <= phi <= +90deg

		// initialize variables
		double solution1DEG;
		double solution2DEG;
		double solution1RAD;
		double solution2RAD;
		double quadrant;

		bool sol1OK = false;
		bool sol2OK = false;

		// calculate solutions

		solution1RAD = atan(Gy / (sqrt(pow(Gx, 2) + pow(Gz, 2)))); // rad
		solution2RAD = atan(Gy / Gz); // rad

		solution1DEG = solution1RAD * 180 / M_PI; // degrees
		solution2DEG = solution2RAD * 180 / M_PI; // degrees

		// check conditions

		if (-90 <= solution1DEG <= 90)
		{
			sol1OK = true;
		}
		if (-90 <= solution2DEG <= 90)
		{
			sol2OK = true;
		}

		// choose final theta value
		
		// both solution in the determined range
		if (sol1OK && sol2OK) {
			quadrant = atan2(Gy, Gz); // the solution is at the same quadrant from this result

			if (quadrant/ abs(quadrant) > 0) // means that is the positive solution
			{

				if (solution1DEG >= 0)
				{
					phi = solution1DEG;
				}
				else
				{
					phi = solution2DEG;
				}
			}
			else // negative root is the correct solution
			{
				if (solution1DEG <= 0)
				{
					phi = solution1DEG;
				}
				else
				{
					phi = solution2DEG;
				}
			}
		}
		// only first solution in the defined range
		else if (sol1OK)
			phi = solution1DEG;
		// only second solution in the defined range
		else if (sol2OK)
			phi = solution2DEG;
		else // no solutions available - instability zone
			phi = INFINITY;

	}
};

class LogReader
{
private:

	// attributes

	// declarations
	char separator; // char from the log file that separates information
	std::string path; // path to the log file
	std::string outputPath; // name for the output file

	std::ifstream openFile(); // opens log file
	std::string autoPath(); // originally designed created to designed output path
							// now returns direct programmed string
	std::string outputLineValue(accelerometerReading reading);	// struct that contains
																// values from the reading info
																// as well as functions to manipulate
																// this values
public:
	void setPath(std::string input); // set readings log path
	void setOuputPath(std::string output); // sets output path
	void setSeparator(char sep); // sets the separator for the log file to be read
	accelerometerReading translateLine(std::string line, int g_mode);	// translates each line into the correspondent 
																		// gx,gy,gz and timestamp
	// different constructors possibilities
	LogReader(std::string p, std::string outputpath);
	LogReader(std::string p, std::string outputpath, char sep);

	// routine to read all data, translate and save in the output file
	void readAndTranslate(int g_mode);
};

