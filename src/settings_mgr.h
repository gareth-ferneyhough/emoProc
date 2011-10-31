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

  int getPitchWindowSize() const
  {
    return settings.pitch_window_size_;
  }

  void setPitchWindowSize(int pitch_window_size)
  {
    settings.pitch_window_size_ = pitch_window_size;
  }

  int getPitchWindowOverlap() const
  {
    return settings.pitch_window_overlap_;
  }

  void setPitchWindowOverlap(int pitch_window_overlap)
  {
    settings.pitch_window_overlap_ = pitch_window_overlap;
  }

  float getMaxSilenceBtwnUtterances() const
  {
    return settings.max_silence_btwn_utterances_;
  }

  void setMaxSilenceBtwnUtterances(float max_silence)
  {
    settings.max_silence_btwn_utterances_ = max_silence;
  }

  std::string getModelFilename() const
  {
    return settings.model_filename_;
  }

  void setModelFilename(std::string filename)
  {
    settings.model_filename_ = filename;
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
    pitch_window_size_(-1),
    pitch_window_overlap_(-1),
    speech_energy_threshold_(-1),
    max_silence_btwn_utterances_(-1)
    {}

    int sample_rate_;          /* sample rate in Hz    */
    int pitch_window_size_;    /* window size in ms    */
    int pitch_window_overlap_; /* window overlap in ms */
    float speech_energy_threshold_;
    float max_silence_btwn_utterances_;
    std::string model_filename_;
  };

  Settings settings;
};

#endif // SETTINGS_H
