#include "logger.h"

#include <fstream>
#include <iostream>
#include <iterator>

Logger::Logger()
{
  //ctor
  fout.open("log.log");
}

Logger::~Logger()
{
  //dtor
  writeLog("logger", "in Destructor");

  saveAllDataToFile();

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

int Logger::logRawAudio(const float* const audio_frames, int num_frames)
{
  int current_size = raw_audio_data_.size();
  raw_audio_data_.resize(current_size + num_frames);
  copy(audio_frames, audio_frames + num_frames, raw_audio_data_.begin() + current_size);

  return 0;
}

int Logger::saveAllDataToFile()
{
  std::ofstream outfile1;
  outfile1.open("raw.dat");
  
  std::vector<float>::iterator it;
  
  for(it = raw_audio_data_.begin(); it != raw_audio_data_.end(); it++){
    outfile1 << *it << std::endl;
  }

  outfile1.close();

  return 0;
}
  
