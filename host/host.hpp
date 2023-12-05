#include <optional>
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
#ifndef HOST_H
#define HOST_H

template <typename Connection>
class Wolf {
private:
    Wolf() = default;
    Wolf(const Wolf&) = delete;
    Wolf(Wolf&&) = delete;
    Wolf& operator=(const Wolf&) = delete;
    Wolf& operator=(Wolf&&) = delete;
    void connect() {
        if constexpr (std::is_same_v<Connection, Mmap>) {
            //call mmap constructor here
        }
        else if constexpr (std::is_same_v<Connection, Pipe>) {
            w_g_connection_(true);
        }
        else {
            //call shm consructor here
        }
    }
    void run() {
        pid_t goat_pid = fork();
//TODO: signal handling    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else if (goat_pid) {
            connect();
            //manage game process
        }
        else {
            Goat
        }
    };
    Connection w_g_connection_;
    std::optional<Connection> g_w_connection_; //for pipe
public:
    void run(const std::string& executable_name) {
        
    };
    static Wolf& get_instance() {
        static Wolf<Connection> instance;
        return instance;
    }
    ~Wolf() = default;
};

template <typename Connection>
class Goat {
private:
    Goat();
    Goat(const Goat&) = delete;
    Goat(Goat&&) = delete;
    Goat& operator=(const Goat&) = delete;
    Goat& operator=(Goat&&) = delete;
    bool status_;
public:
    static Goat& get_instance() {
        static Goat instance = Goat();
        return instance;
    }
    ~Goat() = default;
};
#endif