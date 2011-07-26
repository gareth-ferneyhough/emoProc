#include <iostream>
#include <string>

#include "initializer.h"
#include "logger.h"

int main()
{
  // these two classes are singleton implementations,
  // so call them to instanciate. Initializer will spawn
  // two threads to do audio capture and processing, then return
  // to main.
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
