//
// Created by jj Rowand on 1/17/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_THREAD_H
#define ANDROIDRENDERPLAYGROUND_THREAD_H

#include <memory>
#include "message_loop.h"


namespace bw {

    class Thread final {
    public:

        Thread(const char* name);
        ~Thread();

        std::shared_ptr<TaskRunner> GetTaskRunner() const {
            return task_runner_;
        }

    private:
        void Join();

    private:
        Thread& operator=(const Thread&) = delete;
        Thread(const Thread&) = delete;

        std::shared_ptr<TaskRunner> task_runner_;
        std::unique_ptr<std::thread> thread_;
    };
}

#endif //ANDROIDRENDERPLAYGROUND_THREAD_H
