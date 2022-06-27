/*
 * @Author: vhdsih
 * @Date: 2022-06-19 22:11:06
 */

#ifndef CCL_UTILS_LOG_H
#define CCL_UTILS_LOG_H

#include <ctime>

#ifndef logger
#define logger(...)                    \
    printf("[I]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

#endif

#ifndef logger_error
#define logger_error(...)              \
    printf("[E]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

#endif

#ifndef logger_warn
#define logger_warn(...)               \
    printf("[W]%ld: ", time(nullptr)); \
    printf(__VA_ARGS__);               \
    printf("\n");

#endif

namespace ccl {} // namespace ccl

#endif
