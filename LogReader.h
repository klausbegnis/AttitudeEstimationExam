#pragma once
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

struct accelerometerReading
{
	int TimeStamp; // double for the case where math operations are required
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
		if (sol1OK and sol2OK) {
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
		if (sol1OK and sol2OK) {
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
			else
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
	char separator;
	std::string path;
	std::string outputPath;
	void saveLine(std::string timestamp,accelerometerReading reading);
	std::ifstream openFile();
	std::string autoPath();
	std::string outputLineValue(accelerometerReading reading);
public:
	void setPath(std::string input);
	void setOuputPath(std::string output);
	void setSeparator(char sep);
	accelerometerReading translateLine(std::string line, int g_mode);
	LogReader(std::string p);
	LogReader(std::string p, char sep);
	void readAndTranslate(int g_mode);
};

