#ifndef SPEECH_ENERGY_H
#define SPEECH_ENERGY_H

class SpeechEnergy
{
 public:
  SpeechEnergy();
  ~SpeechEnergy();
  float getSpeechEnergy(const float* const, int);
};

#endif // SPEECH_ENERGY_H
