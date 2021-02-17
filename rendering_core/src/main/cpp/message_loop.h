//
// Created by jj Rowand on 2/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_H
#define ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_H

#include <functional>
#include <queue>
#include <mutex>
#include <atomic>

namespace bw {

    class MessageLoop;
    using Task = std::function<void()>;

    class TaskRunner final{
    public:
        TaskRunner(MessageLoop* message_loop);
        ~TaskRunner() = default;

        void PostTask(const Task& task);

        void SetValid(bool valid) {
            is_valid_ = valid;
        }

    private:
        MessageLoop* message_loop_;

        std::atomic<bool> is_valid_;
    };

    class MessageLoop {
    public:

        static MessageLoop* GetCurrent();

        MessageLoop();
        virtual ~MessageLoop();

        virtual void Run() = 0;
        virtual void Terminate() = 0;

        std::shared_ptr<TaskRunner> GetTaskRunner();

    protected:
        using TaskQueue = std::queue<Task>;
        void DrainTasksLocked();
        void RegisterTaskLocked(Task&& task);
        void RegisterTaskLocked(const Task& task);

        virtual void PostTask(const Task& task) {}

    private:
        static void EnsureInitialized();

        friend TaskRunner;
        std::mutex task_mutex_;
        TaskQueue task_queue_;

        std::shared_ptr<TaskRunner> task_runner_;
    };
}

#endif //ANDROIDRENDERPLAYGROUND_MESSAGE_LOOP_H
