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

  int getSampleRate()
  {
    return settings.sample_rate_;
  }

  void setSampleRate(int sample_rate)
  {
    settings.sample_rate_ = sample_rate;
  }

  int getPitchWindowSize()
  {
    return settings.pitch_window_size_;
  }

  void setPitchWindowSize(int pitch_window_size)
  {
    settings.pitch_window_size_ = pitch_window_size;
  }

  int getPitchWindowOverlap()
  {
    return settings.pitch_window_overlap_;
  }

  void setSampleRate(int pitch_window_overlap)
  {
    settings.pitch_window_overlap_ = pitch_window_overlap;
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
      pitch_window_overlap_(-1)
      {}

    int sample_rate_;          /* sample rate in Hz    */
    int pitch_window_size_;    /* window size in ms    */
    int pitch_window_overlap_; /* window overlap in ms */
  };

  Settings settings;
};

#endif // SETTINGS_H
