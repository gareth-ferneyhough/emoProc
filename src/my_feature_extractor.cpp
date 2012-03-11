#include "my_feature_extractor.h"

#include <iostream>
#include <sndfile.h>

#include <stdlib.h>

#include "pitch.h"
#include "settings_mgr.h"
#include "features.h"
#include "classifier.h"

MyFeatureExtractor::MyFeatureExtractor() :
  FeatureExtractor(),
  window_size_(-1),
  window_overlap_(-1),
  speech_energy_threshold_(-1),
  previous_pitch_(0),
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
  //sample_rate_ = SettingsMgr::getInstance()->getSampleRate();
  max_silence_ = SettingsMgr::getInstance()->getMaxSilenceBtwnUtterances();

  // window_size_ = 1024;
  //window_overlap_ = 512;

  testProcessFromFile();

  //pitch_ = new Pitch(sample_rate_, window_size_, window_overlap_);
  //features_ = new Features();
  //audio_frames_to_process_ = new float[window_size_];
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
  // // read audio frames from ringbuffer and process speech segments.
  // while(audio_frames->getReadSpace() >= window_size_){    
  //   std::cout << audio_frames->getReadSpace() << std::endl;

  //   audio_frames->peek(audio_frames_to_process_, window_size_);
  //   audio_frames->erase(window_overlap_);

  //   //logger_->logRawAudio(audio_frames_to_process_, window_size_);
    
  //   processSpeechSegment(audio_frames_to_process_, window_size_);
  //   // // speech detected
  //   // if(speech_energy >= speech_energy_threshold_){
  //   //   processSpeechSegment(audio_frames_to_process_, window_size);
  //   // }

  //   // // no speech detected
  //   // else{
  //   //   processSilence(window_size);
  //   // }
  // }
}

void MyFeatureExtractor::processSilence(int num_frames)
{
  logger_->logSpeechSegmentationData(false);
  logger_->logPitchData(0.0);
  features_->pushFeatures();

  length_silence_ += 1000.0 * (float)num_frames / (sample_rate_); // in ms
  //  std::cout << length_silence_ << std::endl;

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
  logger_->logSpeechSegmentationData(true);

  // this sucks //
  double* double_frames = new double[num_frames];
  for (int i = 0; i < num_frames; ++i){
    double_frames[i] = static_cast<double>(audio_frames[i]);
  }

  logger_->logRawAudioD(double_frames, num_frames);

  pitch_->getPitch(double_frames, num_frames);

  // std::cout << the_pitch << std::endl;
  // logger_->logPitchData(the_pitch);
  // features_->savePitch(the_pitch);
  // features_->saveRaw(audio_frames, num_frames);
  

  length_silence_ = 0;
  new_utterance_ = true;

  delete[] double_frames;
}

void MyFeatureExtractor::testProcessFromFile()
{
  double *audio_frames;
  int sample_rate;
  int sample_length;
  const char *filename = "in.wav";

  readFile(filename, &audio_frames, &sample_length, &sample_rate);
  int frames_remaining = sample_length;

  // init pitch
  int window_size;
  int window_stride;
  pitch_ = new Pitch(sample_rate, window_size, window_stride);
  
  int rt = 1;
  int index = 0;
  do{
    rt = pitch_->getPitch(&audio_frames[index], window_size);
    index += window_stride;
  }
  while (rt != 0);
}

// Read input file with libsndfile
void MyFeatureExtractor::readFile(const char *filename, double **audio_frames, int *sample_length, int *sample_rate)
{
  SNDFILE *infile;
  SF_INFO sfinfo;

  // check file
  if(!(infile = sf_open(filename, SFM_READ, &sfinfo)))
    {
      printf("Unable to open %s\n", filename);
      puts(sf_strerror(NULL));
      exit(-1);
    }

  if(sfinfo.channels > 1)
    {
      printf("Not able to process more than 1 channel.\n");
      exit(-1);
    }

  //read sound file
  *sample_length = sfinfo.frames;
  *sample_rate = sfinfo.samplerate;

  (*audio_frames) = (double*)malloc(*sample_length * sizeof(double));
  int readcount = sf_readf_double(infile, (*audio_frames), *sample_length);

  if (readcount != *sample_length){
    printf("Error reading sound file\n");
    exit(-1);
  }

  else printf("Read %d frames at %f Hz\n", *sample_length, *sample_rate);
}

