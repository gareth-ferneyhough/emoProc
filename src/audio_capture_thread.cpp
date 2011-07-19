#include "audio_capture.h"

#include <iostream>
#include <assert.h>
#include "audio_capture_thread.h"

AudioCaptureThread::AudioCaptureThread(JackCpp::RingBuffer<float>* in,
				       JackCpp::RingBuffer<float>* out,
                                       boost::condition* cond) :
  ThreadWrapper(),
  audio_capture_(NULL),
  audio_capture_started_(false),
  audio_buffer_in_(in),
  audio_buffer_out_(out),
  go_condition_(cond)
{
  //ctor
  assert (audio_buffer_in_ != NULL);
  assert (audio_buffer_out_ != NULL);
  assert (go_condition_ != NULL);
}

AudioCaptureThread::~AudioCaptureThread()
{
  //dtor
  stopThread();
  delete audio_capture_;
}

int AudioCaptureThread::getSampleRate()
{
  return audio_capture_->getSampleRate();
}

void AudioCaptureThread::runFunction()
{
  assert(audio_buffer_in_ != NULL);
  assert(audio_buffer_out_ != NULL);

  if (!audio_capture_started_){
    audio_capture_ = new AudioCapture(audio_buffer_in_, audio_buffer_out_, go_condition_);
    audio_capture_->startAudioClient();
    audio_capture_started_ = true;
  }

  sleep(1);
}
