#include <iostream>
#include "LogReader.h"
#include <string>

int main()
{
    std::string path = "logs/attitude_exam.log";
    LogReader log = LogReader(path);
    log.readAndTranslate();
    
}