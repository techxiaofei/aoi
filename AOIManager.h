#pragma once

#include <stdint.h>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Entity.h"
#include "AOIGrid.h"

#define FACTOR 100  //x*FACTOR+y

#define ENTITY_TYPE_NPC 1
#define ENTITY_TYPE_PC 2
#define ENTITY_TYPE_ENEMY 3

namespace AOI{

class AOIManager{

public:
    typedef std::function<void(const Entity& entity, uint64_t entity_id)> GridsFunctionCB;

    AOIManager(int width, int height, int gx, int gy): width_(width),height_(height),gx_(gx),gy_(gy){
        InitGrids();
    }

    ~AOIManager(){}

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
    int width_;  //场景总宽度
    int height_;  //场景总长度
    int gx_;  //每个格子的宽度
    int gy_;  //每个格子的长度

    typedef std::unordered_map<int, std::unordered_set<int>> GRIDS;  //gid=>(idlist)
    GRIDS grids_;

    typedef std::unordered_map<int, std::shared_ptr<AOIGrid>> UnorderedMap;
    UnorderedMap map_;

    GridsFunctionCB enterMessageCB_;
    GridsFunctionCB leaveMessageCB_;
    GridsFunctionCB moveMessageCB_;

private:
    void InitGrids();
    void EnterBroadcast(const Entity& entity, int x, int y);
    void LeaveBroadcast(const Entity& entity, int x, int y);
    void MoveBroadcast(const Entity& entity, int x, int y);

    int GetGid(int x, int y) const;
    int GxGy2Gid(int g_x, int g_y) const;

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