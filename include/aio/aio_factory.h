/*
 * @Author: vhdsih
 * @Date: 2022-06-17 16:06:06
 */

#ifndef CCL_AIO_FACTORY_H
#define CCL_AIO_FACTORY_H

#include <memory>

#include "../utils/ptr.h"
#include "aio.h"
#include "aio_epoll.h"
#include "aio_poll.h"
#include "aio_select.h"
#include "aio_types.h"

namespace ccl {

class aio_factory_t {
public:
    static std::unique_ptr<aio_t> create(aio_type_t type, int timeout,
                                         int size) {
        switch (type) {
        case aio_type_t::poll:
            return ccl::make_unique<aio_poll_t>(timeout, size);
        case aio_type_t::epoll:
#if __linux__
            return ccl::make_unique<aio_epoll_t>(timeout, size);
#else
            return ccl::make_unique<aio_poll_t>(timeout, size);
#endif
        case aio_type_t::select:
            return ccl::make_unique<aio_select_t>(timeout, size);
        default:;
            return nullptr;
        }
    }
};

} // namespace ccl

#endif