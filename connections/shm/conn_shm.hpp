class Shm : public Conn {
    public:
    Shm(size_t type_size);
    Shm(bool is_parent){};
    bool read(void *buf, size_t count);
    bool write(void *buf, size_t count);
    ~Shm();
private:
    TimedSemaphore sem_;
    int *shared_data_ = nullptr;
    size_t mem_size_;
};