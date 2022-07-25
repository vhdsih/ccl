#include "ccl.h"

struct Ret {
    int a;
    int b;
    std::string c;
};

int case4() {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 4 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 4 yes " << this_id << std::endl;
    return 2;
}

int case5(int a) {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 5 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 5 yes " << this_id << std::endl;
    return 2 + a;
}

int case6(int a, int b) {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 6 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 6 yes " << this_id << std::endl;
    return 2 + a + b;
}

std::string case7(int a, int b, const std::string &c) {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 7 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 7 yes " << this_id << std::endl;
    return std::to_string(2 + a + b) + " " + c;
}

Ret case8() {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 8 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 8 yes " << this_id << std::endl;

    Ret ret{1, 2, "abc"};
    return ret;
}

void case9() {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "case 9 run " << this_id << std::endl;
    sleep(4);
    std::cout << "case 9 yes " << this_id << std::endl;
}

int main() {
    ccl::thread_pool_t pool(4);

    auto f1 = pool.push([]() {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "case 1 run " << this_id << std::endl;
        sleep(10);
        std::cout << "case 1 yes " << this_id << std::endl;
        return 13;
    });

    auto f2 = pool.push(
        [](int a, double b) {
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << "case 2 run " << this_id << std::endl;
            std::cout << "case 2 yes " << this_id << std::endl;
            return 14 + a + b;
        },
        2, 2.2);

    auto f3 = pool.push([]() {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "case 3 run " << this_id << std::endl;
        sleep(4);
        std::cout << "case 3 yes " << this_id << std::endl;
        return 13;
    });

    auto f4 = pool.push(case4);

    auto f5 = pool.push(case5, 101);

    auto f6 = pool.push(case6, 101, 102);

    auto f7 = pool.push(case7, 100, 101, " what ");

    auto f8 = pool.push(case8);

    pool.push(case9);

    std::cout << "idle " << pool.idle() << "f1 " << f1.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f2 " << f2.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f3 " << f3.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f4 " << f4.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f5 " << f5.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f6 " << f6.get() << std::endl;
    std::cout << "idle " << pool.idle() << "f7 " << f7.get() << std::endl;
    auto val = f8.get();
    std::cout << "idle " << pool.idle() << "f4 " << val.a << " " << val.b << " "
              << val.c << std::endl;


    pool.resize(8);
    std::cout << "::idle " << pool.idle() << " " << pool.size()  << std::endl;

    sleep(10);
    std::cout << "::idle " << pool.idle() << " " << pool.size()  << std::endl;

    return 0;
}