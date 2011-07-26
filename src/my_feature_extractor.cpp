#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "filter.h"
#include "speech_energy.h"
#include "settings_mgr.h"

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  pitch_(NULL),
  filter_(NULL),
  speech_energy_(NULL),
  speech_energy_threshold_(-1)
{
  //ctor
  init();
}

void MyFeatureExtractor::init()
{
  int sample_rate = SettingsMgr::getInstance()->getSampleRate();
  pitch_ = new Pitch(sample_rate);
  filter_ = new Filter(sample_rate);
  speech_energy_ = new SpeechEnergy();

  speech_energy_threshold_ = SettingsMgr::getInstance()->getSpeechEnergyThreshold();
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete pitch_;
  delete filter_;
  delete speech_energy_;
}

void MyFeatureExtractor::processAudioSampleFunction(float* const audio_frames, int num_frames)
{
  float speech_energy = speech_energy_->getSpeechEnergy(audio_frames, num_frames);
  logger_->logRawAudio(audio_frames, num_frames);

  if(speech_energy >= speech_energy_threshold_){

    filter_->doLowpassFilter(audio_frames, num_frames);
    int the_pitch = pitch_->getPitch(audio_frames, num_frames);

    if(the_pitch >= 87 && the_pitch <= 320){
      std::cout << the_pitch << std::endl;
    }

    else std::cout << 0 << std::endl;
  }
}
