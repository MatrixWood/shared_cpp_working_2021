#include "boolinq.h"

#include <vector>
#include <string>
#include <iostream>

auto main() -> int {
    std::vector<int> src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3);

    auto range = from(src);
    std::cout << std::boolalpha << range.any() << std::endl;
    std::cout << std::boolalpha << range.any([](int a){return a == 1;}) << std::endl;
    std::cout << std::boolalpha << range.all([](int a){return a > 0;}) << std::endl;
    std::cout << std::boolalpha << range.all([](int a){return a > 4;}) << std::endl;
    std::cout << std::boolalpha << range.contains(1) << std::endl;
    std::cout << std::boolalpha << range.contains(4) << std::endl;

    std::cout << std::boolalpha << range.where([](int a){return a % 2 == 1;})
                                   .any([](int a){return a == 2;}) << std::endl;

    int num = 0;
    from(src).for_each([&](int a){num += a;});
    std::cout << num << std::endl;
    return 0;
}