/*
 * @Author: vhdsih
 * @Date: 2022-07-07 15:57:07
 */

#include "ccl.h"

int main() {
    const char *log_path = "./logs/mytest/1/2/3";

    ccl::glog_log_init("ccl", log_path);

    ccl_glog_info << ccl::version();
    ccl_glog_info << "hello";
    ccl_glog_info << "hello word";

    ccl_glog_info << "word";
    ccl_glog_warn << "warn";
    ccl_glog_error << "word";
    ccl_glog_error_if(true) << "word if true";
    ccl_glog_error_if(false) << "word if false";

    ccl_glog(false) << "abc";

    ccl::glog_log_clean();

    return 0;
}