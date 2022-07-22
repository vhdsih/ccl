//
// Created by vhdsih on 2022/7/22.
//

#ifndef ERSPAN_AGENT_THREAD_POOL_H
#define ERSPAN_AGENT_THREAD_POOL_H

#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <memory>
#include <condition_variable>

#include "safe_queue_t.h"
#include "../utils/noncopyable.h"

namespace ccl {

class thread_pool_t : noncopyable {
public:
    using task_t = std::function<int(void)>;

    explicit thread_pool_t(size_t n);

    ~thread_pool_t();

    template<typename Fn, typename ...Args>
    bool push(Fn &fn, Args && ...args);

    void get();

    void start();
    void close();

private:
    std::unique_ptr<std::thread> thread_init();

private:
    std::atomic<int> state_;
    std::function<void(void)> thread_fn_;

    std::mutex mutex_;
    std::condition_variable cond_var_;
    safe_queue_t<std::unique_ptr<task_t*>> tasks_;
    std::vector<std::unique_ptr<std::thread>> threads_;
};

}

#endif // ERSPAN_AGENT_THREAD_POOL_H
