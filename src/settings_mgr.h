#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <typeinfo>

#include "logger.h"
#include "uncopyable.h"

class SettingsMgr
{
 public:
  static SettingsMgr* getInstance();

  int getSampleRate() const
  {
    return settings.sample_rate_;
  }

  void setSampleRate(int sample_rate)
  {
    settings.sample_rate_ = sample_rate;
  }

  float getSpeechEnergyThreshold() const
  {
    return settings.speech_energy_threshold_;
  }

  void setSpeechEnergyThreshold(float threshold)
  {
    settings.speech_energy_threshold_ = threshold;
  }

 private:
  Logger* const logger_;
  const std::string my_name_;

  DISALLOW_COPY_AND_ASSIGN(SettingsMgr);
  SettingsMgr();  // ctor private for Singleton implementation
  ~SettingsMgr(); // same for dtor

  class Settings{
  public:
  Settings() :
    sample_rate_(-1),
    speech_energy_threshold_(-1)
    {}

    int sample_rate_;
    float speech_energy_threshold_;
  };

  Settings settings;
};

#endif // SETTINGS_H
