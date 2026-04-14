#include "core.h"
#include "config.h"

void update_ship(Ship *ship)
{
    ship->p.x += 1.0 / 2 * ship->a.x * TICK_STEP * TICK_STEP + ship->v.x * TICK_STEP;
    ship->p.y += 1.0 / 2 * ship->a.y * TICK_STEP * TICK_STEP + ship->v.y * TICK_STEP;
    ship->v.x += ship->a.x * TICK_STEP;
    ship->v.y += ship->a.y * TICK_STEP;
}

void update_map(
    Map *map,
    PlanController *controller)
{
    /*
    TODO:
    1. 遍历ship, 处理视野与隐蔽, send_msg并更新controller的receiver
    2. 处理碰撞检测.
    3. 更新飞船和伤害来源的位置
    4. 母舰处理同阵营其他飞船的信息, 更新map中的discovery.
    5. 飞船处理自己母舰的指令, 更新controller的plan
    */
    Ship *curr_ship = map->ships;
    while (curr_ship)
    {
        Ship *detected = map->ships;
        while (detected)
        {
            if (detected->king == curr_ship->king)
            {
                detected = detected->next;
                continue;
            }
            Plan *observed_plan = ght_find(&controller->plans, detected, sizeof(detected))->value;
            Discovery *out = pool_alloc(controller->pool_for_discoveries);
            if (get_discovery(
                    observed_plan,
                    curr_ship,
                    detected,
                    map->tick,
                    detected->max_discovered_light_tick,
                    map->squares, 0, out))
            {
                ght_node_t *node = pool_alloc(controller->pool_for_ght_nodes);
                Message *msg = pool_alloc(controller->pool_for_msgs);
                msg->data = out;
                ght_insert(&controller->receivers, node, curr_ship->king, msg, sizeof(curr_ship->king), sizeof(msg));
            }
            else
            {
                pool_return(controller->pool_for_discoveries, out);
            }
        }
        DamageSrc *debeis = map->debris;
        while (debeis)
        {
            Collision collisions[4];
            if (get_collisions(curr_ship, debeis, map->squares, collisions))
            {
                if (calculate_damage(curr_ship, collisions, 0))
                {
                    /*TODO: 生成一个爆炸*/
                    /*TODO: 延迟删除该船只*/
                }
            }
            debeis = debeis->next;
        }
        DamageSrc *laser = map->lasers;
        while (laser)
        {
            Collision collisions[4];
            if (get_collisions(curr_ship, laser, map->squares, collisions))
            {
                if (calculate_damage(curr_ship, collisions, 0))
                {
                    /*TODO: 生成一个爆炸*/
                    /*TODO: 延迟删除该船只*/
                }
            }
        }
        update_ship(curr_ship);
        if (is_king(curr_ship))
        {
            MsgReceiver *receiver;
            receiver = ght_find(&controller->receivers, curr_ship, sizeof(curr_ship));
            if (is_king(curr_ship))
            {
                while (receiver)
                {
                    Discovery *new_discovery = receiver->message.data;
                    new_discovery->next = map->discoveries;
                    map->discoveries = new_discovery;
                    receiver = receiver->next;
                }
            }
            else
            {
                ShipAct *ship_act = receiver->message.data;
                /*....*/
            }
        }
        else
        {
            Message *msg;
            msg = ght_find(&controller->receivers, curr_ship, sizeof(curr_ship));
        }
        curr_ship = curr_ship->next;
    }
    DamageSrc *damage_src = map->debris;
    while (damage_src)
    {
        update_damagesrc(damage_src, map->squares, controller->pool_for_angs);
        damage_src = damage_src->next;
    }
    damage_src = map->lasers;
    while (damage_src)
    {
        update_damagesrc(damage_src, map->squares, controller->pool_for_angs);
        damage_src = damage_src->next;
    }
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