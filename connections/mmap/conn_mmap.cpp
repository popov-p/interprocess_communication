#include "conn_mmap.hpp"


Mmap::Mmap(size_t mem_size) : sem_(1), mem_size_(mem_size) {
    shared_data_ = static_cast<int*>(mmap(NULL, mem_size_, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    if (shared_data_ == MAP_FAILED) {
        syslog(LOG_ERR,"Mmap constructor failed");
        exit(EXIT_FAILURE);
    }
}

bool Mmap::read(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if (count != mem_size_) {
        syslog(LOG_ERR, "Mmap: Reading more data than available in shared memory");
        sem_.post();
        return false;
    }
    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(buf, shared_data_, count);
        sem_.post();
        return true;
    } else {
        syslog(LOG_ERR, "Mmap read: Invalid pointers for memcpy");
        sem_.post();
        return false;
    }
}

bool Mmap::write(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);

    if (count != mem_size_) {
        syslog(LOG_ERR, "Mmap read: Invalid pointers for memcpy");
        sem_.post();
        return false;
    }

    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(shared_data_, buf, count);
        sem_.post();
        return true;
    } else {
        syslog(LOG_ERR, "Mmap write: Invalid pointers for memcpy");
        sem_.post();
        return false;
    }
}

Mmap::~Mmap() {
    syslog(LOG_INFO, "Destructor mmap called");
    sem_.~TimedSemaphore();
    if (munmap(shared_data_, mem_size_) == -1) {
        syslog(LOG_ERR, "Error(mmap destructor): munmap failed");
    }
    closelog();
}