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
    bool push(size_t th_id, Fn &&fn, Args &&...args);

private:
    const size_t n_pool_;
    const size_t nth_in_pool_;
    std::atomic<bool> closed_;
    std::vector<std::unique_ptr<ccl::thread_pool_t>> pools_;
};

template <typename Fn, typename... Args>
bool thread_pools_t::push(size_t th_id, Fn &&fn, Args &&...args) {
    if (th_id >= n_pool_) {
        return false;
    }
    pools_[th_id]->push(std::forward<Fn>(fn), std::forward<Args>(args)...);
    return true;
}

} // namespace ccl

#endif // CCL_THREAD_POOLS_H
