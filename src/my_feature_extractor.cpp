#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "filter.h"
#include "settings_mgr.h"

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  window_size_(-1),
  window_overlap_(-1),
  window_size_frames_(-1),
  window_overlap_frames_(-1),
  audio_frames_to_process_(NULL),
  audio_buffer_in_(NULL),
  pitch(NULL),
  filter(NULL)
{
  //ctor
  init();
}

void MyFeatureExtractor::init()
{
  int sample_rate = SettingsMgr::getInstance()->getSampleRate();
  window_size_ = SettingsMgr::getInstance()->getPitchWindowSize();
  window_overlap_ = SettingsMgr::getInstance()->getPitchWindowOverlap();

  window_size_frames_ = (int) (float)window_size_ / (float)1000 * (float)sample_rate;
  window_overlap_frames_ = (int) (float)window_overlap_ / (float)sample_rate;

  audio_frames_to_process_ = new float[window_size_frames_];

  pitch = new Pitch(window_size_frames_, sample_rate);
  filter = new Filter(sample_rate);
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete[] audio_frames_to_process_;
  delete pitch;
  delete filter;
}

void MyFeatureExtractor::processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames)
{
  while(audio_frames->getReadSpace() >= window_size_frames_){

    audio_frames->read(audio_frames_to_process_, window_size_frames_);

    filter->doLowpassFilter(audio_frames_to_process_, window_size_frames_);
    float the_pitch = pitch->getPitch(audio_frames_to_process_, window_size_frames_);

    if(the_pitch > 101.0 && the_pitch < 320.0){
      std::cout << the_pitch << std::endl;
    }
    else std::cout << "0" << std::endl;
  }
}
