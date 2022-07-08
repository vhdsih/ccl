/*
 * @Author: vhdsih
 * @Date: 2022-07-07 14:12:57
 */

#include "path.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

namespace ccl {

bool try_mkdir(const std::string& path) {
    size_t end   = 0;
    size_t start = 0;

    const std::string delim = "/";

    while (start < path.size()) {
        end = path.find(delim, start);
        end = (end == std::string::npos) ? path.size() : end;
        if (start != end) {
            struct stat st;
            memset(&st, 0, sizeof(struct stat));
            auto local = path.substr(0, end);
            if (local == ".") {
                start = end + 1;
                continue;
            }
            int ret = stat(local.c_str(), &st);
            if (ret == -1 && errno != ENOENT) {
                return false;
            }
            if (ret == 0 && !S_ISDIR(st.st_mode)) {
                return false;
            }
            if (ret == 0 && S_ISDIR(st.st_mode)) {
                start = end + 1;
                continue;
            }
            ret = mkdir(local.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (ret != 0) {
                return false;
            }
        }
        start = end + 1;
    }

    return false;
}

bool path_exits(const std::string& path) {
    (void)path;
    return false;
}

bool path_is_dir(const std::string& path) {
    (void)path;
    return false;
}

bool path_is_file(const std::string& path) {
    (void)path;
    return false;
}

} // namespace ccl