#ifndef MYFEATUREEXTRACTOR_H
#define MYFEATUREEXTRACTOR_H

#include "feature_extractor.h"

class Pitch;
class Filter;
class SpeechEnergy;

class MyFeatureExtractor : public FeatureExtractor
{
 public:
  MyFeatureExtractor();
  ~MyFeatureExtractor();

 private:
  Pitch* pitch_;
  Filter* filter_;
  SpeechEnergy* speech_energy_;
  
  float speech_energy_threshold_;

  void init();
  void processAudioSampleFunction(float* audio_frames, int num_frames);
};

#endif // MYFEATUREEXTRACTOR_H
