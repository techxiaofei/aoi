#pragma once
#include <stdint.h>

#define ENEMY_STATE_QUIET 0
#define ENEMY_STATE_MOVE 1

namespace AOI{

class Enemy{
public:
    Enemy(uint64_t id, int x, int y):id_(id),x_(x),y_(y){}

public:
    

private:
    uint64_t id_;
    int x_;
    int y_;
};

}