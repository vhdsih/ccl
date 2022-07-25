#include "ccl.h"

using task_t = std::function<void(void)>;

template <typename Fn, typename... Args>
auto push(Fn &&fn, Args &&...args)
    -> std::future<decltype(fn(args...))> {
    auto pck =
        std::make_shared<std::packaged_task<decltype(fn(args...))(void)>>(
            [&]() { return fn(args...); });

    auto task = std::make_shared<task_t>(
        std::function<void(void)>([pck]() -> void { (*pck)(); }));
    auto th = new std::thread([task]() { (*task)(); });
    (void)th;
    return pck->get_future();
}

int zero() {
    return 13;
}


int one(int a) {
    return 13 + a ;
}

int two(int a, int b) {
    return 13 + a + b;
}

int main() {
    auto x0 = push(zero);
    auto val0 = x0.get();
    std::cout << "val is " << val0 << std::endl;

    auto x1 = push(one, 100);
    auto val1 = x1.get();
    std::cout << "val is " << val1 << std::endl;

    auto x2 = push(two, 100, 200);
    auto val2 = x2.get();
    std::cout << "val is " << val2 << std::endl;

    return 0;
}