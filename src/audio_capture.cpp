#include "audio_capture.h"

#include <iostream>
#include <typeinfo>
#include <string>
#include <assert.h>
#include <sndfile.h>

#include "jackcpp/jackringbuffer.hpp"
#include "logger.h"
#include "settings_mgr.h"

AudioCapture::AudioCapture(JackCpp::RingBuffer<float>* in,
                           JackCpp::RingBuffer<float>* out,
                           boost::condition* cond) :

  logger_(Logger::getInstance()),
  my_name_(typeid(*this).name()),
  ring_buffer_in_(in),
  ring_buffer_out_(out),
  go_condition_(cond)
{
  assert (ring_buffer_in_ != NULL);
  assert (ring_buffer_out_ != NULL);
  assert (go_condition_ != NULL);
}

AudioCapture::~AudioCapture()
{
  logger_->writeLog(my_name_, "in Destructor");
}

int AudioCapture::audioCallback(float* inBufs, int nframes)
{
  ring_buffer_in_->write(&inBufs[0], nframes);
  
  // for (int i = 0; i < nframes; i += 3)
  //   ring_buffer_in_->write(inBufs[0][i]);

  go_condition_->notify_one();

  return 0;
}

int AudioCapture::startAudioClient()
{
  float *audio_frames_f;
  int sample_length;
  const char *filename = "in.wav";
  int sample_rate;

  readFile(filename, &audio_frames_f, &sample_length, &sample_rate);
  SettingsMgr::getInstance()->setSampleRate(sample_rate);
  audioCallback(audio_frames_f, sample_length); 

  return 0;
}

// Read input file with libsndfile
void AudioCapture::readFile(const char *filename, float **audio_frames, int *sample_length, int *sample_rate)
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

  (*audio_frames) = (float*)malloc(*sample_length * sizeof(float));
  int readcount = sf_readf_float(infile, (*audio_frames), *sample_length);

  if (readcount != *sample_length){
    printf("Error reading sound file\n");
    exit(-1);
  }

  else printf("Read %d frames at %d Hz\n", *sample_length, *sample_rate);
}
