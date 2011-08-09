#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "filter.h"
#include "speech_energy.h"
#include "settings_mgr.h"
#include "features.h"

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  window_size_(-1),
  window_overlap_(-1),
  window_size_frames_(-1),
  window_overlap_frames_(-1),
  speech_energy_threshold_(-1),
  audio_frames_to_process_(NULL),
  audio_buffer_in_(NULL),
  pitch_(NULL),
  filter_(NULL),
  speech_energy_(NULL),
  features_(NULL)
{
  //ctor
  init();
}

void MyFeatureExtractor::init()
{
  int sample_rate = SettingsMgr::getInstance()->getSampleRate();
  window_size_ = SettingsMgr::getInstance()->getPitchWindowSize();
  window_overlap_ = SettingsMgr::getInstance()->getPitchWindowOverlap();

  window_size_frames_ = 1024; //(int) (float)window_size_ / (float)1000 * (float)sample_rate;
  window_overlap_frames_ = 512; //(int) (float)window_overlap_ / (float)sample_rate;

  audio_frames_to_process_ = new float[window_size_frames_];

  pitch_ = new Pitch(window_size_frames_, sample_rate);
  filter_ = new Filter(sample_rate);
  speech_energy_ = new SpeechEnergy();
  speech_energy_threshold_ = SettingsMgr::getInstance()->getSpeechEnergyThreshold();
  features_ = new Features();
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete[] audio_frames_to_process_;
  delete pitch_;
  delete filter_;
  delete speech_energy_;
  delete features_;
}

void MyFeatureExtractor::processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames)
{
  // read audio frames from ringbuffer, get speech energy, and process speech segments.
  while(audio_frames->getReadSpace() >= window_size_frames_){
    audio_frames->read(audio_frames_to_process_, window_size_frames_);
    logger_->logRawAudio(audio_frames_to_process_, window_size_frames_);

    float speech_energy = speech_energy_->getSpeechEnergy(audio_frames_to_process_, window_size_frames_);

    // speech detected
    if(speech_energy >= speech_energy_threshold_){
      processSpeechSegment(audio_frames_to_process_, window_size_frames_);

    }

    // no speech detected
    else{
      logger_->logSpeechSegmentationData(false);
      logger_->logPitchData(0.0);
      features_->pushFeatures();
    }
  }
}

void MyFeatureExtractor::processSpeechSegment(float* audio_frames, int num_frames)
{
  logger_->logSpeechSegmentationData(true);

  filter_->doLowpassFilter(audio_frames, num_frames);
  int the_pitch = pitch_->getPitch(audio_frames, num_frames);

  if (!(the_pitch >= 87 && the_pitch <= 320)){
    the_pitch = 0;
  }

  //std::cout << the_pitch << std::endl;
  logger_->logPitchData(the_pitch);
  features_->savePitch(the_pitch);
  features_->saveRaw(audio_frames, num_frames);
}
