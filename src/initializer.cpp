#include "initializer.h"

#include <iostream>

#include "jackcpp/jackringbuffer.hpp"

#include "logger.h"
#include "audio_capture_thread.h"
#include "audio_processor_thread.h"

Initializer::Initializer() :
  audio_buffer_(NULL),
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
  delete audio_buffer_;

  logger_->writeLog(my_name_, "in Destructor");
}

Initializer* Initializer::getInstance()
{
  static Initializer instance;
  return &instance;
}

int Initializer::init()
{
  audio_buffer_ = new JackCpp::RingBuffer<float>(65536);

  audio_capture_ = new AudioCaptureThread(audio_buffer_, &c1);
  audio_processor_ = new AudioProcessorThread(audio_buffer_, &c1);

  audio_capture_->startThread();
  audio_processor_->startThread();

  return 0;
}



