#include <iostream>
#include <string>

#include "initializer.h"
#include "logger.h"

int main()
{
    Logger* logger = Logger::getInstance();
    Initializer::getInstance();

    logger->writeLog("main", "in main");

    // Exit program on 'q'
    char c;
    do{
        std::cin >> c;
    }

    while(c != 'q');

    return 0;
}
