#include "conn_pipe.hpp"
#include <unistd.h>
#include <iostream>

//TODO: error processing add here
Pipe::Pipe(bool is_parent) : is_parent_process_(is_parent) {
    if (pipe(pipe_descriptors_) == -1) {
        std::cout << "error creation pipe" << std::endl;
    }
    if (sem_init(&semaphore_, 1, 1) == -1) {
        std::cout << "semaphore initialization error" << std::endl;
    }
}

bool Pipe::read(void *buf, size_t count) {
    sem_wait(&semaphore_);
    
    if (is_parent_process_) {
        close(pipe_descriptors_[WRITE_END]);
    } else {
        close(pipe_descriptors_[WRITE_END]);
    }

    ssize_t bytes_read = ::read(pipe_descriptors_[READ_END], buf, count);

    
    sem_post(&semaphore_);

    return bytes_read != -1;
}

bool Pipe::write(void *buf, size_t count) {
    sem_wait(&semaphore_);

    if (is_parent_process_) {
        close(pipe_descriptors_[READ_END]);
    } else {
        close(pipe_descriptors_[READ_END]);
    }

    ssize_t bytes_written = ::write(pipe_descriptors_[WRITE_END], buf, count);

    sem_post(&semaphore_);

    return bytes_written != -1;
};

Pipe::~Pipe() {
    sem_destroy(&semaphore_);

    close(pipe_descriptors_[READ_END]);
    close(pipe_descriptors_[WRITE_END]);
}