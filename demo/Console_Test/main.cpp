#include <iostream>
#include "common.hpp"

int main()
{
    auto ret = test_memory_management();
    if (ret == 0) std::cout << "========== test success ==========\n";
    else std::cerr << "########## test fail ##########\n";
    return 0;
}
