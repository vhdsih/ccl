//
// Created by vhdsih on 2022/7/22.
//

#ifndef CCL_THREAD_POOL_H
#define CCL_THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../utils/noncopyable.h"
#include "../utils/ptr.h"
#include "safe_queue_t.h"

namespace ccl {

class thread_pool_t : noncopyable {
public:
    using wrappered_task_t = std::function<void(void)>;

    explicit thread_pool_t(size_t n);

    ~thread_pool_t();

    template <typename Fn, typename... Args>
    auto push(Fn &&fn, Args &&...args) -> std::future<decltype(fn(args...))>;

    void close();

    void resize(size_t n);

    size_t idle() const { return n_idle_; }
    size_t size() const { return threads_.size(); }

private:
    void runner(size_t id);
    std::unique_ptr<std::thread> make_thread(size_t id);

private:
    std::atomic<bool> closed_;
    std::atomic<bool> terminate_;
    std::atomic<size_t> n_idle_;

    mutable std::mutex cv_m_;
    std::condition_variable cv_;
    safe_queue_t<std::shared_ptr<wrappered_task_t>> tasks_;
    std::vector<std::unique_ptr<std::thread>> threads_;
    std::vector<std::unique_ptr<std::thread>> useless_threads_;
    std::vector<std::unique_ptr<std::atomic<bool>>> flags_;
};

template <typename Fn, typename... Args>
auto thread_pool_t::push(Fn &&fn, Args &&...args)
    -> std::future<decltype(fn(args...))> {
    // packaged the fn
    auto pck = std::make_shared<std::packaged_task<decltype(fn(args...))()>>(
        [&]() { return fn(std::forward<Args>(args)...); });

    // wrapper the pck
    auto task = std::make_shared<wrappered_task_t>(
        std::function<void(void)>([pck]() -> void { (*pck)(); }));

    // push into the queue
    tasks_.push(task);

    // notify one thread
    {
        std::unique_lock<std::mutex> lock(cv_m_);
        cv_.notify_one();
    }
    return pck->get_future();
}

} // namespace ccl

#endif // CCL_THREAD_POOL_H
