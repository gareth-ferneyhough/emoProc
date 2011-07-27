#include "speech_energy.h"
#include <iostream>

SpeechEnergy::SpeechEnergy()
{
}

SpeechEnergy::~SpeechEnergy()
{
}

/* calculate speech energy */
float SpeechEnergy::getSpeechEnergy(const float* const audio_frames, int num_frames)
{
  float sum_of_squares = 0;
  
  for(int i = 0; i < num_frames; i++){
    sum_of_squares += audio_frames[i] * audio_frames[i];
  }

  float speech_energy = sum_of_squares / (float)num_frames;

  //std::cout << speech_energy << std::endl;
  return speech_energy;
}
