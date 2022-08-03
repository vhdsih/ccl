#include "ccl.h"

struct Data {
    Data() {
        std::cout << "create\n";
    }
    ~Data() {
        std::cout << "release\n";
    }
};

void runner(std::unique_ptr<Data> data) {
    (void)data;
}

int main() {
    ccl::thread_pool_t pool(1);
    /*
    for (int i = 0; i < 10; ++i) {
        auto ptr = new Data;
        auto data = std::unique_ptr<Data>(ptr);
        pool.push([&](std::unique_ptr<Data> data) {
            runner(std::move(data));
        }, std::move(data));
    }
     */

    return 0;
}
