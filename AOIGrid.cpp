#include <stdio.h>
#include "AOIGrid.h"

namespace AOI{

void AOIGrid::Enter(int entity_id, int type){
    characters_.insert(entity_id);
    printf("grid %d add id=%d\n", grid_id_, entity_id);
}

void AOIGrid::Leave(int entity_id, int type){
    characters_.erase(entity_id);

}

void AOIGrid::GridEntities(std::unordered_set<uint64_t>& entities){
    /*
    auto grid_it = grids_.find(gid);
    if (grid_it != grids_.end()){
        for (auto grid_entity_id : grid_it->second){
            entities.insert(grid_entity_id);
        }
    }
    */
   for (auto entity_id : characters_){
       entities.insert(entity_id);
   }
}

}