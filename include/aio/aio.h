/*
 * @Author: zhangdongchang
 * @Date: 2022-06-17 11:09:35
 * @LastEditTime: 2022-06-18 12:32:22
 */

#ifndef CCL_AIO_H
#define CCL_AIO_H

#include <unistd.h>

#include <functional>

#include "noncopyable.h"

namespace ccl {

enum class event_type_t : uint8_t {
    read,
    write,
    error,
};
} // namespace ccl

namespace std {

template <>
struct hash<ccl::event_type_t> {
    size_t operator()(const ccl::event_type_t type) const {
        return static_cast<size_t>(type);
    }
};

} // namespace std

namespace ccl {

class aio_t : public noncopyable {
public:
    using callback_t = std::function<void(int, event_type_t)>;

    aio_t()          = default;
    virtual ~aio_t() = default;

    virtual int add_event(int fd, event_type_t type) = 0;
    virtual int del_event(int fd, event_type_t type) = 0;
    virtual int listen_and_exec(callback_t callback) = 0;

private:
};

} // namespace ccl

#endif