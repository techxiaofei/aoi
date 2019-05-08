#include "AOIGrid.h"

namespace AOI{

void AOIGrid::Enter(const Entity& entity){
    int gid = GetGid(entity.x(), entity.y());
    Enter(entity.id(), gid);
    EnterBroadcast(entity, entity.x(), entity.y());
}

void AOIGrid::Enter(uint64_t id, int gid){
    auto it = grids_.find(gid);
    if (it != grids_.end()){
        it->second.insert(id);
        printf("Enter: gid:%d->entityid:%lu\n",gid, id);
    }
}

void AOIGrid::Leave(const Entity& entity){
    int gid = GetGid(entity.x(), entity.y());
    Leave(entity.id(), gid);
    LeaveBroadcast(entity, entity.x(), entity.y());
}

void AOIGrid::Leave(uint64_t id, int gid){
    auto it = grids_.find(gid);
    if (it != grids_.end()){
        it->second.erase(id);
        printf("Leave: gid:%d->entityid:%lu\n",gid, id);
    }
}

/*
* entity存放的坐标是旧的坐标
*/
void AOIGrid::Move(const Entity& entity, int x, int y){
    MoveBroadcast(entity, x, y);

    int old_gid = GetGid(entity.x(), entity.y());
    int new_gid = GetGid(x, y);
    if (old_gid == new_gid){
        return;
    }else{
        Leave(entity.id(), old_gid);
        Enter(entity.id(), new_gid);
        move_cross_grid(entity, x, y);
    }
}

void AOIGrid::InitGrids(){
    int width = width_;
	int height = height_;
	//将场景中的每个坐标都转换为9宫格坐标
	for (int w = 0; w <= width; ++w)
	{
		for (int h = 0; h <= height; ++h)
		{
			int gid = GetGid(w, h);
			auto it = grids_.find(gid);
			//没有初始化过就初始化一次
			if (it == grids_.end()) 
			{
				//std::vector<uint64_t> grid_entities = {};
				grids_.insert({ gid, {} });
				//printf("insert %d\n", gid);
			}
			
		}
	}
}

void AOIGrid::EnterBroadcast(const Entity& entity, int x, int y){
    std::unordered_set<int> enter_grids;
    ViewGrids(enter_grids, x, y);
    enter_message(entity, enter_grids);
}

void AOIGrid::LeaveBroadcast(const Entity& entity, int x, int y){
    std::unordered_set<int> leave_grids;
    ViewGrids(leave_grids, x, y);
    leave_message(entity, leave_grids);
}

void AOIGrid::MoveBroadcast(const Entity& entity, int x, int y){
    std::unordered_set<int> move_grids;
    ViewGrids(move_grids, x, y);
    move_message(entity, move_grids);
}

int AOIGrid::GetGid(int x, int y) const{
    int gx = x / GX;
    int gy = y / GY;
    return GxGy2Gid(gx, gy);
}

int AOIGrid::GxGy2Gid(int gx, int gy) const{
    int gx1 = gx < 0 ? 0 : gx;
    int gy1 = gy < 0 ? 0 : gy;
    return gx1 * FACTOR + gy1;
}

/*
* x,y视野内的Entity，先找到格子
* */
void AOIGrid::ViewEntities(std::unordered_set<uint64_t>& entities, int x, int y){
    std::unordered_set<int> view_grids;
    ViewGrids(view_grids, x, y);
    for (auto gid : view_grids){
        auto grid_it = grids_.find(gid);
        if(grid_it != grids_.end()){
            for (auto grid_entity_id : grid_it->second){
                entities.insert(grid_entity_id);
            }
        }
    }

}

/*
* x,y视野内的格子，就是九宫格
* */
void AOIGrid::ViewGrids(std::unordered_set<int>& view_grids, int x, int y){
    int gx = x / GX;
    int gy = y / GY;
    view_grids.insert({
        GxGy2Gid(gx,gy)
        ,GxGy2Gid(gx,gy-1)
        ,GxGy2Gid(gx,gy+1)
        ,GxGy2Gid(gx-1,gy-1)
        ,GxGy2Gid(gx-1,gy)
        ,GxGy2Gid(gx-1,gy+1)
        ,GxGy2Gid(gx+1,gy-1)
        ,GxGy2Gid(gx+1,gy)
        ,GxGy2Gid(gx+1,gy+1)
    });
}

/*
* gid格子内的Entity
* */
void AOIGrid::GridEntities(std::unordered_set<uint64_t>& entities, int gid){
    auto grid_it = grids_.find(gid);
    if (grid_it != grids_.end()){
        for (auto grid_entity_id : grid_it->second){
            entities.insert(grid_entity_id);
        }
    }
}

/*
AOIGrid::DIR AOIGrid::GetDir(int gx1, int gy1, int gx2, int gy2){
    if (gx1 == gx2){
        if (gy2 > gy1)return AOIGrid::DIR::DIR_UP;
        else return AOIGrid::DIR::DIR_DOWN;
    }
    else if(gy1 == gy2){
        if (gx2 > gy1)return AOIGrid::DIR::DIR_RIGHT;
        else return AOIGrid::DIR::DIR_LEFT;
    }
    else{
        if (gx2 > gx1 && gy2 > gy1)return AOIGrid::DIR::DIR_RIGHT_UP;
        else if (gx2 > gx1 && gy2 < gy1)return AOIGrid::DIR::DIR_RIGHT_DOWN;
        else if (gx2 < gx1 && gy2 > gy1)return AOIGrid::DIR::DIR_LEFT_UP;
        else if (gx2 < gx1 && gy2 < gy1)return AOIGrid::DIR::DIR_LEFT_DOWN;
        else return AOIGrid::DIR::DIR_NULL;
    }
}
*/

void AOIGrid::move_cross_grid(const Entity& entity, int x, int y){
    std::unordered_set<int> leave_grids, enter_grids, move_grids;
    
    int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
    if(gx1 == gx2 && gy1 == gy2)return;

    std::unordered_set<int> old_view_grids,new_view_grids;
    ViewGrids(old_view_grids, entity.x(), entity.y());
    ViewGrids(new_view_grids, x, y);
    for (auto old_grid_id : old_view_grids){
        auto it = new_view_grids.find(old_grid_id);
        if (it == new_view_grids.end()){
            leave_grids.insert(old_grid_id);
        }
        else{
            move_grids.insert(old_grid_id);  //TODO,重复了
        }
    }

    for (auto new_grid_id : new_view_grids){
        auto it = old_view_grids.find(new_grid_id);
        if (it == old_view_grids.end()){
            enter_grids.insert(new_grid_id);
        }
    }

    //move_message(entity, move_grids);
    leave_message(entity, leave_grids);
    enter_message(entity, enter_grids);
}

//TODO,设置callback给外部函数
void AOIGrid::leave_message(const Entity& entity, std::unordered_set<int> leave_grids){
    std::unordered_set<uint64_t> leave_entities;
    for (auto it : leave_grids){
        GridEntities(leave_entities, it);
    }

    for (auto id : leave_entities){
        printf("leave message->to entity:%lu, entityid=%lu,x=%d,y=%d\n",id,entity.id(),entity.x(), entity.y());
    }
}

void AOIGrid::enter_message(const Entity& entity, std::unordered_set<int> enter_grids){
    std::unordered_set<uint64_t> enter_entities;
    for (auto it : enter_grids){
        GridEntities(enter_entities, it);
    }

    for (auto id : enter_entities){
        printf("enter message->to entity:%lu, entityid=%lu,x=%d,y=%d\n",id,entity.id(),entity.x(), entity.y());
    }
}

void AOIGrid::move_message(const Entity& entity, std::unordered_set<int> move_grids){
    std::unordered_set<uint64_t> move_entities;
    for (auto it : move_grids){
        GridEntities(move_entities, it);
    }

    for (auto id : move_entities){
        printf("move message->to entity:%lu, entityid=%lu,x=%d,y=%d\n",id,entity.id(),entity.x(), entity.y());
    }
}

}