#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <string>
#include "logger.h"

class FeatureExtractor
{
    public:
        FeatureExtractor();
        virtual ~FeatureExtractor();

        void processAudioSample(float* audio_frames, int num_frames){
                processAudioSampleFunction(audio_frames, num_frames);
        }

    protected:
        Logger* const logger_;
        const std::string my_name_;

    private:
        virtual void processAudioSampleFunction(const float* audio_frames, int num_frames) = 0;
};

#endif // FEATUREEXTRACTOR_H
