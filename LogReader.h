#pragma once
#include <iostream>
#include <string>

struct accelerometerReading
{
	double TimeStamp; // double for the case where math operations are required
	// reading from aceleation portion on x,y and z axis
	double Gx; 
	double Gy;
	double Gz;
};

class LogReader
{
private:
	char separator = char(59);
	std::string path;
	void saveLine(std::string timestamp,accelerometerReading reading);
	std::ifstream openFile();
public:
	void setPath(std::string input);
	void setSeparator(char sep);
	accelerometerReading translateLine(std::string line);
	LogReader(std::string p);
	LogReader(std::string p, char sep);
	void readAndTranslate();
};

