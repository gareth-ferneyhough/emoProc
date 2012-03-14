#include "audio_processor_thread.h"

#include <assert.h>
#include "audio_processor.h"

AudioProcessorThread::AudioProcessorThread(JackCpp::RingBuffer<float>* in,
                                           JackCpp::RingBuffer<float>* out,
                                           boost::condition* cond) :
  ThreadWrapper (),
  audio_processor_ (NULL),
  audio_processor_started_(false),
  audio_buffer_in_(in),
  audio_buffer_out_(out),
  go_condition_(cond)
{
  //ctor
  assert (audio_buffer_in_ != NULL);
  assert (audio_buffer_out_ != NULL);
  assert (go_condition_ != NULL);
}

AudioProcessorThread::~AudioProcessorThread()
{
  //dtor
  stopThread();
  delete audio_processor_;
}

void AudioProcessorThread::runFunction()
{
  assert(audio_buffer_in_ != NULL);
  assert(audio_buffer_out_ != NULL);

  if (!audio_processor_started_){
    audio_processor_ = new AudioProcessor();
    audio_processor_->setAudioBuffers(audio_buffer_in_, audio_buffer_out_);
    audio_processor_started_ = true;
  }

  // {
  //   boost::mutex::scoped_lock lock(mutex_);
  //   go_condition_->wait(lock);
  // }

  audio_processor_->processAudio();
}

