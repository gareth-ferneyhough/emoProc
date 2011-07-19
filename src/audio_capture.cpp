#include "audio_capture.h"

#include <typeinfo>
#include <assert.h>
#include <string>
#include <iostream>

#include <jackringbuffer.hpp>
#include "logger.h"
#include "settings_mgr.h"

AudioCapture::AudioCapture(JackCpp::RingBuffer<float>* in,
                           JackCpp::RingBuffer<float>* out,
                           boost::condition* cond) :

  JackCpp::AudioIO("jackcpp-test", 1, 1), // mono only
  logger_(Logger::getInstance()),
  my_name_(typeid(*this).name()),
  sample_rate_(-1),
  ring_buffer_in_(in),
  ring_buffer_out_(out),
  go_condition_(cond)
{
  assert (ring_buffer_in_ != NULL);
  assert (ring_buffer_out_ != NULL);
  assert (go_condition_ != NULL);
}

AudioCapture::~AudioCapture()
{
  disconnectInPort(0);
  close();

  logger_->writeLog(my_name_, "in Destructor");
}

int AudioCapture::audioCallback(jack_nframes_t nframes,
                                audioBufVector inBufs,
                                audioBufVector outBufs)
{
  ring_buffer_in_->write(inBufs[0], nframes); // mono only

  int output_size = ring_buffer_out_->getReadSpace();
  ring_buffer_out_->read(outBufs[0], output_size);

  // if (ring_buffer_->getReadSpace() >= 1024)
  go_condition_->notify_one();

  return 0;
}

int AudioCapture::startAudioClient()
{
  start();
  connectToPhysical(0, 0);  // Connect output to port 0

  SettingsMgr::getInstance()->setSampleRate(getSampleRate());

  if (isRealTime())
    logger_->writeLog(my_name_, "Jack is real time");
  else
    logger_->writeLog(my_name_, "Jack is not real time");

  return 0;
}

