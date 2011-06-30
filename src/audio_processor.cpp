#include "audio_processor.h"

#include <assert.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "logger.h"
#include "settings_mgr.h"
#include "my_feature_extractor.h"

AudioProcessor::AudioProcessor() :
    audio_buffer_(NULL),
    sample_rate_(-1),
    feature_extractor(NULL),
    logger_(Logger::getInstance()),
    my_name_(typeid(*this).name())

{
    //ctor
    logger_->writeLog(my_name_, "in ctor");
    init();
}

void AudioProcessor::init()
{
    int rate = -1;

    do{
        // should sleep here, but works fine without it
        rate = SettingsMgr::getInstance()->getSampleRate();
    }
    while(rate <= 0);

    sample_rate_ = rate;
    std::cout << "sample rate: " << sample_rate_ << std::endl;

    feature_extractor = new MyFeatureExtractor();
}

AudioProcessor::~AudioProcessor()
{
    //dtor
    logger_->writeLog(my_name_, "in Destructor");
}

void AudioProcessor::processAudio()
{
    assert (audio_buffer_ != NULL);

    int size = audio_buffer_->getReadSpace();
    //if (size >= 2056){
        float* f = new float[size];

        for (int i = 0; i < size; i++)
            audio_buffer_->read(f, size);

        feature_extractor->processAudioSample(f, size);
    //}
}

