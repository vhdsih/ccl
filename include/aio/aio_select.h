/*
 * @Author: vhdsih
 * @Date: 2022-06-17 11:08:39
 */

#ifndef CCL_AIO_SELECT_H
#define CCL_AIO_SELECT_H

#include <memory>
#include <unordered_set>

#include "aio.h"

namespace ccl {

class aio_select_t : public aio_t {
public:
    static const int default_max_size = 128;
    using fds_t                       = std::unordered_set<int>;

    explicit aio_select_t(int timeout /* ms */,
                          int max_size = default_max_size);
    ~aio_select_t() override;
    int add_event(int fd, event_type_t type) override;
    int del_event(int fd, event_type_t type) override;
    int listen_and_exec(callback_t callback) override;

private:
    static void init_fdset(fd_set *fdsetp, fds_t &fds, int &max_fd);
    static void exec_task(fd_set *fdsetp, fds_t &fds, event_type_t,
                          callback_t &);

private:
    struct timeval timeout_;
    size_t max_size_;
    fds_t rfds_, wfds_, efds_, allfds_;
    fd_set *readset_, *writeset_, *exceptset_;
};

} // namespace ccl

#endif