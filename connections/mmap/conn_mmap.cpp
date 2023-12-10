#include "conn_mmap.hpp"


Mmap::Mmap(size_t type_size) : sem_(1) {
    mem_size_ = 1*type_size; //memory for one integer
    shared_data_ = static_cast<int*>(mmap(NULL, mem_size_, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    if (shared_data_ == MAP_FAILED) {
        std::cerr << "mmap failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Mmap::read(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if (count != mem_size_) {
        std::cerr << "Error: Reading more data than available in shared memory\n";
        sem_.post();
        exit(EXIT_FAILURE);
    }
    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(buf, shared_data_, count);
        sem_.post();
        return true;
    } else {
        std::cerr << "Error(read): Invalid pointers for memcpy\n";
        sem_.post();
        exit(EXIT_FAILURE);
    }
}

bool Mmap::write(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);

    if (count != mem_size_) {
        std::cerr << "Error: Writing more data than available in shared memory\n";
        sem_.post();
        exit(EXIT_FAILURE);
    }

    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(shared_data_, buf, count);
        sem_.post();
        return true;
    } else {
        std::cerr << "Error(write): Invalid pointers for memcpy\n";
        sem_.post();
        exit(EXIT_FAILURE);
    }
}

Mmap::~Mmap() {
    std::cerr<< "destructor mmap called" << std::endl;
    sem_.~TimedSemaphore();
    if (munmap(shared_data_, mem_size_) == -1) {
        std::cerr << "Error(mmap destructor): munmap failed" << std::endl;
    }

}