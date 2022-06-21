/*
 * @Author: vhdsih
 * @Date: 2022-06-18 22:41:35
 */

#ifndef CCL_PIPE_HELPER_H
#define CCL_PIPE_HELPER_H

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <functional>

#include "pipe.h"

namespace ccl {

template <typename T>
class pipe_helper_t {
public:
    using check_fn_t    = std::function<bool(const T &)>;
    using callback_fn_t = std::function<void(const T &)>;

    using stream_t = std::unique_ptr<pipe_t> &;

    static bool push(stream_t, const T &item);
    static bool pop(stream_t, check_fn_t, callback_fn_t, char *buff,
                    size_t len_len);

private:
    static bool safe_pop(stream_t, check_fn_t, callback_fn_t, char *buff,
                         size_t buff_len);
    static bool unsafe_pop(stream_t, check_fn_t, callback_fn_t callback,
                           char *buff, size_t buff_len);
};

template <typename T>
inline bool pipe_helper_t<T>::pop(stream_t stream, check_fn_t check,
                                  callback_fn_t callback, char *buff,
                                  size_t buff_len) {
    if (!stream) return false;
    return stream->packet_mode()
               ? safe_pop(stream, check, callback, buff, buff_len)
               : unsafe_pop(stream, check, callback, buff, buff_len);
}

template <typename T>
inline bool pipe_helper_t<T>::push(stream_t &stream, const T &item) {
    if (!stream) return false;
    return write(stream->wfd(), &item, sizeof(T)) == sizeof(T);
}

template <typename T>
inline bool pipe_helper_t<T>::safe_pop(stream_t stream, check_fn_t check,
                                       callback_fn_t callback, char *buff,
                                       size_t buff_len) {
    if (!stream) return false;

    bool ret         = false;
    size_t idx       = 0;
    size_t n_to_read = sizeof(T) * buff_len / sizeof(T);
    size_t n_readed  = read(stream->rfd(), buff, n_to_read);
    while (idx + sizeof(T) <= n_readed) {
        T item;
        memcpy(&item, buff + idx, sizeof(T));
        if (check && !check(item)) {
            idx += 1;
            continue;
        }
        if (callback) {
            ret = true;
            callback(item);
        }
        idx += sizeof(T);
    }
    return ret;
}

template <typename T>
inline bool pipe_helper_t<T>::unsafe_pop(stream_t stream, check_fn_t check,
                                         callback_fn_t callback, char *buff,
                                         size_t buff_len) {
    return safe_pop(stream, check, callback, buff, buff_len);
}

} // namespace ccl

#endif