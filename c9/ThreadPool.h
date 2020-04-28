#ifndef ThreadPool_h
#define ThreadPool_h

#include <queue>
#include <thread>
#include <memory>
#include <atomic>

#include "Work.h"

class ThreadPool {
public:
   virtual ~ThreadPool() {
      done_ = true;
      if (workThread_)
         workThread_->join();
   }

   void start() {
      workThread_ = std::make_shared<std::thread>(&ThreadPool::worker, this);
   }

   bool hasWork() {
      return !workQueue_.empty();
   }

   void add(Work work) {
      workQueue_.push(work);
   }

   Work pullWork() {
      auto work = workQueue_.front();
      workQueue_.pop();
      return work;
   }

private:
   void worker() {
      while (!done_) {
         while (!done_ && !hasWork())
            ;

	 if (done_) break;
         pullWork().execute();
      }
   }

   std::atomic<bool> done_{false};
   std::queue<Work> workQueue_;
   std::shared_ptr<std::thread> workThread_;
};

#endif
