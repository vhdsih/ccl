//
// Created by vhdsih on 2022/8/3.
//

#include "thread_pools.h"

namespace ccl {

thread_pools_t::thread_pools_t(size_t n_pool, size_t nth_in_pool) :
    n_pool_(n_pool), nth_in_pool_(nth_in_pool), closed_(false) {
    for (size_t i = 0; i < n_pool_; ++i) {
        pools_.push_back(ccl::make_unique<thread_pool_t>(nth_in_pool_));
    }
}

thread_pools_t::~thread_pools_t() { close(); }

void thread_pools_t::close() {
    if (closed_) {
        return;
    } else {
        closed_ = true;
    }
    for (auto &pool : pools_) {
        pool->close();
    }
}

} // namespace ccl
