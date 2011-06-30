#ifndef Initializer_H
#define Initializer_H

#include <string>
#include <typeinfo>

#include <boost/thread/condition.hpp>
#include "jackcpp/jackringbuffer.hpp"

#include "uncopyable.h"

class Logger;
class AudioProcessorThread;
class AudioCaptureThread;

class Initializer
{
    public:
        static Initializer* getInstance();

    private:
        JackCpp::RingBuffer<float>* audio_buffer_;

        AudioCaptureThread* audio_capture_;
        AudioProcessorThread* audio_processor_;
        boost::condition c1;

        Logger* const logger_;
        const std::string my_name_;

        DISALLOW_COPY_AND_ASSIGN(Initializer);
        Initializer();  // ctor private for Singleton implementation
        ~Initializer(); // same for dtor
        int init();
};

#endif // Initializer_H
