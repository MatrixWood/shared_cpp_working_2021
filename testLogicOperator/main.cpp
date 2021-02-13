#include "logic.h"
#include <iostream>

int main() {
    std::cout << std::boolalpha << all<true, true, true>::type::value << std::endl;
    std::cout << std::boolalpha << all<true, true, false>::type::value << std::endl;
    std::cout << std::boolalpha << all<true, false, false>::type::value << std::endl;
    std::cout << std::boolalpha << all<false, false, false>::type::value << std::endl;

    std::cout << std::boolalpha << any<true, true, true>::type::value << std::endl;
    std::cout << std::boolalpha << any<true, true, false>::type::value << std::endl;
    std::cout << std::boolalpha << any<true, false, false>::type::value << std::endl;
    std::cout << std::boolalpha << any<false, false, false>::type::value << std::endl;
}