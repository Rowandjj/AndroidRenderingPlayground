//
// Created by jj Rowand on 1/17/21.
//

#include "thread.h"
#include <thread>
#include <pthread.h>
#include <semaphore.h>
#include "log.h"

#define LOG_TAG "Thread"

namespace bw {
    Thread::Thread(const char* name) {
        sem_t sem;
        std::shared_ptr<TaskRunner> runner_;
        if(sem_init(&sem, 0/*NOT_SHARED*/, 0) != 0) {
            ALOGE("sem_init failed...");
            return;
        }
        thread_ = std::make_unique<std::thread>([&sem, &runner_, name](){
            pthread_setname_np(pthread_self(), name);

            MessageLoop* loop = MessageLoop::GetCurrent();
            if(loop == nullptr) {
                ALOGE("initialize MessageLoop failed...");
                return;
            }
            runner_ = loop->GetTaskRunner();
            sem_post(&sem);
            loop->Run();
        });
        sem_wait(&sem);
        task_runner_ = runner_;
        if(task_runner_ == nullptr) {
            ALOGE("task runner is invalid!");
        }
    }

    Thread::~Thread() {
        this->Join();
        ALOGE("Thread dtor called...");
    }

    void Thread::Join() {
        if(task_runner_) {
            task_runner_->PostTask([]() {
                if(MessageLoop::GetCurrent()) {
                    MessageLoop::GetCurrent()->Terminate();
                }
            });
        }
        thread_->join();
    }
}