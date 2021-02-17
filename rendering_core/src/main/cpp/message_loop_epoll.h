//
// Created by jj Rowand on 2/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_EPOLL_H
#define ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_EPOLL_H

#include "message_loop.h"
#include "sys/epoll.h"
#include "unistd.h"

namespace bw {
    class MessageLoopEpoll: public MessageLoop {
    public:
        MessageLoopEpoll();
        ~MessageLoopEpoll();

        void PostTask(const Task& task) override;
        void Run() override;
        void Terminate() override;

    private:
        void AddOrRemoveFD(bool add = true);
        void Awake();
        void Awoken();
    private:
        int pipe_fds_[2];
        int epoll_fd_;

        bool running_;
    };
}

#endif //ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_EPOLL_H
