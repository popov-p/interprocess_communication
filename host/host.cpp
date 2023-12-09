#include<iostream>
#include "host.hpp"


auto main(int argc, char** argv) -> int {
    //lol kek
    auto& h = Host<Pipe>::get_instance();
    h.run(argv[0]);
    return 0;
}