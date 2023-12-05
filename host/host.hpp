#include <optional>
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
#include "../players/players.hpp"
#ifndef HOST_H
#define HOST_H

template <typename Connection>
class Host {
protected:
    Host() = default;
private:    
    Host(const Host&) = delete;
    Host(Host&&) = delete;
    Host& operator=(const Host&) = delete;
    Host& operator=(Host&&) = delete;
    void manage_game() {
        while(true) {
            //game logic here
        }
    };
    void connect() {
        if constexpr (std::is_same_v<Connection, Mmap>) {
            //call mmap constructor here
        }
        else if constexpr (std::is_same_v<Connection, Pipe>) {
            w_g_connection_ = Pipe(true);
            g_w_connection_ = Pipe(false);
        }
        else {
            //call shm consructor here
        }
    }
    Connection w_g_connection_;
    std::optional<Connection> g_w_connection_; //for pipe
public:
    void run(const std::string& executable_name) {
        pid_t goat_pid = fork();
//TODO: signal handling    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else if (goat_pid) /*parent*/{
            connect();
            Wolf& w = Player<Wolf>::get_instance(); 
            //manage game process
        }
        else {
        }
    };
    static Host& get_instance() {
        static Host<Connection> instance;
        return instance;
    }
    ~Host() = default;
};
#endif