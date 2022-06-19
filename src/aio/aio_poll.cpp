/*
 * @Author: vhdsih
 * @Date: 2022-06-18 22:41:35
 */

#include "aio_poll.h"

namespace ccl {

aio_poll_t::aio_poll_t(int timeout, int max_size) :
    timeout_(timeout),
    max_size_(max_size > 0 ? max_size : default_max_size),
    fds_(new struct pollfd[max_size_]) {}

aio_poll_t::~aio_poll_t() {
    delete[] fds_;
    fd2pollfd_.clear();
}

int aio_poll_t::add_event(int fd, event_type_t type) {
    auto to_insert = fd2pollfd_.size();
    if (fd2pollfd_.find(fd) == fd2pollfd_.end()) {
        if (to_insert >= max_size_) {
            return -1;
        }
        fds_[to_insert].fd      = fd;
        fds_[to_insert].events  = 0;
        fds_[to_insert].revents = 0;
        fd2pollfd_[fd]          = static_cast<int>(to_insert);
    } else {
        to_insert = fd2pollfd_[fd];
    }
    switch (type) {
    case event_type_t::read:
        fds_[to_insert].events |= POLLIN;
        break;
    case event_type_t::write:
        fds_[to_insert].events |= POLLOUT;
        break;
    case event_type_t::error:
        fds_[to_insert].events |= POLLERR;
        break;
    }
    return 1;
}

int aio_poll_t::del_event(int fd, event_type_t type) {
    (void)type;
    if (fd2pollfd_.find(fd) == fd2pollfd_.end()) {
        return 0;
    }
    auto to_del   = fd2pollfd_.at(fd);
    auto last_one = fd2pollfd_.size() - 1;

    fd2pollfd_[fds_[last_one].fd] = to_del;
    std::swap(fds_[to_del], fds_[last_one]);
    fd2pollfd_.erase(fd);
    return 1;
}

int aio_poll_t::listen_and_exec(callback_t callback) {
    int events = poll(fds_, fd2pollfd_.size(), timeout_);
    if (events <= 0) {
        return events;
    }
    for (size_t i = 0; i < fd2pollfd_.size(); ++i) {
        if (fds_[i].fd < 0) {
            continue;
        }
        if (fds_[i].revents & POLLIN) {
            callback(fds_[i].fd, event_type_t::read);
        } else if (fds_[i].revents & POLLOUT) {
            callback(fds_[i].fd, event_type_t::write);
        } else if (fds_[i].revents & POLLERR) {
            callback(fds_[i].fd, event_type_t::error);
        }
    }
    return events;
}

} // namespace ccl