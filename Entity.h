#pragma once
#include <stdint.h>
namespace AOI{

class Entity{

public:
		Entity(uint64_t id) :id_(id), x_(0), y_(0) {}
		Entity(uint64_t id, int x, int y) :id_(id), x_(x), y_(y) {}
		Entity() = delete;
		Entity& operator=(const Entity& rhs) = delete;
		~Entity() {};

public:
    //API
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