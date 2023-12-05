#include<iostream>
#include <unistd.h>
#include "host.hpp"
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
 


auto main(int argc, char** argv) -> int {
    
    auto& w = Wolf<Pipe>::get_instance();
    return 0;
}