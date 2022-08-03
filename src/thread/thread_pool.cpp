//
// Created by vhdsih on 2022/7/22.
//

#include "thread_pool.h"

#include <iostream>
#include <thread>

namespace ccl {

thread_pool_t::thread_pool_t(size_t n) :
    closed_(false), terminate_(false), n_idle_(n) {
    for (size_t i = 0; i < n; ++i) {
        // warn: must init flags first
        flags_.push_back(ccl::make_unique<std::atomic<bool>>(false));
        threads_.push_back(make_thread(i));
    }
}

thread_pool_t::~thread_pool_t() { close(); }

void thread_pool_t::close() {
    if (closed_) {
        return;
    } else {
        closed_ = true;
    }

    // wait tasks
    while (!tasks_.empty()) {
        std::lock_guard<std::mutex> lk(cv_m_);
        cv_.notify_one();
    }

    // terminate
    terminate_ = true;
    {
        std::lock_guard<std::mutex> lk(cv_m_);
        cv_.notify_all();
    }

    // wait threads
    for (auto &th : threads_) {
        th->join();
    }

    threads_.clear();
    while (!tasks_.empty()) tasks_.pop();
}

void thread_pool_t::runner(size_t id) {
    std::shared_ptr<wrappered_task_t> task = nullptr;
    while (true) {
        task = nullptr;
        {
            // wait the task
            std::unique_lock<std::mutex> lk(cv_m_);
            cv_.wait(lk, [&] {
                return terminate_ || tasks_.pop(task) || *flags_[id];
            });
        }

        // check thread pool
        if (terminate_) {
            --n_idle_;
            break;
        }
        if (*flags_[id]) {
            --n_idle_;
            break;
        }
        if (!task) {
            continue;
        }

        // exec the task
        --n_idle_;
        (*task)();
        ++n_idle_;
    }
}

std::unique_ptr<std::thread> thread_pool_t::make_thread(size_t i) {
    return ccl::make_unique<std::thread>([&](size_t i) { this->runner(i); }, i);
}

void thread_pool_t::resize(size_t n) {
    if (n > size()) {
        for (auto i = threads_.size(); i < n; ++i) {
            ++n_idle_;
            threads_.push_back(make_thread(i));
            flags_.push_back(ccl::make_unique<std::atomic<bool>>(false));
        }
    } else if (n < size()) {
        while (size() > n) {
            size_t idx   = size() - 1;
            *flags_[idx] = true;
            threads_[idx]->detach();
            useless_threads_.push_back(std::move(threads_[idx]));
            threads_.pop_back();
        }
        {
            std::unique_lock<std::mutex> lock(cv_m_);
            cv_.notify_all();
        }
    } else {
    }
}

} // namespace ccl
