/*
 * @Author: zhangdongchang
 * @Date: 2022-06-17 11:08:55
 * @LastEditTime: 2022-06-17 18:56:14
 */

#ifndef CCL_AIO_EPOLL_H
#define CCL_AIO_EPOLL_H

#if __linux__

#include <sys/epoll.h>

#include <unordered_map>

#include "aio.h"

namespace ccl {

class aio_epoll_t : public aio_t {
public:
    static const int default_events_size = 128;

    explicit aio_epoll_t(int timeout /* ms */,
                         int max_events = default_events_size);
    ~aio_epoll_t() override;
    int add_event(int fd, event_type_t type) override;
    int del_event(int fd, event_type_t type) override;
    int listen_and_exec(callback_t callback) override;

private:
    int epollfd_;
    int timeout_;
    int events_size_;
    struct epoll_event *events_;
    std::unordered_map<int, int> fd2events_;
};

} // namespace ccl

#endif
#endif