
#ifndef CCL_NONCOPYABLE_H
#define CCL_NONCOPYABLE_H

namespace ccl {

class noncopyable {
public:
    noncopyable()                               = default;
    ~noncopyable()                              = default;
    noncopyable(const noncopyable &)            = delete;
    noncopyable &operator=(const noncopyable &) = delete;
};

} // namespace ccl

#endif