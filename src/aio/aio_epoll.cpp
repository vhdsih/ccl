/*
 * @Author: zhangdongchang
 * @Date: 2022-06-17 14:54:08
 * @LastEditTime: 2022-06-18 23:10:36
 */

#if __linux__
#include "aio_epoll.h"

namespace ccl {

aio_epoll_t::aio_epoll_t(int timeout, int max_events) :
    epollfd_(epoll_create1(EPOLL_CLOEXEC)),
    timeout_(timeout),
    events_size_(max_events > 0 ? max_events : default_events_size),
    events_(new struct epoll_event[events_size_]) {}

aio_epoll_t::~aio_epoll_t() {
    close(epollfd_);
    delete[] events_;
}

int aio_epoll_t::add_event(int fd, event_type_t type) {
    int op        = 0;
    int fd_events = 0;
    if (fd2events_.find(fd) != fd2events_.end()) {
        fd_events = fd2events_[fd];
        op        = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
    }
    switch (type) {
    case event_type_t::read:
        fd_events |= EPOLLIN;
        break;
    case event_type_t::write:
        fd_events |= EPOLLOUT;
        break;
    case event_type_t::error:
        fd_events |= EPOLLERR;
        break;
    }

    fd2events_[fd] = fd_events;

    struct epoll_event event;
    event.data.fd = fd;
    event.events  = fd_events;
    return epoll_ctl(epollfd_, op, fd, &event);
}

int aio_epoll_t::del_event(int fd, event_type_t type) {
    (void)type;
    if (fd2events_.find(fd) == fd2events_.end()) {
        return 0;
    }
    return epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr);
}

int aio_epoll_t::listen_and_exec(callback_t callback) {
    int events = epoll_wait(epollfd_, events_, events_size_, timeout_);

    for (int i = 0; i < events; ++i) {
        if (events_[i].events & EPOLLIN) {
            callback(events_[i].data.fd, event_type_t::read);
        } else if (events_[i].events & EPOLLOUT) {
            callback(events_[i].data.fd, event_type_t::write);
        } else if (events_[i].events & EPOLLERR) {
            callback(events_[i].data.fd, event_type_t::error);
        } else {
            // TODO:
        }
    }
    return events;
}

} // namespace ccl

#endif