#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

#include "uncopyable.h"

class Logger
{
    public:
        static Logger* getInstance();
        int writeLog(const std::string& object_name, const std::string& log_entry);
        int writeLog(const std::string& object_name, int log_entry);

    private:
        DISALLOW_COPY_AND_ASSIGN(Logger);
        Logger();  // ctor private for Singleton implementation
        ~Logger(); // same for dtor

        std::ofstream fout;
};

#endif // LOGGER_H
