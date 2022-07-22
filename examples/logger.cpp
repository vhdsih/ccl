/*
 * @Author: vhdsih
 */

#include "ccl.h"

int main() {
    const char *log_path = "./logs/ccl";

    ccl::glog_log_init("ccl", log_path);

    // write info
    ccl_glog_info << ccl::version();
    ccl_glog_info << "hello";
    ccl_glog_info << "world";

    // warn
    ccl_glog_warn << "this is a warn";

    // error
    ccl_glog_error << "this is an error";

    // log with if
    ccl_glog(true) << "this is an info by ccl_log";
    ccl_glog(false) << "this is an error by ccl_log";

    ccl_glog_error_if(1 > 0) << "write error if 1 > 0";
    ccl_glog_error_if(1 < 0) << "write error if 1 < 0";

    // fatal
    ccl_glog_fatal << "terminate";

    ccl::glog_log_clean();

    return 0;
}