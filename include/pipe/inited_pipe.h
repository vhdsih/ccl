/*
 * @Author: vhdsih
 * @Date: 2022-06-20 15:53:40
 */

#ifndef CCL_INITED_PIPE_H
#define CCL_INITED_PIPE_H

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>

#include "pipe.h"

namespace ccl {

class inited_pipe_t : public pipe_t {
public:
    inited_pipe_t() : fds_{-1, -1} {
        if (pipe(fds_) < 0) {
            perror("pipe create failed!\n");
        }
    }
    ~inited_pipe_t() override = default;

    int rfd() const override { return fds_[0]; }
    int wfd() const override { return fds_[1]; }

private:
    int fds_[2]; // Pipe fd
};

} // namespace ccl

#endif