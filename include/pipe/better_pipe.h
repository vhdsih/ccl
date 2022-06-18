/*
 * @Author: zhangdongchang
 * @Date: 2022-06-16 00:06:26
 * @LastEditTime: 2022-06-18 12:27:52
 */

#ifndef CCL_BETTER_PIPE_H
#define CCL_BETTER_PIPE_H

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <functional>

namespace ccl {

template <typename T>
class better_pipe_t {
public:
    static const int default_limit = 64;

    using check_fn_t    = std::function<bool(const T &)>;
    using callback_fn_t = std::function<void(const T &)>;

    explicit better_pipe_t(int limit = default_limit);
    ~better_pipe_t();

    bool pop(check_fn_t check, callback_fn_t callback);
    bool push(const T &item);

    int rfd() const { return fd_[0]; }
    int wfd() const { return fd_[1]; }

private:
    bool safe_pop(check_fn_t check, callback_fn_t callback);
    bool unsafe_pop(check_fn_t check, callback_fn_t callback);

private:
    int idx_;                 // Index to read bytes from buffer
    int limit_;               // The maximum number of items that can be cached
    int fd_[2];               // Pipe fd
    size_t buff_len_;         // Buffer length
    size_t used_len_;         // The current number of bytes in the cache
    bool safe_;               // Whether the pipe is safe to read the packet
    unsigned char *buff_ptr_; // Buffer pointer
};

template <typename T>
better_pipe_t<T>::better_pipe_t(int limit) :
    idx_(0),
    limit_(limit),
    fd_{-1, -1},
    buff_len_(sizeof(T) *
              ((limit <= 0 || limit >= 1024) ? default_limit : limit)),
    used_len_(0),
    safe_(false),
    buff_ptr_(new unsigned char[buff_len_]) {
#if _GNU_SOURCE
    safe_ = true;
    if (pipe2(fd_, O_DIRECT | O_NONBLOCK) < 0) { // 'packet mode' + 'nonblock'
        perror("better pipe create failed!\n");
        exit(-1);
    }
#else
    safe_ = false;
    if (pipe(fd_) < 0) {
        perror("better pipe create failed!\n");
        exit(-1);
    }
    // enable 'nonblock'
    int flags;
    flags = fcntl(rfd(), F_GETFL, 0);
    fcntl(rfd(), F_SETFL, flags | O_NONBLOCK);
    flags = fcntl(wfd(), F_GETFL, 0);
    fcntl(wfd(), F_SETFL, flags | O_NONBLOCK);
#endif
}

template <typename T>
better_pipe_t<T>::~better_pipe_t() {
    delete[] buff_ptr_;
}

template <typename T>
bool better_pipe_t<T>::pop(check_fn_t check, callback_fn_t callback) {
    return safe_ ? safe_pop(check, callback) : unsafe_pop(check, callback);
}

template <typename T>
bool better_pipe_t<T>::push(const T &item) {
    return write(wfd(), &item, sizeof(T)) == sizeof(T);
}

template <typename T>
bool better_pipe_t<T>::safe_pop(check_fn_t check, callback_fn_t callback) {
    bool ret  = false;
    idx_      = 0;
    used_len_ = read(rfd(), &buff_ptr_, buff_len_);
    while (idx_ + sizeof(T) <= used_len_) {
        T item;
        memcpy(&item, buff_ptr_ + idx_, sizeof(T));
        if (check && !check(item)) {
            idx_ += 1;
            continue;
        }
        if (callback) {
            ret = true;
            callback(item);
        }
        idx_ += sizeof(T);
    }
    return ret;
}

template <typename T>
bool better_pipe_t<T>::unsafe_pop(check_fn_t check, callback_fn_t callback) {
    bool ret = false;
    idx_     = 0;
    used_len_ += read(rfd(), (buff_ptr_ + used_len_), (buff_len_ - used_len_));
    while (idx_ + sizeof(T) <= used_len_) {
        T item;
        memcpy(&item, buff_ptr_ + idx_, sizeof(T));
        if (check && !check(item)) {
            idx_ += 1;
            continue;
        }
        if (callback) {
            ret = true;
            callback(item);
        }
        idx_ += sizeof(T);
    }
    memmove(buff_ptr_, buff_ptr_ + idx_, used_len_ - idx_);
    used_len_ -= idx_;
    return ret;
}

} // namespace ccl

#endif