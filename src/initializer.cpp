#include "initializer.h"

#include <iostream>

#include <jackringbuffer.hpp>

#include "logger.h"
#include "audio_capture_thread.h"
#include "audio_processor_thread.h"
#include "settings_mgr.h"

Initializer::Initializer() :
  audio_buffer_in_(NULL),
  audio_buffer_out_(NULL),
  audio_capture_(NULL),
  audio_processor_(NULL),
  logger_(Logger::getInstance()),
  my_name_(typeid(*this).name())

{
  //ctor
  init();
}

Initializer::~Initializer()
{
  //dtor
  delete audio_processor_;
  delete audio_capture_;
  delete audio_buffer_in_;
  delete audio_buffer_out_;

  logger_->writeLog(my_name_, "in Destructor");
}

Initializer* Initializer::getInstance()
{
  static Initializer instance;
  return &instance;
}

int Initializer::init()
{
  // set some settings
  SettingsMgr::getInstance()->setSpeechEnergyThreshold(0.0001);

  // audio ring buffers
  audio_buffer_in_ = new JackCpp::RingBuffer<float>(65536);
  audio_buffer_out_ = new JackCpp::RingBuffer<float>(65536);

  // spawn two threads
  audio_capture_ = new AudioCaptureThread(audio_buffer_in_, audio_buffer_out_, &c1);
  audio_processor_ = new AudioProcessorThread(audio_buffer_in_, audio_buffer_out_,  &c1);

  audio_capture_->startThread();
  audio_processor_->startThread();

  // return to main
  return 0;
}



