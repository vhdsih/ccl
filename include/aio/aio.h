/*
 * @Author: vhdsih
 * @Date: 2022-06-17 11:09:35
 */

#ifndef CCL_AIO_H
#define CCL_AIO_H

#include <unistd.h>

#include <functional>

#include "../utils/noncopyable.h"
#include "aio_types.h"

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