#include "my_feature_extractor.h"
#include <iostream>
#include "pitch.h"
#include "filter.h"
#include "settings_mgr.h"

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  pitch(NULL),
  filter(NULL)
{
  //ctor
  init();
}

void MyFeatureExtractor::init()
{
  int sample_rate = SettingsMgr::getInstance()->getSampleRate();
  pitch = new Pitch(sample_rate);
  filter = new Filter(sample_rate);
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete pitch;
  delete filter;
}

void MyFeatureExtractor::processAudioSampleFunction(float* const audio_frames, int num_frames)
{
  filter->doLowpassFilter(audio_frames, num_frames);
  int the_pitch = pitch->getPitch(audio_frames, num_frames); 

  if(the_pitch >= 87 && the_pitch <= 320){
    std::cout << the_pitch << std::endl;
  }

  else std::cout << "0" << std::endl;
}
