/*
 * @Author: vhdsih
 * @Date: 2022-06-18 23:25:39
 */

#ifndef CCL_PIPE_H
#define CCL_PIPE_H

#include <fcntl.h>
#include <unistd.h>

namespace ccl {

class pipe_t {
public:
    pipe_t() = default;

    virtual ~pipe_t() = default;

    virtual int rfd() const = 0;
    virtual int wfd() const = 0;

    virtual bool packet_mode() const { return false; }

    virtual void rfd_unblocked() {
        int flags;
        flags = fcntl(rfd(), F_GETFL, 0);
        fcntl(rfd(), F_SETFL, flags | O_NONBLOCK);
    }

    virtual void wfd_unblocked() {
        int flags;
        flags = fcntl(wfd(), F_GETFL, 0);
        fcntl(wfd(), F_SETFL, flags | O_NONBLOCK);
    }
};

} // namespace ccl

#endif