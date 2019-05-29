#pragma once

#include <unordered_set>
#include <unordered_map>
#include "Entity.h"

namespace AOI{

class AOIGrid{
public:
    AOIGrid(int grid_id):grid_id_(grid_id){

    }

    ~AOIGrid(){}

    void Enter(int entity_id, int type);
    void Leave(int entity_id, int type);

    /**
     * 返回本格子内的entity
     */
    void GridEntities(std::unordered_set<uint64_t>& entities);

private:
    int grid_id_;
    typedef std::unordered_set<int> UnorderSet;
    UnorderSet characters_;
    UnorderSet npcs_;
    UnorderSet enemies_; 

};

}