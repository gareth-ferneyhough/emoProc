#include "audio_capture.h"

#include <typeinfo>
#include <assert.h>
#include <string>
#include <iostream>

#include <jackringbuffer.hpp>
#include "logger.h"
#include "settings_mgr.h"

AudioCapture::AudioCapture(JackCpp::RingBuffer<float>* rb,
                           boost::condition* cond) :

  JackCpp::AudioIO("jackcpp-test", 1, 0),
  logger_(Logger::getInstance()),
  my_name_(typeid(*this).name()),
  sample_rate_(-1),
  ring_buffer_(rb),
  go_condition_(cond)
{
  assert (ring_buffer_ != NULL);
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
  for(unsigned int i = 0; i < inBufs.size(); i++){
    for(unsigned int j = 0; j < nframes; j++)
      ring_buffer_->write(inBufs[i][j]);
  }
  //std::cout << ring_buffer_->getReadSpace() << std::endl;
  //std::cout << getSampleRate() << std::endl;

  if (ring_buffer_->getReadSpace() >= 1024)
    go_condition_->notify_one();

  return 0;
}

int AudioCapture::startAudioClient()
{
  start();
  connectFromPhysical(0, 0);  // Connect input to port 0

  SettingsMgr::getInstance()->setSampleRate(getSampleRate());

  if (isRealTime())
    logger_->writeLog(my_name_, "Jack is real time");
  else
    logger_->writeLog(my_name_, "Jack is not real time");

  return 0;
}

