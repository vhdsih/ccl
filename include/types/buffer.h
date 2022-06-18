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
#include <vector>

#define LIMIT_MAX 10240

namespace ccl {

template <typename T>
class buffer_t {
public:
    explicit buffer_t();
    explicit buffer_t(size_t limit);
    explicit buffer_t(size_t n, std::function<void(T &)> release_fn);

    void push(const T &item);
    void emplace(const T &item);
    const T &get(size_t i) const;
    void clear();

    void release();

    bool full() const { return data_.size() >= limit_; }
    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
    size_t limit_capacity() const { return limit_; };

    void sort(std::function<bool(const T &, const T &)> fn);

    std::vector<T> &data() { return data_; }

    /* Copy disabled */
    buffer_t(buffer_t<T> &) = delete;
    buffer_t<T> &operator=(const buffer_t<T> &) = delete;

private:
    ~buffer_t();

private:
    std::vector<T> data_;
    size_t limit_;
    bool released_;
    std::function<void(T &)> item_release_fn_;
};

template <typename T>
buffer_t<T>::buffer_t()
    : limit_(LIMIT_MAX), released_(false), item_release_fn_(nullptr) {}

template <typename T>
buffer_t<T>::buffer_t(size_t limit)
    : limit_(limit), released_(false), item_release_fn_(nullptr) {}

template <typename T>
buffer_t<T>::buffer_t(size_t limit, std::function<void(T &)> fn)
    : limit_(limit), released_(false), item_release_fn_(fn) {}

template <typename T>
void buffer_t<T>::push(const T &item) {
    data_.push_back(item);
}

template <typename T>
void buffer_t<T>::emplace(const T &item) {
    data_.emplace_back(item);
}

template <typename T>
const T &buffer_t<T>::get(size_t i) const {
    // TODO: bug fix
    return data_[i];
}

template <typename T>
void buffer_t<T>::clear() {
    if (item_release_fn_) {
        for (auto item : data_) {
            item_release_fn_(item);
        }
    }
    data_.clear();
}

template <typename T>
void buffer_t<T>::release() {
    clear();
    released_ = true;
    delete this;
}

template <typename T>
void buffer_t<T>::sort(std::function<bool(const T &, const T &)> fn) {
    if (fn) {
        std::sort(data_.begin, data_.end(), fn);
    }
}

template <typename T>
buffer_t<T>::~buffer_t() {
    if (!released_) {
        release();
    }
}

}  // namespace erspan

#endif  // ERSPAN_AGENT_COMMON_BUFFER_H
