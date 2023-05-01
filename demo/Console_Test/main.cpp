#include <iostream>
#include "common.hpp"

int main()
{
    auto ret = test_qdebug_1();
    if (ret == 0) std::cout << "========== test success ==========\n";
    else std::cerr << "########## test fail ##########\n";
    return 0;
}
