#include "ccl.h"


int main() {
    ccl::thread_pools_t pools(4, 1);

    for (size_t i = 0; i < 100; ++i) {
        pools.push(i % 4, [&](int x) {
            sleep(1);
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << this_id << " " << x << "\n";
        }, i);
    }

    return 0;
}
