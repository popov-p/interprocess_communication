#include <optional>
#include <memory>
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
#include "../players/players.hpp"
#ifndef HOST_H
#define HOST_H

template <typename Connection>
class Host {
    Host(const Host&) = delete;
    Host(Host&&) = delete;
    Host& operator=(const Host&) = delete;
    Host& operator=(Host&&) = delete;
private:
    Host() = default;    
    void manage_game(const pid_t& pid) {
        connect();
        switch (pid) {
            default:
                while(true) {
                    Wolf& wolf = Wolf::get_instance();
                    int current_wolf_num = wolf.throw_number();
                    int current_goat_num;
                    int dead_times = 0;
                    //g_w_connection_.value().read(&current_goat_num, sizeof(int));
                    if ((dead_times == 0) && (abs(current_wolf_num - current_goat_num)) <= 70) {
                        bool status = Status::hidden;
                        //w_g_connection_.write(&status, sizeof(bool));
                    }
                    else if ((dead_times == 1) && (abs(current_wolf_num - current_goat_num) <= 20)) {
                        bool status = Status::hidden;
                        //w_g_connection_.write(&status, sizeof(bool)); //reincarnation
                        dead_times--;
                    }
                    else {
                        bool status = Status::dead;
                        //w_g_connection_.write(&status, sizeof(bool));
                        dead_times++;
                        if(dead_times == 2) {
                            //stop game, send sigkill
                        }
                    }

                }
            case 0: // child 
                while(true) {
                    Goat& goat = Goat::get_instance();
                    int current_goat_num = goat.throw_number();
                    g_w_connection_.value().write(&current_goat_num, sizeof(int));
                }
            
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
    int round_counter_;
public:
    void run(const std::string& executable_name) {
        const pid_t & goat_pid = fork();
//TODO: signal handling    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else {
            manage_game(goat_pid);
        }
    };
    static Host& get_instance() {
        static Host<Connection> instance;
        return instance;
    }
    ~Host() = default;
};
#endif