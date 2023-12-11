#include "../conn.h"
#include "../../timed_ops/timed_ops.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>  
 class Shm : public Conn {
public:
    Shm(size_t mem_size);
    bool read(void *buf, size_t count);
    bool write(void *buf, size_t count);
    ~Shm();
private:
    TimedSemaphore sem_;
    const char* SHM_NAME = "/shm";
    int *shared_data_ = nullptr;
    size_t mem_size_;
    int shm_fd_;
};