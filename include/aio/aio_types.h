/*
 * @Author: vhdsih
 * @Date: 2022-06-19 11:32:14
 */

#ifndef CCL_AIO_TYPES_H
#define CCL_AIO_TYPES_H

#include <unistd.h>

#include <unordered_map>

namespace ccl {

enum class aio_type_t : uint8_t {
    poll,
    epoll,
    select,
};

enum class event_type_t : uint8_t {
    read,
    write,
    error,
};

} // namespace ccl

namespace std {

template <>
struct hash<ccl::aio_type_t> {
    size_t operator()(const ccl::aio_type_t type) const {
        return static_cast<size_t>(type);
    }
};

template <>
struct hash<ccl::event_type_t> {
    size_t operator()(const ccl::event_type_t type) const {
        return static_cast<size_t>(type);
    }
};

} // namespace std

#endif