#pragma once
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

struct accelerometerReading
{
	double TimeStamp; // double for the case where math operations are required
	// reading from aceleation portion on x,y and z axis
	double Gx; 
	double Gy;
	double Gz;

	// steering wheel angles
	double theta;
	double phi;

	double senseBaseValue = 2048;
	// registers values 00; 01; 10;
	enum scale		{	g2,	g4,	g8	};

	double sensitivity;
	
	void refreshSensitivity(int g_mode)
	{
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

		bool sol1OK = false;
		bool sol2OK = false;

		// calculate solutions

		solution1RAD = -Gx / (sqrt(pow(Gy, 2) + pow(Gz, 2))); // rad
		solution2RAD = -Gx / Gz; // rad

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

		if (sol1OK and sol2OK)
			theta = atan2(solution1RAD, solution2RAD) * 180 / M_PI;
		else if (sol1OK)
			theta = solution1DEG;
		else if (sol2OK)
			theta = solution2DEG;
		else // no solutions available - instability zone
			theta = INFINITY;

	}

	void solvePhi() {
		// roll angle -90deg <= phi <= +90deg
	}
};

class LogReader
{
private:
	char separator;
	std::string path;
	void saveLine(std::string timestamp,accelerometerReading reading);
	std::ifstream openFile();
public:
	void setPath(std::string input);
	void setSeparator(char sep);
	accelerometerReading translateLine(std::string line, int g_mode);
	LogReader(std::string p);
	LogReader(std::string p, char sep);
	void readAndTranslate(int g_mode);
};

