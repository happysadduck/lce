#include "core.h"
#include "config.h"

void update_map(
    Map *map,
    PlanController *controller)
{
    /*
    TODO:
    1. 遍历ship, 处理视野与隐蔽, send_msg并更新controller的receiver
    2. 处理碰撞检测. 如果船发生碰撞, 暂时让它直接消失
    3. 更新飞船和伤害来源的位置
    4. 母舰处理同阵营其他飞船的信息, 更新map中的discovery.
    5. 飞船处理自己母舰的指令, 更新controller的plan
    */
}

void send_msg(
    const ShipAct *act,
    const Ship *src,
    long send_tick,
    void *data,
    MsgReceiver *out)
{
    /*
    TODO:
    根据act, 直接往receiver里写东西
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