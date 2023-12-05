#ifndef HOST_H
#define HOST_H

class Wolf {
private:
    Wolf() = default;
    Wolf(const Wolf&) = delete;
    Wolf(Wolf&&) = delete;
    Wolf& operator=(const Wolf&) = delete;
    Wolf& operator=(Wolf&&) = delete;
    void init_() const;
public:
    void run() const;
    static Wolf& get_instance() {
        static Wolf instance = Wolf();
        return instance;
    }
    ~Wolf() = default;
};

class Goat {
private:
    Goat() = default;
    Goat(const Goat&) = delete;
    Goat(Goat&&) = delete;
    Goat& operator=(const Goat&) = delete;
    Goat& operator=(Goat&&) = delete;
public:
    static Goat& get_instance() {
        static Goat instance = Goat();
        return instance;
    }
    ~Goat() = default;
};
#endif