/*
 * @Author: vhdsih
 * @Date: 2022-07-07 15:57:07
 */

#include "ccl.h"

int main() {
    ccl_glog_info << ccl::version();
    ccl_glog_info << "hello";

    ccl_glog(false) << "abc";

    return 0;
}