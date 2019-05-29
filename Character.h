#pragma once
#include <stdint.h>
namespace AOI{

class Character{
public:
    Character(uint64_t id, int x, int y):id_(id),x_(x),y_(y){}

public:
    uint64_t id() const { return id_; }
    int x() const { return x_; }
    void set_x(int x) { x_ = x; }
    int y() const { return y_; }
    void set_y(int y) { y_ = y; }

private:
    uint64_t id_;
    int x_;
    int y_;
};
}
