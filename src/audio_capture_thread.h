#ifndef AUDIOCAPTURETHREAD_H
#define AUDIOCAPTURETHREAD_H

#include "thread.h"

class AudioCapture;

class AudioCaptureThread : public ThreadWrapper
{
    public:
        explicit AudioCaptureThread(JackCpp::RingBuffer<float>*,
                                    boost::condition*);
        ~AudioCaptureThread();

        int getSampleRate();

        bool isStarted() {
            return audio_capture_started_;
        }

    private:
        AudioCapture* audio_capture_;
        bool audio_capture_started_;
        JackCpp::RingBuffer<float>* audio_buffer_;

        boost::condition* go_condition_;

        void runFunction();
};

#endif // AUDIOCAPTURETHREAD_H

