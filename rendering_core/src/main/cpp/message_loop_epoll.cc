//
// Created by jj Rowand on 2/16/21.
//

#include "message_loop_epoll.h"
#include "log.h"

#define LOG_TAG "MessageLoopEpoll"
namespace bw {
    MessageLoopEpoll::MessageLoopEpoll():running_(false){
        epoll_fd_ = epoll_create(1/*unused*/);
        if(epoll_fd_ == -1) {
            ALOGE("create epoll failure...");
            return;
        }

        if(pipe(pipe_fds_) == -1) {
            ALOGE("create pipe failure...");
            return;
        }

        AddOrRemoveFD(true);

        ALOGD("epoll setup success...");
    }

    MessageLoopEpoll::~MessageLoopEpoll() {
        AddOrRemoveFD(false);
        close(pipe_fds_[0]);
        close(pipe_fds_[1]);
        ALOGD("message loop epoll destructor...");
    }

    void MessageLoopEpoll::PostTask(const Task& task) {
        ALOGE("post task success!!!!");

        if(running_) {
            RegisterTaskLocked(task);
            Awake();
        } else {
            ALOGE("post task failed. message loop is terminate...");
        }
    }

    void MessageLoopEpoll::Run() {
        if(epoll_fd_ <= 0 || pipe_fds_[0] <= 0) {
            ALOGE("invalid descriptor...");
            return;
        }
        running_ = true;

        while(running_) {
            struct epoll_event event = {};
            int result = epoll_wait(epoll_fd_, &event, 1, -1/*infinity*/);// 阻塞等

            // 错误事件
            if(event.events & (EPOLLERR | EPOLLHUP)) {
                running_ = false;
                continue;
            }

            // 唤醒事件
            if(result != 1) {
                running_ = false;
                continue;
            }

            ALOGW("try handle events...");

            if(event.data.fd == pipe_fds_[0]) {// 读事件
                DrainTasksLocked();// 消费task
                Awoken();// 消耗掉管道读事件
            }
        }

        ALOGW("exit message loop...");
    }

    void MessageLoopEpoll::Terminate() {
        ALOGD("will terminate...");
        running_ = false;
        Awake();// 唤醒消息循环
    }

    void MessageLoopEpoll::AddOrRemoveFD(bool add) {
        struct epoll_event event = {};
        event.events = EPOLLIN;
        event.data.fd = pipe_fds_[0];// 关心读事件

        if(epoll_ctl(epoll_fd_, add ? EPOLL_CTL_ADD: EPOLL_CTL_DEL, event.data.fd, &event) != 0) {
            ALOGE("epoll add or remove fd failed");
        } else {
            ALOGD("epoll add or remove fd success");
        }
    }

    void MessageLoopEpoll::Awake() {
        if(pipe_fds_[1] < 0) {
            ALOGE("pipe write descriptor is invalid...");
            return;
        }
        write(pipe_fds_[1], "w", 1);
    }

    void MessageLoopEpoll::Awoken() {
        if(pipe_fds_[0] < 0) {
            ALOGE("pipe read descriptor is invalid");
            return;
        }
        char buf;
        read(pipe_fds_[0], &buf, sizeof(char));
    }

}