#include <conn_pipe.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h>
//TODO: error processing add here


Pipe::Pipe() : sem_(1) {
    if (pipe(pipe_descriptors_) == -1) {
        std::cerr << "Pipe initialization error: " << strerror(errno) << std::endl;
    }
}

bool Pipe::read(void *buf, size_t count) {

    sem_.wait(TIMEOUT_SECONDS);
    if (timed_io(pipe_descriptors_[PipeEnd::READ_END], buf, count, TIMEOUT_SECONDS, OperationType::Read)) {
        //std::cout << "Read successful" << std::endl;
        sem_.post();
        return true;
    } else {
        std::cerr << "Read operation failed or timed out" << std::endl;
        sem_.post();
        return false;
        //exit(EXIT_FAILURE);
    }
    //sem_.post();
    //return true;
}

bool Pipe::write(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if (timed_io(pipe_descriptors_[PipeEnd::WRITE_END], buf, count, TIMEOUT_SECONDS, OperationType::Write)) {
        //std::cout << "Write successful" << std::endl;
        sem_.post();
        return true;
    } else {
        std::cerr << "Write operation failed or timed out" << std::endl;
        sem_.post();
        return false;
        //exit(EXIT_FAILURE);
    }
    //sem_.post();
    //return true;
};

Pipe::~Pipe() {
    std::cerr<< "destructor pipe called" << std::endl;
    sem_.~TimedSemaphore();
    close(pipe_descriptors_[READ_END]);
    close(pipe_descriptors_[WRITE_END]);
}