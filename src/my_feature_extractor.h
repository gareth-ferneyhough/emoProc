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
  int window_size_;
  int window_overlap_;
  int window_size_frames_;
  int window_overlap_frames_;
  float speech_energy_threshold_;

  float* audio_frames_to_process_;
  JackCpp::RingBuffer<float>* audio_buffer_in_;

  Pitch* pitch_;
  Filter* filter_;
  SpeechEnergy* speech_energy_;

  void init();
  void processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames);
};

#endif // MYFEATUREEXTRACTOR_H
