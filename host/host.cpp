#include<iostream>
#include <unistd.h>
#include "host.hpp"


auto main(int argc, char** argv) -> int {
    auto& w = Wolf<Pipe>::get_instance();
    w.run(argv[0]);
    return 0;
}