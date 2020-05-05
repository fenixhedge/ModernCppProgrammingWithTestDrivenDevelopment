#ifndef ThreadPool_h
#define ThreadPool_h

#include <queue>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <vector>
#include <iostream>

#include "Work.h"

class ThreadPool {
public:
   virtual ~ThreadPool() {
      stop();
   }

   void start(unsigned numberOfThreads=1) {
      for (unsigned i{0u}; i < numberOfThreads; i++)
         threads_.push_back(std::thread(&ThreadPool::worker, this));
   }

   void stop() {
      done_ = true;
      for (auto& thread : threads_) thread.join();
   }

   bool hasWork() {
      std::lock_guard<std::mutex> block(mutex_);
      return !workQueue_.empty();
   }

   virtual void add(Work work) {
      std::lock_guard<std::mutex> block(mutex_);
      workQueue_.push(work);
   }

   Work pullWork() {
      std::lock_guard<std::mutex> block(mutex_);

      if (workQueue_.empty()) return Work{};

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
   std::mutex mutex_;
   std::vector<std::thread> threads_;
};

#endif
