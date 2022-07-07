/*
 * @Author: vhdsih
 * @Date: 2022-06-19 22:11:06
 */

#ifndef CCL_UTILS_LOG_H
#define CCL_UTILS_LOG_H

#include <ctime>

#include "glog/logging.h"

namespace ccl {

inline void glog_log_init(const char* target, const char* dir) {
    google::InitGoogleLogging(target);
    FLAGS_log_dir = dir;
}

inline void glog_log_clean() { google::ShutdownGoogleLogging(); }

} // namespace ccl

#define ccl_glog(is_info) ((is_info) ? LOG(INFO) : LOG(ERROR))

#define ccl_glog_info LOG(INFO)
#define ccl_glog_warn LOG(WARNING)
#define ccl_glog_error LOG(ERROR)
#define ccl_glog_fatal LOG(FATAL)

#endif
