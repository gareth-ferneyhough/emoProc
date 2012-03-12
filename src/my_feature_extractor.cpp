#include "my_feature_extractor.h"

#include <iostream>
#include <sndfile.h>

#include <stdlib.h>

#include "pitch.h"
#include "settings_mgr.h"
#include "features.h"
#include "classifier.h"

using std::cout;
using std::endl;

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  window_size_(-1),
  window_overlap_(-1),
  speech_energy_threshold_(-1),
  max_silence_(-1),
  length_silence_(0),
  new_utterance_(false),
  sample_rate_(-1),
  audio_frames_to_process_(NULL),
  audio_buffer_in_(NULL),
  pitch_(NULL),
  features_(NULL)
{
  //ctor
  init();
}

void MyFeatureExtractor::init()
{
  sample_rate_ = SettingsMgr::getInstance()->getSampleRate();
  max_silence_ = SettingsMgr::getInstance()->getMaxSilenceBtwnUtterances();

  pitch_ = new Pitch(sample_rate_, window_size_, window_overlap_);
  features_ = new Features();
  audio_frames_to_process_ = new float[window_size_];
}

MyFeatureExtractor::~MyFeatureExtractor()
{
  //dtor
  delete[] audio_frames_to_process_;
  delete pitch_;
  delete features_;
}

void MyFeatureExtractor::processAudioSampleFunction(JackCpp::RingBuffer<float>* const audio_frames)
{
  // read audio frames from ringbuffer and process speech segments.
  while(audio_frames->getReadSpace() >= window_size_){    
    audio_frames->peek(audio_frames_to_process_, window_size_);
    audio_frames->erase(window_overlap_);

    //logger_->logRawAudio(audio_frames_to_process_, window_size_);
    
     processSpeechSegment(audio_frames_to_process_, window_size_);
    // // speech detected
    // if(speech_energy >= speech_energy_threshold_){
    //   processSpeechSegment(audio_frames_to_process_, window_size);
    // }

    // // no speech detected
    // else{
    //   processSilence(window_size);
    // }
  }
}

void MyFeatureExtractor::processSilence(int num_frames)
{
  logger_->logSpeechSegmentationData(false);
  logger_->logPitchData(0.0);
  features_->pushFeatures();

  length_silence_ += 1000.0 * (float)num_frames / (sample_rate_); // in ms

  if (length_silence_ > max_silence_ && new_utterance_ == true){
    std::string utterance = features_->getLastUtteranceAsString(); 
    //    Classifier::getInstance()->scaleData(utterance);

    features_->startNewUtterance();
    length_silence_ = 0;
    new_utterance_ = false;
  }  
}

void MyFeatureExtractor::processSpeechSegment(float* audio_frames, int num_frames)
{
   // this sucks //
  double* double_frames = new double[num_frames];
  for (int i = 0; i < num_frames; ++i)
    double_frames[i] = static_cast<double>(audio_frames[i]);
  
  logger_->logSpeechSegmentationData(true);
  logger_->logRawAudioD(double_frames, num_frames);
  
  std::vector<float> pitches = pitch_->getPitch(double_frames, num_frames);
  for (int i = 0; i < pitches.size(); i++)
    logger_->logPitchData(pitches[i]);


  // features_->savePitch(the_pitch);
  // features_->saveRaw(audio_frames, num_frames);


  length_silence_ = 0;
  new_utterance_ = true;

  delete[] double_frames;
}
