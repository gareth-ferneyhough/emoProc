#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <vector>
#include "uncopyable.h"

class Logger
{
 public:
  static Logger* getInstance();
  int writeLog(const std::string& object_name, const std::string& log_entry);
  int writeLog(const std::string& object_name, int log_entry);
  int logRawAudio(const float* const audio_data, int num_frames);
  int logRawAudioD(const double* const audio_data, int num_frames);

  int logPitchData(float pitch);
  int logSpeechSegmentationData(bool speech_detected);
  int saveAllDataToFile();

  bool isProcessingDone();
  void setProcessingDone();

 private:
  DISALLOW_COPY_AND_ASSIGN(Logger);
  Logger();  // ctor private for Singleton implementation
  ~Logger(); // same for dtor

  std::ofstream fout;
  std::vector<float> raw_audio_data_;
  std::vector<float> pitch_data_;
  std::vector<int> speech_segmentation_data_;

  bool processing_done;
};

#endif // LOGGER_H
