#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "filter.h"
#include "speech_energy.h"
#include "settings_mgr.h"

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
  speech_energy_(NULL)
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
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete[] audio_frames_to_process_;
  delete pitch_;
  delete filter_;
  delete speech_energy_;
}

void MyFeatureExtractor::processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames)
{
  while(audio_frames->getReadSpace() >= window_size_frames_){
    audio_frames->read(audio_frames_to_process_, window_size_frames_);
    logger_->logRawAudio(audio_frames_to_process_, window_size_frames_);

    float speech_energy = speech_energy_->getSpeechEnergy(audio_frames_to_process_, window_size_frames_);

    if(speech_energy >= speech_energy_threshold_){
      logger_->logSpeechSegmentationData(true);

      filter_->doLowpassFilter(audio_frames_to_process_, window_size_frames_);
      int the_pitch = pitch_->getPitch(audio_frames_to_process_, window_size_frames_);

      if (!(the_pitch >= 87 && the_pitch <= 320)){
        the_pitch = 0;
      }

      std::cout << the_pitch << std::endl;
      logger_->logPitchData(the_pitch);
    }

    // no speech detected
    else{
      logger_->logSpeechSegmentationData(false);
      logger_->logPitchData(0.0);
    }
  }
}
