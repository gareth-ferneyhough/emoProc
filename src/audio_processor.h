#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <string>
#include <typeinfo>

#include <jackringbuffer.hpp>

#include "uncopyable.h"

class Logger;
class MyFeatureExtractor;

class AudioProcessor
{
 public:
  AudioProcessor();
  ~AudioProcessor();
  void init();

  void setAudioBuffer(JackCpp::RingBuffer<float>* audio_buffer)
  {
    audio_buffer_ = audio_buffer;
  }

  void processAudio();

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioProcessor);

  JackCpp::RingBuffer<float>* audio_buffer_;
  int sample_rate_;

  MyFeatureExtractor* feature_extractor;

  Logger* const logger_;
  const std::string my_name_;
};

#endif // AUDIOPROCESSOR_H
