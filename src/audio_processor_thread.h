#ifndef AUDIOPROCESSORTHREAD_H
#define AUDIOPROCESSORTHREAD_H

#include <jackringbuffer.hpp>
#include "thread.h"

class AudioProcessor;

class AudioProcessorThread : public ThreadWrapper
{
 public:
  explicit AudioProcessorThread(JackCpp::RingBuffer<float>*,
                                boost::condition*);
  ~AudioProcessorThread();

 private:
  AudioProcessor* audio_processor_;
  bool audio_processor_started_;
  JackCpp::RingBuffer<float>* audio_buffer_;

  boost::mutex mutex_;
  boost::condition* go_condition_;

  void runFunction();
};

#endif // AUDIOPROCESSORTHREAD_H
