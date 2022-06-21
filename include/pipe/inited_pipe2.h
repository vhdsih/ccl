/*
 * @Author: vhdsih
 * @Date: 2022-06-20 15:53:40
 */

#ifndef CCL_INITED_PIPE2_H
#define CCL_INITED_PIPE2_H
#if 0

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>

#include "pipe.h"

namespace ccl {

class inited_pipe2_t : public pipe_t {
public:
    inited_pipe2_t() {
        fds_[0] = -1;
        fds_[1] = -1;
        if (pipe2(fds_, O_DIRECT | O_NONBLOCK) < 0) {
            // 'packet mode' + 'nonblock'
            perror("better pipe create failed!\n");
        }
    }
    ~inited_pipe2_t() override = default;

    int rfd() const override { return fds_[0]; }
    int wfd() const override { return fds_[1]; }

    bool packet_mode() const override { return true; }

private:
    int fds_[2]; // Pipe fd
};

} // namespace ccl

#endif
#endif