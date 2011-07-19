#ifndef MYFEATUREEXTRACTOR_H
#define MYFEATUREEXTRACTOR_H

#include "feature_extractor.h"

class Pitch;
class Filter;

class MyFeatureExtractor : public FeatureExtractor
{
 public:
  MyFeatureExtractor();
  ~MyFeatureExtractor();

 private:
  Pitch* pitch;
  Filter* filter;

  void init();
  void processAudioSampleFunction(float* audio_frames, int num_frames);
};

#endif // MYFEATUREEXTRACTOR_H
