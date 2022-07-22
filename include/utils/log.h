/*
 * @Author: vhdsih
 * @Date: 2022-06-19 22:11:06
 */

#ifndef CCL_UTILS_LOG_H
#define CCL_UTILS_LOG_H

#include "glog/logging.h"
#include "path.h"

namespace ccl {

inline void glog_log_init(const char* target, const char* dir) {
    try_mkdir(dir);
    FLAGS_log_dir                   = dir;
    FLAGS_logbufsecs                = 0;
    FLAGS_max_log_size              = 512;
    FLAGS_colorlogtostderr          = true;
    FLAGS_stop_logging_if_full_disk = true;
    // google::SetLogFilenameExtension("log");
    google::InitGoogleLogging(target);
}

inline void glog_log_clean() {
    google::ShutdownGoogleLogging();
    google::ShutDownCommandLineFlags();
}

} // namespace ccl

#define ccl_glog(is_info) ((is_info) ? LOG(INFO) : LOG(ERROR))

#define ccl_glog_info LOG(INFO)
#define ccl_glog_info_if(rule) LOG_IF(INFO, rule)
#define ccl_glog_warn LOG(WARNING)
#define ccl_glog_warn_if(rule) LOG(WARNING, rule)
#define ccl_glog_error LOG(ERROR)
#define ccl_glog_error_if(rule) LOG_IF(ERROR, rule)
#define ccl_glog_fatal LOG(FATAL)
#define ccl_glog_fatal_if(rule) LOG_IF(FATAL, rule)

#endif
