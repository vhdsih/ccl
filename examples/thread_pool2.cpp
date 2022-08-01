#include "ccl.h"



void f() {
    thread_local int tl = 0;
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;
}


int main() {
    ccl::thread_pool_t pool(3);
    for (int i = 0; i < 1000; ++i) {
        // pool.push(f);
        pool.push([]() {
        thread_local int *tl = new int(100);
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << this_id << " " << tl << std::endl;

        });
    }
    sleep(100);

    return 0;
}
