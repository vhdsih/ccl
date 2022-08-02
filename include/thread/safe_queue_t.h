//
// Created by vhdsih on 2022/7/22.
//

#ifndef CCL_SAFE_QUEUE_T_H
#define CCL_SAFE_QUEUE_T_H

#include <mutex>
#include <queue>

namespace ccl {

template <typename value_type>
class safe_queue_t {
public:
    safe_queue_t();
    ~safe_queue_t();

    void push(value_type& val);
    void push(const value_type& val);

    void pop();
    bool pop(value_type& val);

    value_type front() const;
    value_type back() const;

    bool empty() const;
    size_t size() const;

private:
    mutable std::mutex mutex_;
    std::queue<value_type> q_;
};

template <typename value_type>
safe_queue_t<value_type>::safe_queue_t() = default;

template <typename value_type>
safe_queue_t<value_type>::~safe_queue_t() = default;

template <typename value_type>
void safe_queue_t<value_type>::push(value_type& val) {
    std::lock_guard<std::mutex> guard(mutex_);
    q_.push(val);
}

template <typename value_type>
void safe_queue_t<value_type>::push(const value_type& val) {
    std::lock_guard<std::mutex> guard(mutex_);
    q_.push(std::forward<value_type>(val));
}

template <typename value_type>
void safe_queue_t<value_type>::pop() {
    std::lock_guard<std::mutex> guard(mutex_);
    if (!q_.empty()) {
        q_.pop();
    }
}

template <typename value_type>
bool safe_queue_t<value_type>::pop(value_type& val) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (q_.empty()) {
        return false;
    } else {
        std::swap(val, q_.front());
        q_.pop();
        return true;
    }
}

template <typename value_type>
value_type safe_queue_t<value_type>::front() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return q_.front();
}

template <typename value_type>
value_type safe_queue_t<value_type>::back() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return q_.back();
}

template <typename value_type>
bool safe_queue_t<value_type>::empty() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return q_.empty();
}

template <typename value_type>
size_t safe_queue_t<value_type>::size() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return q_.size();
}

} // namespace ccl

#endif // CCL_SAFE_QUEUE_T_H
