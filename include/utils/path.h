/*
 * @Author: vhdsih
 * @Date: 2022-07-07 14:07:30
 */

#ifndef CCL_PATH_H
#define CCL_PATH_H

namespace ccl {

bool mkdir(const char* path);
bool path_exits(const char* path);
bool path_is_dir(const char* path);
bool path_is_file(const char* path);

} // namespace ccl

#endif