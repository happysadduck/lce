#include "core.h"
#include "config.h"

void update_map(
    Map *map,
    PlanController *controller)
{
    /*
    TODO:
    1. 遍历ship, 处理视野与隐蔽, send_msg并更新controller的monitored
    2. 处理碰撞检测. 如果船发生碰撞, 暂时让它直接消失
    3. 更新飞船和伤害来源的位置
    4. 母舰处理同阵营其他飞船的信息, 更新map中的discovery.
    5. 飞船处理自己母舰的指令, 更新controller的plan
    */
}

void send_command(
    Monitored *monitors,
    const ShipAct *act,
    const Ship *target,
    long send_tick)
{
    /*
    TODO:
    根据act, 直接往monitors中头插法添加message即可.
    */
}

void create_ship(
    Map *map,
    Ship *ship)
{
    /*
    TODO:
    直接把ship添加进map的ships中
    */
}