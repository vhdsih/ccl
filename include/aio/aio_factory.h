/*
 * @Author: zhangdongchang
 * @Date: 2022-06-17 16:06:06
 * @LastEditTime: 2022-06-18 12:32:35
 */

#ifndef CCL_AIO_FACTORY_H
#define CCL_AIO_FACTORY_H

#include <memory>

#include "aio.h"
#include "aio_epoll.h"
#include "aio_poll.h"
#include "aio_select.h"

namespace ccl {

enum class aio_type_t : uint8_t {
    poll,
    epoll,
    select,
};
} // namespace ccl

namespace std {

template <>
struct hash<ccl::aio_type_t> {
    size_t operator()(const ccl::aio_type_t type) const {
        return static_cast<size_t>(type);
    }
};

} // namespace std

namespace ccl {

class aio_factory_t {
public:
    static std::unique_ptr<aio_t> create(aio_type_t type, int timeout,
                                         int size) {
        switch (type) {
        case aio_type_t::poll:
            return std::unique_ptr<aio_t>(new aio_poll_t(timeout, size));
        case aio_type_t::epoll:
#if __linux__
            return std::unique_ptr<io_t>(new io_epoll_t(timeout, size));
#else
            return std::unique_ptr<aio_t>(new aio_poll_t(timeout, size));
#endif
        case aio_type_t::select:
            return std::unique_ptr<aio_t>(new aio_select_t(timeout, size));
        default:;
        }
        return nullptr;
    }
};

} // namespace ccl

#endif