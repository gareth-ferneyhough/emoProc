#include "logger.h"

#include <fstream>
#include <iostream>

Logger::Logger()
{
    //ctor
    fout.open("log.log");
}

Logger::~Logger()
{
    //dtor
    writeLog("logger", "in Destructor");
    fout.close();
    fout.clear();
}

Logger* Logger::getInstance()
{
    static Logger instance_;
    return &instance_;
}

int Logger::writeLog(const std::string& object_name, const std::string& logEntry)
{
    fout << object_name <<":" << " " << logEntry << std::endl;
    std::cout << object_name <<":" << " " << logEntry << std::endl;

    return 0;
}

int Logger::writeLog(const std::string& object_name, int logEntry)
{
    fout << object_name <<":" << " " << logEntry << std::endl;
    std::cout << object_name <<":" << " " << logEntry << std::endl;

    return 0;
}
