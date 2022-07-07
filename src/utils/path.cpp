/*
 * @Author: vhdsih
 * @Date: 2022-07-07 14:12:57
 */

#include "path.h"

#include <sys/stat.h>

namespace ccl {

bool try_mkdir(const char* path) {
    (void)path;
    return false;
}

bool path_exits(const char* path) {
    (void)path;
    return false;
}

bool path_is_dir(const char* path) {
    (void)path;
    return false;
}

bool path_is_file(const char* path) {
    (void)path;
    return false;
}

} // namespace ccl