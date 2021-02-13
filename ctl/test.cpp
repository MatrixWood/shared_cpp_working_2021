#include "ctl.h"
#include <string>
#include <iostream>

int main() {
    std::string prefix_str1 = "prefix_str1";
    std::string prefix_str1_str2 = "prefix_str1_str2";
    std::string str1;
    std::string prefix;

    //std::string str3 = JOIN("prefix", str2);

    std::cout << JOIN(prefix, str1) << std::endl;
    std::cout << JOIN(JOIN(prefix, str1), str2) << std::endl;
    return 0;
}