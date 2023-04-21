#include <iostream>
#include "LogReader.h"
#include <string>


// function called at AttitudeEstimationExam.exe
int main(int argc,char* argv[])
{
    // declare argument strings
    std::string inputpath;
    std::string outputpath;
    
    // checks if user added output string
    if (argc > 2)
    {
        // passes the values
        inputpath = argv[1]; // errors are catched inside logreader
        outputpath = argv[2]; // if not adequate, logreader generates it's own
        bool typeInOutput = false; 
        
        // checks if output path has a file type
        for (int i = 0; i < outputpath.size(); i++)
        {
            // if type in outputpath
            if (outputpath[i] == '.')
                typeInOutput = true;
               
        }
        if (!typeInOutput)
        {
            // add log type
            outputpath += ".log";
        }
    }
    else if (argc > 1) // checks if the only input was the log file
    {
        inputpath = argv[1]; //  sets the input path
        outputpath = ""; // lets the logreader set the output path

    }
    else {
        // input log file path was not informed
        std::cout << "Input path must be informed" << std::endl;
        std::cout << "Example: file.log" << std::endl;
        return -1;
    }

    // builds the logreader
    LogReader log = LogReader(inputpath, outputpath);
    // 2 -> 10 -> register configurated at the accelerator mode: 8g
    log.readAndTranslate(2);
    return 1;
}