#pragma once

#include <stdint.h>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "Entity.h"

#define GX 8  //每个格子的宽
#define GY 8  //每个格子的高
#define FACTOR 100  //x*FACTOR+y

namespace AOI{

class AOIGrid{

public:
    typedef std::function<void(const Entity& entity, uint64_t entity_id)> GridsFunctionCB;

    AOIGrid(int width, int height){
        width_ = width;
        height_ = height;

        InitGrids();
    }

    ~AOIGrid(){}

public:
    //aoi api
    void Enter(const Entity& entity);
    void Enter(uint64_t id, int gid);
    void Leave(const Entity& entity);
    void Leave(uint64_t id, int gid);
    void Move(const Entity& entity, int x, int y);

    int width() const{return width_;}
    int height() const{return height_;}

public:
    void setEnterMessageCB(const GridsFunctionCB& cb){enterMessageCB_ = cb;}
    void setLeaveMessageCB(const GridsFunctionCB& cb){leaveMessageCB_ = cb;}
    void setMoveMessageCB(const GridsFunctionCB& cb){moveMessageCB_ = cb;}

private:
    int width_;  //scene width
    int height_;  //scene height

    typedef std::unordered_map<int, std::unordered_set<int>> GRIDS;  //gid=>(idlist)
    GRIDS grids_;

    GridsFunctionCB enterMessageCB_;
    GridsFunctionCB leaveMessageCB_;
    GridsFunctionCB moveMessageCB_;

private:
    void InitGrids();
    void EnterBroadcast(const Entity& entity, int x, int y);
    void LeaveBroadcast(const Entity& entity, int x, int y);
    void MoveBroadcast(const Entity& entity, int x, int y);

    int GetGid(int x, int y) const;
    int GxGy2Gid(int gx, int gy) const;

    /*
    * x,y视野内的Entity，先找到格子
    * */
    void ViewEntities(std::unordered_set<uint64_t>& entities, int x, int y);

    /*
    * x,y视野内的格子，就是九宫格
    * */
    void ViewGrids(std::unordered_set<int>& view_grids, int x, int y);

    /*
    * gid格子内的Entity
    * */
    void GridEntities(std::unordered_set<uint64_t>& entities, int gid);

    /*
    * 跨格子移动时计算
    */
    void move_cross_grid(const Entity& entity, int x, int y);

private:
    //TODO,设置callback给外部函数
    void leave_message(const Entity& entity, std::unordered_set<int> leave_grids);
    void enter_message(const Entity& entity, std::unordered_set<int> enter_grids);
    void move_message(const Entity& entity, std::unordered_set<int> move_grids);

};

}