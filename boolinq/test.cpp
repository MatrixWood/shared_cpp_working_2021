#include "boolinq.h"

#include <vector>
#include <string>

auto main() -> int {
    std::vector<int> src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3);

    auto range = from(src);
    return 0;
}