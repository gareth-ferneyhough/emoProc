#ifndef Initializer_H
#define Initializer_H

#include <string>
#include <typeinfo>

#include <boost/thread/condition.hpp>
#include <jackringbuffer.hpp>

#include "uncopyable.h"

class Logger;
class AudioProcessorThread;
class AudioCaptureThread;
class Classifier;

class Initializer
{
 public:
  static Initializer* getInstance();

 private:
  JackCpp::RingBuffer<float>* audio_buffer_in_;
  JackCpp::RingBuffer<float>* audio_buffer_out_;

  AudioCaptureThread* audio_capture_;
  AudioProcessorThread* audio_processor_;
  boost::condition c1;

  Classifier* classifier_;

  Logger* const logger_;
  const std::string my_name_;

  DISALLOW_COPY_AND_ASSIGN(Initializer);
  Initializer();  // ctor private for Singleton implementation
  ~Initializer(); // same for dtor
  int init();
};

#endif // Initializer_H
