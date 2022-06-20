/*
 * @Author: vhdsih
 * @Date: 2022-06-19 22:11:06
 */

#ifndef CCL_UTILS_LOG_H
#define CCL_UTILS_LOG_H

#include <ctime>

#define logger(...)                    \
    printf("[I]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

#define logger_error(...)              \
    printf("[E]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

#define logger_warn(...)               \
    printf("[W]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

namespace ccl {} // namespace ccl

#endif
