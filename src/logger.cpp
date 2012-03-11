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

int Logger::logRawAudioD(const double* const audio_frames, int num_frames)
{
  int current_size = raw_audio_data_.size();
  raw_audio_data_.resize(current_size + num_frames);
  copy(audio_frames, audio_frames + num_frames, raw_audio_data_.begin() + current_size);

  return 0;
}

int Logger::logPitchData(float pitch)
{
  pitch_data_.push_back(pitch);
  return 0;
}

int Logger::logSpeechSegmentationData(bool speech_detected)
{
  speech_segmentation_data_.push_back(speech_detected);
  return 0;
}

int Logger::saveAllDataToFile()
{
  std::ofstream outfile1;
  
  // save raw audio
  outfile1.open("../data/raw.dat");
  
  std::vector<float>::iterator it;
  
  for(it = raw_audio_data_.begin(); it != raw_audio_data_.end(); it++){
    outfile1 << *it << std::endl;
  }

  outfile1.close();
  outfile1.clear();

  // save pitch
  outfile1.open("../data/pitch.dat");
  
  for(it = pitch_data_.begin(); it != pitch_data_.end(); it++){
    outfile1 << *it << std::endl;
  }

  outfile1.close();
  outfile1.clear();

  // save speech segments
  outfile1.open("../data/segment.dat");
  
  std::vector<int>::iterator it2;
  
  for(it2 = speech_segmentation_data_.begin(); it2 != speech_segmentation_data_.end(); it2++){
    outfile1 << *it2 << std::endl;
  }

  outfile1.close();
  outfile1.clear();

  return 0;
}

  
