//
// Created by vhdsih on 2022/8/3.
//

#ifndef CCL_THREAD_POOLS_H
#define CCL_THREAD_POOLS_H

#include "thread_pool.h"

namespace ccl {

class thread_pools_t : noncopyable {
public:
    thread_pools_t(size_t n_pool, size_t nth_in_pool);
    ~thread_pools_t();

    void close();

    template <typename Fn, typename... Args>
    auto push(size_t id, Fn &&fn, Args &&...args)
    -> std::future<decltype(fn(std::forward<Args>(args)...))>;

private:
    const size_t n_pool_;
    const size_t nth_in_pool_;
    std::atomic<bool> closed_;
    std::vector<std::unique_ptr<ccl::thread_pool_t>> pools_;
};

template <typename Fn, typename... Args>
auto thread_pools_t::push(size_t id, Fn &&fn, Args &&...args)
-> std::future<decltype(fn(std::forward<Args>(args)...))> {
    return pools_.at(id)->push(std::forward<Fn>(fn), std::forward<Args>(args)...);
}

} // namespace ccl

#endif // CCL_THREAD_POOLS_H
