#include<iostream>
#include "host.hpp"
#include <filesystem>

namespace fs = std::filesystem;

auto main(int argc, char** argv) -> int {
    //lol kek 
    std::string input;
    while(true) {
        std::cout << "Do you want to play as wolf (manually input wolf`s numbers) [y/n]?" << std::endl;
        std::cin >> input;
        if(input == "y" || input == "n") {
            break;
        }
    }
    fs::path filename = fs::absolute(fs::path(argv[0])).filename();  
    if(filename == "host_pipe") {
        auto& h = Host<Pipe, std::unique_ptr>::get_instance();
        h.set_keyboard_input_flag(input == "y" ? true : false);
        h.run(argv[0]);
    }
    // else if (argv[0] =="host_shm") {
    //     auto& h = Host<Shm>::get_instance();
    //     h.set_keyboard_input_flag(input == "y" ? true : false);
    //     h.run(argv[0]);
    // }
    else if (filename == "host_mmap") {
        auto& h = Host<Mmap, std::shared_ptr>::get_instance();
        h.set_keyboard_input_flag(input == "y" ? true : false);
        h.run(argv[0]);
    }
    // else {
    //     std::cout << "Wrong executable name";
    // }
    
    return 0;
}