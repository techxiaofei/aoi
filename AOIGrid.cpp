#include <stdio.h>
#include "AOIGrid.h"

namespace AOI{

void AOIGrid::Enter(int id){
    entities_.insert(id);
    printf("grid %d add id=%d\n", grid_id_, id);
}

void AOIGrid::Leave(int id){
    entities_.erase(id);

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
   for (auto entity_id : entities_){
       entities.insert(entity_id);
   }
}

}