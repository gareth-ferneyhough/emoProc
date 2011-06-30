#ifndef THREAD_H
#define THREAD_H

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>

class ThreadWrapper
{
    public:
        ThreadWrapper();
        virtual ~ThreadWrapper();
        void startThread();
        void pauseThread();
        void resumeThread();
        void stopThread();
        void runThread();

    protected:
        boost::mutex mutex_;
        boost::condition cond_;
        boost::thread* thread_;
        bool stop_;
        bool pause_;
        bool is_running_;

    private:
        virtual void runFunction() = 0;
};

#endif // THREAD_H
