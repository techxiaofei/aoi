/**
 * 传入AOI模块最简单的Entity，包括id，x，y，type属性
 */
#pragma once
#include <stdint.h>
namespace AOI{

class Entity{

public:
		Entity(uint64_t id, int type) :id_(id), x_(0), y_(0), type_(type) {}
		Entity(uint64_t id, int x, int y, int type) :id_(id), x_(x), y_(y), type_(type) {}
		Entity() = delete;
        Entity(const Entity& rhs) = delete;
		Entity& operator=(const Entity& rhs) = delete;
		~Entity() {};

public:
    //API
    uint64_t id() const { return id_; }
    int x() const { return x_; }
    void set_x(int x) { x_ = x; }
    int y() const { return y_; }
    void set_y(int y) { y_ = y; }
    int type() const {return type_;}


private:
    uint64_t id_;
    int x_;
    int y_;
    int type_;

};

}