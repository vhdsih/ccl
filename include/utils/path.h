/*
 * @Author: vhdsih
 * @Date: 2022-07-07 14:07:30
 */

#ifndef CCL_PATH_H
#define CCL_PATH_H

#include <string>

namespace ccl {

bool try_mkdir(const std::string& path);
bool path_exits(const std::string& path);
bool path_is_dir(const std::string& path);
bool path_is_file(const std::string path);

} // namespace ccl

#endif