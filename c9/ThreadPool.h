#ifndef ThreadPool_h
#define ThreadPool_h

#include <queue>
#include "Work.h"

class ThreadPool {
public:
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
   std::queue<Work> workQueue_;
};

#endif
