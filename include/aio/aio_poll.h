/*
 * @Author: vhdsih
 * @Date: 2022-06-17 11:08:48
 */

#ifndef CCL_AIO_POLL_H
#define CCL_AIO_POLL_H

#include <poll.h>

#include <unordered_map>

#include "aio.h"

namespace ccl {

class aio_poll_t : public aio_t {
public:
    static const int default_max_size = 128;

    explicit aio_poll_t(int timeout /* ms */, int max_size = default_max_size);
    ~aio_poll_t() override;

    int add_event(int fd, event_type_t type) override;
    int del_event(int fd, event_type_t type) override;
    int listen_and_exec(callback_t callback) override;

private:
    int timeout_;
    size_t max_size_;
    struct pollfd *fds_;
    std::unordered_map<int, int> fd2pollfd_;
};

} // namespace ccl

#endif