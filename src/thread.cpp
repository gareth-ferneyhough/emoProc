#include "thread.h"
#include <iostream>

ThreadWrapper::ThreadWrapper() :
    pause_ (false),
    stop_ (false),
    is_running_ (false),
    thread_ (NULL)
{
}

ThreadWrapper::~ThreadWrapper()
{
  if (thread_ != NULL){
    if (is_running_)
        stopThread();
    delete thread_;
  }
}

void ThreadWrapper::startThread()
{
  if (thread_ == NULL){
      std::cout << "creating thread\n";
      thread_ = new boost::thread(boost::bind(&ThreadWrapper::runThread, this));
      is_running_ = true;
  }
}

void ThreadWrapper::pauseThread()
{
    boost::mutex::scoped_lock lock(mutex_);
    pause_ = true;
    cond_.notify_one();
}

void ThreadWrapper::resumeThread()
{
    boost::mutex::scoped_lock lock(mutex_);
    pause_ = false;
    cond_.notify_one();
}

void ThreadWrapper::stopThread()
{
    boost::mutex::scoped_lock lock(mutex_);
    stop_=true;
    cond_.notify_one();
    thread_->join();
    is_running_ = false;

    std::cout << "stop thread\n";
}

void ThreadWrapper::runThread()
{
    while(stop_ == false)
    {
        if (pause_ == true)
        {
            boost::mutex::scoped_lock lock(mutex_);
            while(pause_ == true)
            {
                cond_.wait(lock);
            }
        }
        runFunction();
    }
}
