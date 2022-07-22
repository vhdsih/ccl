//
// Created by vhdsih on 2022/7/23.
//

#include "ccl.h"

int main() {
    const char *path = "./a/b/c";
    std::cout << path << " exists? " << ccl::path_exits(path) << std::endl;
    std::cout << path << " is dir? " << ccl::path_is_dir(path) << std::endl;

    std::cout << "try make dir " << ccl::try_mkdir(path) << std::endl;

    std::cout << path << " exists? " << ccl::path_exits(path) << std::endl;
    std::cout << path << " is dir? " << ccl::path_is_dir(path) << std::endl;

    return 0;
}