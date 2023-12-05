#include<iostream>
#include <unistd.h>
#include "host.hpp"
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
void Wolf::init_() const {

    pid_t goat_pid = fork();
    
    if (goat_pid < 0) {
        exit(EXIT_FAILURE);
    }
    else if (goat_pid) {
        //Pipe p = Pipe(false);
    }
    else {
        //goat init    
    }
}
 
void Wolf::run() const {

}


auto main(int argc, char** argv) -> int {
    Pipe p = Pipe();
    Wolf& w = Wolf::get_instance();
    return 0;
}