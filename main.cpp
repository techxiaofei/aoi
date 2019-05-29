#include <stdint.h>
#include <stdio.h>
#include<vector>
#include <map>
#include "define.h"
#include "Enemy.h"
#include "Character.h"
#include "Entity.h"
#include "AOIManager.h"

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

std::map<int, Character*> pc_map;
std::map<int, Enemy*> enemy_map;

int main(){
    /**
     * 初始化AOI
     */
    int width = 32;
    int height = 32;
    int gx = 8;
    int gy = 8;

    int startx = gx/2;
    int starty = gy/2;

    auto aoi = AOIManager(width, height, gx, gy);
    aoi.setEnterMessageCB(enterNotify);
    aoi.setLeaveMessageCB(leaveNotify);
    aoi.setMoveMessageCB(moveNotify);

    //在每个格子的中间添加
    int idx = 1;
    for(int i = startx; i< width; i=i+gx){
        for(int j = starty; j< height; j=j+gy){
            Character* pc = new Character(idx, i, j);
            idx++;
            //entities.push_back(tmp);
            //aoi.Enter(*tmp);
            pc_map[idx] = pc;

            Enemy* enemy = new Enemy(idx, i, j);
            enemy_map[idx] = enemy;
        }
    }

    for (auto it : pc_map){
        Character* pc = it.second;
        Entity entity(pc->id(),pc->x(),pc->y(), ENTITY_TYPE_PC);
        aoi.Enter(entity);
    }

    //std::vector<Entity*> entities;
    //add one entity per grid
    /*
    int idx = 1;
    for(int i = startx; i< width; i=i+gx){
        for(int j = starty; j< height; j=j+gy){
            Entity* tmp = new Entity(idx, i, j);
            idx++;
            entities.push_back(tmp);
            aoi.Enter(*tmp);
        }
    }*/
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

//g++ -o test Entity.cpp Character.cpp Enemy.cpp AOIGrid.cpp AOIManager.cpp main.cpp --std=c++11