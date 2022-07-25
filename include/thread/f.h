//
// Created by vhdsih on 2022/7/25.
//

#ifndef CCL_F_H
#define CCL_F_H

#include <future>

namespace ccl {

template <typename T>
inline bool future_is_ready(std::future<T> const& f) {
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

} // namespace ccl

#endif // CCL_F_H
