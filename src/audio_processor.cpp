#include "audio_processor.h"

#include <assert.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "logger.h"
#include "settings_mgr.h"
#include "my_feature_extractor.h"

AudioProcessor::AudioProcessor() :
  audio_buffer_in_(NULL),
  audio_buffer_out_(NULL),
  sample_rate_(-1),
  feature_extractor(NULL),
  logger_(Logger::getInstance()),
  my_name_(typeid(*this).name())

{
  //ctor
  logger_->writeLog(my_name_, "in ctor");
  init();
}

void AudioProcessor::init()
{
  int rate = -1;

  do{
    // should sleep here, but works fine without it
    rate = SettingsMgr::getInstance()->getSampleRate();
  }
  while(rate <= 0);

  sample_rate_ = rate;
  std::cout << "sample rate: " << sample_rate_ << std::endl;

  feature_extractor = new MyFeatureExtractor();
}

AudioProcessor::~AudioProcessor()
{
  //dtor
  logger_->writeLog(my_name_, "in Destructor");
  delete feature_extractor;
}

void AudioProcessor::processAudio()
{
  // assert (audio_buffer_ != NULL);

  feature_extractor->processAudioSample(audio_buffer_in_);

  /* save output after filtering */
  // audio_buffer_out_->write(f, size);
}

