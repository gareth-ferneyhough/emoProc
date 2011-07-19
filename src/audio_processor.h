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

  void setAudioBuffers(JackCpp::RingBuffer<float>* audio_buffer_in,
                       JackCpp::RingBuffer<float>* audio_buffer_out)
  {
    audio_buffer_in_ = audio_buffer_in;
    audio_buffer_out_ = audio_buffer_out;
  }

  void processAudio();

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioProcessor);

  JackCpp::RingBuffer<float>* audio_buffer_in_;
  JackCpp::RingBuffer<float>* audio_buffer_out_;
  int sample_rate_;

  MyFeatureExtractor* feature_extractor;

  Logger* const logger_;
  const std::string my_name_;
};

#endif // AUDIOPROCESSOR_H
