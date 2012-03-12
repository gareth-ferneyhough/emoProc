#ifndef AudioCapture_H
#define AudioCapture_H

#include <boost/thread/condition.hpp>
#include "jackcpp/jackringbuffer.hpp"
#include "uncopyable.h"

class Logger;

class AudioCapture{

 public:
  explicit AudioCapture(JackCpp::RingBuffer<float>* in,
			JackCpp::RingBuffer<float>* out,
                        boost::condition*);
  ~AudioCapture();

  virtual int audioCallback(float* inBuf, int nframes);
  int startAudioClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioCapture);

  Logger* const logger_;
  const std::string my_name_;

  JackCpp::RingBuffer<float>* ring_buffer_in_;
  JackCpp::RingBuffer<float>* ring_buffer_out_;

  boost::condition* go_condition_;

  // Read file stuff
  void readFile(const char *filename, float **audio_frames, int *sample_length, int *sample_rate);
};

#endif // AudioCapture_H
