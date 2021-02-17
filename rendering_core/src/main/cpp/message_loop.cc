//
// Created by jj Rowand on 2/16/21.
//

#include "message_loop.h"
#include "message_loop_epoll.h"
#include <pthread.h>
#include "log.h"

#define LOG_TAG "MessageLoop"

namespace bw {

    enum {
        TYPE_EPOLL = 0,
        TYPE_ALOOPER = 1
    };

    namespace {
        int g_impl_type_ = TYPE_EPOLL;
        pthread_key_t g_message_loop_impl_tls_ = -1;
    }

    TaskRunner::TaskRunner(MessageLoop *message_loop) :message_loop_(message_loop),is_valid_(true) {}

    void TaskRunner::PostTask(const Task &task) {
        if(is_valid_ && message_loop_) {
            message_loop_->PostTask(task);
        }
    }

    std::shared_ptr<TaskRunner> MessageLoop::GetTaskRunner() {
        return task_runner_;
    }

    MessageLoop::MessageLoop():
        task_runner_(std::make_shared<TaskRunner>(this)) {}

    MessageLoop::~MessageLoop() {
        ALOGE("MessageLoop dtor");
        if(g_message_loop_impl_tls_ != -1) {
            pthread_key_delete(g_message_loop_impl_tls_);
            g_message_loop_impl_tls_ = -1;
            task_runner_->SetValid(false);
        }
    }

    void MessageLoop::EnsureInitialized() {
        if(g_message_loop_impl_tls_ == -1) {
            // 未初始化，则设置tls
            if(g_impl_type_ == TYPE_EPOLL) {
                pthread_key_create(&g_message_loop_impl_tls_,[](void* message_loop){
                    /*destroy*/
                    delete reinterpret_cast<MessageLoopEpoll*>(message_loop);
                });
                pthread_setspecific(g_message_loop_impl_tls_, new MessageLoopEpoll());
            } else {
                // TODO: Not implemented
            }
        }
    }

    MessageLoop* MessageLoop::GetCurrent() {
        EnsureInitialized();
        return reinterpret_cast<MessageLoop*>(pthread_getspecific(g_message_loop_impl_tls_));
    }

    void MessageLoop::DrainTasksLocked() {
        std::lock_guard<std::mutex> guard(task_mutex_);
        while (!task_queue_.empty()) {
            auto& task = task_queue_.front();
            task();
            task_queue_.pop();
        }
    }

    void MessageLoop::RegisterTaskLocked(Task &&task) {
        std::lock_guard<std::mutex> guard(task_mutex_);
        task_queue_.push(task);
    }

    void MessageLoop::RegisterTaskLocked(const Task &task) {
        std::lock_guard<std::mutex> guard(task_mutex_);
        task_queue_.push(task);
    }
}


