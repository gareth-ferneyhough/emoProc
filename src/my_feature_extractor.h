#ifndef MYFEATUREEXTRACTOR_H
#define MYFEATUREEXTRACTOR_H

#include "feature_extractor.h"

class Pitch;
class Filter;
class SpeechEnergy;
class Features;

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
  float previous_pitch_;

  float max_silence_;
  float length_silence_;
  bool new_utterance_;
  int sample_rate_;

  float* audio_frames_to_process_;
  JackCpp::RingBuffer<float>* audio_buffer_in_;

  Pitch* pitch_;
  Filter* filter_;
  SpeechEnergy* speech_energy_;
  Features* features_;
  
  void init();
  void processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames);
  void processSilence(int num_frames);
  void processSpeechSegment(float* audio_frames, int num_frames);
};

#endif // MYFEATUREEXTRACTOR_H
