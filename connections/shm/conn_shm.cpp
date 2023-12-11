#include "conn_shm.hpp"

Shm::Shm(size_t mem_size) : mem_size_(mem_size) {
    shm_fd_ = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd_ == -1) {
        syslog(LOG_ERR, "Failed creating/opening shared memory");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd_, mem_size_);
    shared_data_ = static_cast<int*>(mmap(NULL, mem_size_, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (shared_data_ == MAP_FAILED) {
        syslog(LOG_ERR, "Failed mapping shared memory to process");
        exit(EXIT_FAILURE);
    }
}

bool Shm::read(void *buf, size_t count) {

    sem_.wait(TIMEOUT_SECONDS);
    if(timer(shm_fd_, TIMEOUT_SECONDS, OperationType::Read)) {
    if (count != mem_size_) {
        syslog(LOG_ERR, "Reading more data than available in shared memory");
        sem_.post();
        return false;
    }
    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(buf, shared_data_, count);
        sem_.post();
        return true;
    } else {
        syslog(LOG_ERR, "Read: Invalid pointers for memcpy");
        sem_.post();
        return false;
    }
    } else {
        sem_.post();
        return false;
    }
}

bool Shm::write(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if(timer(shm_fd_, TIMEOUT_SECONDS, OperationType::Read)) {
    if (count != mem_size_) {
        syslog(LOG_ERR, "Shm Writing more data than available in shared memory");
        sem_.post();
        return false;
    }

    if (*shared_data_ != ConnStatus::INACCESSIBLE && buf != nullptr) {
        std::memcpy(shared_data_, buf, count);
        sem_.post();
        return true;
    } else {
        syslog(LOG_ERR, "Shm write: Invalid pointers for memcpy");
        sem_.post();
        return false;
    }
    } else {
        sem_.post();
        return false;
    }
}

Shm::~Shm() {
    syslog(LOG_INFO, "Shm destructor called");
    sem_.~TimedSemaphore();
    if (shm_unlink(SHM_NAME) == -1) {
        syslog(LOG_ERR, "Shm Failed unlinking shared memory");
    }
    if (munmap(shared_data_, sizeof(int)) == -1) {
        syslog(LOG_ERR, "Shm Failed unmapping shared memory");
    }
    if (close(shm_fd_) == -1) {
        syslog(LOG_ERR, "Shm Failed closing file descriptor");
    }
    closelog();
}