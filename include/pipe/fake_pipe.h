/*
 * @Author: vhdsih
 * @Date: 2022-06-20 15:47:57
 */

#ifndef CCL_FAKE_PIPE_H
#define CCL_FAKE_PIPE_H

#include "pipe.h"

namespace ccl {

class fake_pipe_t : public pipe_t {
public:
    fake_pipe_t(int r, int w) : rfd_(r), wfd_(w) {}
    ~fake_pipe_t() override = default;
    int rfd() const override { return rfd_; }
    int wfd() const override { return wfd_; }

private:
    int rfd_;
    int wfd_;
};

} // namespace ccl

#endif