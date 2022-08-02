#include "ccl.h"

struct Ret {
    int a;
    int b;
    std::string c;
};

thread_local int tl = 0;

int case4() {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;

    sleep(4);
    return 2;
}

int case5(int a) {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;

    sleep(4);
    return 2 + a;
}

int case6(int a, int b) {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;

    sleep(4);
    return 2 + a + b;
}

std::string case7(int a, int b, const std::string &c) {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;
    sleep(4);
    return std::to_string(2 + a + b) + " " + c;
}

Ret case8() {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;
    sleep(4);
    Ret ret{1, 2, "abc"};
    return ret;
}

void case9() {
    tl += 1;
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << " " << tl << std::endl;
    sleep(4);
}

int main() {
    ccl::thread_pool_t pool(3);

    auto f1 = pool.push([&]() {
        tl += 1;
        std::thread::id this_id = std::this_thread::get_id();

        std::cout << this_id << " " << tl << std::endl;
        sleep(10);
        return 13;
    });

    auto f2 = pool.push(
        [&](int a, double b) {
            tl += 1;
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << this_id << " " << tl << std::endl;
            return 14 + a + b;
        },
        2, 2.2);

    auto f3 = pool.push([&]() {
        tl += 1;
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << this_id << " " << tl << std::endl;
        sleep(4);
        return 13;
    });

    auto x = ccl::make_unique<int>(100);
    pool.push([](std::unique_ptr<int> p) {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << this_id << " xxxxxxxxxxxxxxxxxxx " << *p << std::endl;
        return 13;
    }, std::move(x));

    auto f4 = pool.push(case4);

    auto f5 = pool.push(case5, 101);

    auto f6 = pool.push(case6, 101, 102);

    auto f7 = pool.push(case7, 100, 101, " what ");

    auto f8 = pool.push(case8);

    pool.push(case9);

    sleep(20);
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
