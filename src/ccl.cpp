/*
 * @Author: vhdsih
 * @Date: 2022-06-18 22:41:35
 */

#include "ccl.h"

#include <iostream>

namespace ccl {

void hello() { std::cout << "Hello, ccl!" << std::endl; }

const char *version() { return CCL_VERSION; }

} // namespace ccl
