/*
 * @Author: vhdsih
 * @Date: 2022-06-21 22:22:53
 */

#ifndef CCL_PTR_H
#define CCL_PTR_H

#include <memory>

namespace ccl {

template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
#if __cplusplus < 201300
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
#else
    return std::make_unique<T>(std::forward<Ts>(params)...);
#endif
}

} // namespace ccl

#endif // CCL_PTR_H