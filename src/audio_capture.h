#ifndef AudioCapture_H
#define AudioCapture_H

#include <boost/thread/condition.hpp>
#include <jackaudioio.hpp>
#include "uncopyable.h"

class Logger;

class AudioCapture: public JackCpp::AudioIO{

 public:
  explicit AudioCapture(JackCpp::RingBuffer<float>* rb,
                        boost::condition*);
  ~AudioCapture();

  virtual int audioCallback(jack_nframes_t nframes,
                            audioBufVector inBufs,
                            audioBufVector outBufs);

  int startAudioClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioCapture);

  Logger* const logger_;
  const std::string my_name_;

  jack_nframes_t sample_rate_;
  float audio_frame_ [1024];

  JackCpp::RingBuffer<float>* ring_buffer_;

  boost::condition* go_condition_;
};

#endif // AudioCapture_H
