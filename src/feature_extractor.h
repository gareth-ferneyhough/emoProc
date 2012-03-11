#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <string>
#include "jackcpp/jackringbuffer.hpp"
#include "logger.h"


class FeatureExtractor
{
 public:
  FeatureExtractor();
  virtual ~FeatureExtractor();

  void processAudioSample(JackCpp::RingBuffer<float>* audio_frames){
    processAudioSampleFunction(audio_frames);
  }

 protected:
  Logger* const logger_;
  const std::string my_name_;

 private:
  virtual void processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames) = 0;
};

#endif // FEATUREEXTRACTOR_H
