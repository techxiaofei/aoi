#include <stdint.h>
#include <stdio.h>
#include<vector>
#include "entity.h"
#include "AOIGrid.h"

using namespace AOI;

void enterNotify(const Entity& entity, uint64_t watcher_id){
    printf("enterNotify to %lu->entity %lu enter\n", watcher_id, entity.id());
}

void leaveNotify(const Entity& entity, uint64_t watcher_id){
    printf("enterNotify to %lu->entity %lu leave\n", watcher_id,  entity.id());
}

void moveNotify(const Entity& entity, uint64_t watcher_id){
    printf("enterNotify to %lu->entity %lu move\n", watcher_id,  entity.id());
}

int main(){
    int width = 32;
    int height = 32;

    int startx = GX/2;
    int starty = GY/2;

    std::vector<Entity*> entities;
    //add one entity per grid
    auto aoi = AOIGrid(width, height);
    aoi.setEnterMessageCB(enterNotify);
    aoi.setLeaveMessageCB(leaveNotify);
    aoi.setMoveMessageCB(moveNotify);
    int idx = 1;
    for(int i = startx; i< width; i=i+GX){
        for(int j = starty; j< height; j=j+GY){
            Entity* tmp = new Entity(idx, i, j);
            idx++;
            entities.push_back(tmp);
            aoi.Enter(*tmp);
        }
    }
    /*
    Entity entity(111111, 2,3);
    aoi.Enter(entity);
    for (int i = 0; i < 10; i++){
        int x = entity.x()+1;
        int y = entity.y()+1;
        aoi.Move(entity, x, y);
        entity.set_x(x);
        entity.set_y(y);
    }
    */
}

//g++ -o test Entity.cpp AOIGrid.cpp main.cpp --std=c++11