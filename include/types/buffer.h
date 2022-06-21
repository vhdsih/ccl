//
// Created by vhdsih on 2022/4/25.
// Used to cache data And reduce i/o between threads
//

#ifndef CCL_BUFFER_H
#define CCL_BUFFER_H

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <vector>

#include "../utils/noncopyable.h"

#define LIMIT_MAX 10240

namespace ccl {

template <typename T>
class buffer_t : public noncopyable {
public:
    explicit buffer_t();
    explicit buffer_t(size_t limit);

    void push(std::unique_ptr<T> item);
    const std::unique_ptr<T> &at(size_t i) const;

    void clear();
    void release();

    bool full() const { return data_.size() >= limit_; }
    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
    size_t limit_capacity() const { return limit_; };

    const std::vector<std::unique_ptr<T>> &data() const { return data_; }

private:
    ~buffer_t();

private:
    size_t limit_;
    bool released_;
    std::vector<std::unique_ptr<T>> data_;
};

template <typename T>
buffer_t<T>::buffer_t() : limit_(LIMIT_MAX), released_(false) {}

template <typename T>
buffer_t<T>::buffer_t(size_t limit) : limit_(limit), released_(false) {}

template <typename T>
void buffer_t<T>::push(std::unique_ptr<T> item) {
    data_.push_back(std::move(item));
}

template <typename T>
const std::unique_ptr<T> &buffer_t<T>::at(size_t i) const {
    return i >= data_.size() ? nullptr : data_.at(i);
}

template <typename T>
void buffer_t<T>::clear() {
    data_.clear();
}

template <typename T>
void buffer_t<T>::release() {
    clear();
    released_ = true;
    delete this;
}

template <typename T>
buffer_t<T>::~buffer_t() {
    if (!released_) {
        release();
    }
}

} // namespace ccl

#endif // ERSPAN_AGENT_COMMON_BUFFER_H
