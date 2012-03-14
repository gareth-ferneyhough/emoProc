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

  // Wait till done
  while (logger->isProcessingDone() == false){
    sleep (1);
  }

  return 0;
}
