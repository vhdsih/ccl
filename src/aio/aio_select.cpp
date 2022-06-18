/*
 * @Author: zhangdongchang
 * @Date: 2022-06-17 11:36:27
 * @LastEditTime: 2022-06-17 15:56:55
 */

#include "aio_select.h"

namespace ccl {

aio_select_t::aio_select_t(int timeout, int max_size) :
    timeout_{timeout / 1000, timeout % 1000},
    max_size_(max_size > 0 ? max_size : default_max_size),
    readset_(nullptr),
    writeset_(nullptr),
    exceptset_(nullptr) {}

aio_select_t::~aio_select_t() {
    delete readset_;
    delete writeset_;
    delete exceptset_;
}

int aio_select_t::add_event(int fd, event_type_t type) {
    switch (type) {
    case event_type_t::read:
        rfds_.insert(fd);
        if (readset_ == nullptr) {
            readset_ = new fd_set;
        }
        break;
    case event_type_t::write:
        wfds_.insert(fd);
        if (writeset_ == nullptr) {
            writeset_ = new fd_set;
        }
        break;
    case event_type_t::error:
        efds_.insert(fd);
        if (exceptset_ == nullptr) {
            exceptset_ = new fd_set;
        }
        break;
    }
    allfds_.insert(fd);
    if (allfds_.size() > max_size_) {
        return -1;
    }
    return 1;
}

int aio_select_t::del_event(int fd, event_type_t type) {
    switch (type) {
    case event_type_t::read:
        rfds_.erase(fd);
        break;
    case event_type_t::write:
        wfds_.erase(fd);
        break;
    case event_type_t::error:
        efds_.erase(fd);
        break;
    }
    allfds_.erase(fd);
    return -1;
}

int aio_select_t::listen_and_exec(callback_t callback) {
    int max_fd = 0, events = 0;

    init_fdset(readset_, rfds_, max_fd);
    init_fdset(writeset_, wfds_, max_fd);
    init_fdset(exceptset_, efds_, max_fd);

    events = select(max_fd + 1, readset_, writeset_, exceptset_, &timeout_);

    if (events <= 0) {
        return events;
    }

    exec_task(readset_, rfds_, event_type_t::read, callback);
    exec_task(writeset_, wfds_, event_type_t::write, callback);
    exec_task(exceptset_, efds_, event_type_t::error, callback);

    return events;
}

void aio_select_t::init_fdset(fd_set *fdsetp, fds_t &fds, int &max_fd) {
    if (fdsetp) {
        FD_ZERO(fdsetp);
        for (auto fd : fds) {
            FD_SET(fd, fdsetp);
            max_fd = std::max(max_fd, fd);
        }
    }
}

void aio_select_t::exec_task(fd_set *fdsetp, fds_t &fds, event_type_t type,
                             callback_t &callback) {
    for (auto fd : fds) {
        if (FD_ISSET(fd, fdsetp)) {
            callback(fd, type);
        }
    }
}

} // namespace common