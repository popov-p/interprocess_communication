#include "conn_pipe.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
//TODO: error processing add here
Pipe::Pipe(bool is_parent) : is_parent_process_(is_parent) {
    if (pipe(pipe_descriptors_) == -1) {
        std::cerr << "Pipe initialization error: " << strerror(errno) << std::endl;
    }
    if (sem_init(&semaphore_, 1, 1) == -1) {
        std::cout << "semaphore initialization error" << std::endl;
    }
}

bool Pipe::read(void *buf, size_t count) {
    sem_wait(&semaphore_);

    ssize_t bytes_read = ::read(pipe_descriptors_[PipeEnd::READ_END], buf, count);
    if (bytes_read == -1) {
        std::cerr << "Read error: " << strerror(errno) << std::endl;
    }

    sem_post(&semaphore_);

    return bytes_read != -1;
}

bool Pipe::write(void *buf, size_t count) {
    sem_wait(&semaphore_);
    ssize_t bytes_written = ::write(pipe_descriptors_[PipeEnd::WRITE_END], reinterpret_cast<int*>(buf), count);
    if (bytes_written == -1) {
        std::cerr << "Write error: " << strerror(errno) << std::endl;
    }
    sem_post(&semaphore_);

    return bytes_written != -1;
};

Pipe::~Pipe() {
    //sem_destroy(&semaphore_);
    //close(pipe_descriptors_[READ_END]);
    //close(pipe_descriptors_[WRITE_END]);
}