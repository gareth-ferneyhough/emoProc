#ifndef MYFEATUREEXTRACTOR_H
#define MYFEATUREEXTRACTOR_H

#include "feature_extractor.h"

class Pitch;

class MyFeatureExtractor : public FeatureExtractor
{
 public:
  MyFeatureExtractor();
  ~MyFeatureExtractor();

 private:
  Pitch* pitch;

  void init();
  void processAudioSampleFunction(const float* audio_frames, int num_frames);
};

#endif // MYFEATUREEXTRACTOR_H
